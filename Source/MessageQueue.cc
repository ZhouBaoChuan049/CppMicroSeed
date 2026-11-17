#include "../Include/MessageQueue.h"

namespace MicroSeedMessageQueue{
    std::string DeclareSetting::DLX_ExchangeName()const{ return DLX_PERFIX + ExchangeName; }
    std::string DeclareSetting::DLX_QueueName()const{ return DLX_PERFIX + QueueName; }
    std::string DeclareSetting::DLX_BindAndRouteKey()const{ return DLX_PERFIX + BindAndRouteKey; }
    static AMQP::ExchangeType TransformTypeFromString(const std::string type){
        std::unordered_map<std::string,AMQP::ExchangeType> TypeExchanges ;
        TypeExchanges[EXCHANGETYPE_DIRECT ] = AMQP::ExchangeType::direct ;
        TypeExchanges[EXCHANGETYPE_FANOUT ] = AMQP::ExchangeType::fanout ;
        TypeExchanges[EXCHANGETYPE_TOPIC  ] = AMQP::ExchangeType::topic  ;
        TypeExchanges[EXCHANGETYPE_HEADERS] = AMQP::ExchangeType::headers;
        TypeExchanges[EXCHANGETYPE_DELAYED] = AMQP::ExchangeType::direct ;
        //AMQPCPP默认没有延迟交换机的类型,我们默认延迟队列为直接交换
        if(TypeExchanges.find(type) == TypeExchanges.end()){
            ERR("声明的交换机类型错误! \
                交换机正确类型范畴:(direct、fanout、topic、headers、delayed)");
            abort();
        }
        return TypeExchanges[type];
    }
    MessageQueueClient::MessageQueueClient(const std::string url)
        :_EvLoop(EV_DEFAULT),
         _Handler(_EvLoop), //这边的构造先后顺序不能调换位置
         _Connection(&_Handler, AMQP::Address(url)),
         _Channel(&_Connection),
         _AsyncThread([=](){ev_run(_EvLoop);})
    {}//启动异步事件循环，这个就是我们架构图上介绍的线程1
    MessageQueueClient::~MessageQueueClient(){
        //初始化异步监视器，绑定一个回调函数，
        //然后发送给AMQPCPP客户端，
        //线程1执行异步监视器的回调函数终止异步事件循环
        ev_async_init(&_EvAsync, _AsyncCallback);
        ev_async_start(_EvLoop, &_EvAsync);
        ev_async_send(_EvLoop, &_EvAsync);
        _AsyncThread.join();
    }
    static void _AsyncCallback(
        struct ev_loop* loop, 
        ev_async* watcher, 
        int32_t revents){
            ev_break(loop,EVBREAK_ALL);
    }
    void MessageQueueClient::_Declared(
        const DeclareSetting& setting,
        AMQP::Table& args, 
        bool is_dlx) {
            //为什么这里要有两把锁？
            //第二把锁，是为了配合后面的Wait使用的,一个线程持有锁,去执行声明操作,然后Wait()释放锁。
            //第一把锁，是为了防止同时有两个线程并发的进行声明,操作channel会出问题.
            std::unique_lock<std::mutex> DeclaredLock(_DeclaredMutex);
            std::unique_lock<std::mutex> Lock(_Mutex);
            std::string Exchange, Queue, BindKey;
            if(is_dlx == true){
                Exchange = setting.DLX_ExchangeName();
                Queue = setting.DLX_QueueName();
                BindKey = setting.DLX_BindAndRouteKey();
            }else{
                Exchange = setting.ExchangeName;
                Queue = setting.QueueName;
                BindKey = setting.BindAndRouteKey;
            }
            _Channel.declareExchange(
                Exchange,
                TransformTypeFromString(setting.ExchangeType),
                AMQP::autodelete).onError([=](const char* ErrMessage){
                                    ERR("交换机[{}]声明失败!详细错误信息查看[{}]",Exchange,ErrMessage);
                                }).onSuccess([=](){//C++17标准允许隐式捕捉this指针
                                    //因为异步回调的执行时间不确定，可能晚于当前函数中局部变量的生命周期
                                    //就是说，异步回调执行的时候，我们的这个函数可能已经结束了，栈帧已经被释放了。
                                    INF("交换机[{}]声明成功!",Exchange);
                                    // 5. 声明队列
                                    _Channel.declareQueue(Queue,args)
                                    //传入参数,在外部再包装一层确定我们要声明的是死信队
                                    //列还是一个普通队列,于是,这样一设计,我们就大幅优化了代码
                                        .onSuccess([=](const std::string& Name,
                                            uint32_t MessageCount,
                                            uint32_t ConsumerCount){
                                                INF("声明队列[{}]成功",Name);
                                                INF("队列中有[{}]个消息",MessageCount);
                                                INF("队列中有[{}]个消费者",ConsumerCount);
                                                // 6. 绑定交换机和队列(采用直接交换)
                                                _Channel.bindQueue(
                                                    Exchange,
                                                    Queue,
                                                    BindKey).onSuccess([=](){
                                                        INF("绑定交换机和队列成功!");
                                                    }).onError([=](const char* ErrMessage){
                                                        ERR("绑定交换机和队列失败!错误信息:[{}]",ErrMessage);
                                                        abort();
                                                    });
                                        })//这个调用链还有先后顺序,onError返回的不是DeferredQueue调用不了onSuccess
                                        .onError([=](const char* ErrMessage){
                                            ERR("队列声明失败!错误信息:[{}]",ErrMessage);
                                            abort();
                                        });
                                });
        _Cv.wait(Lock);
    }
    void MessageQueueClient::Declare(const DeclareSetting& setting){
        std::string Type = setting.ExchangeType;
        AMQP::Table args ;
        if(Type == EXCHANGETYPE_DELAYED){
            _Declared(setting,args, true);//创建死信队列
            //你创建延时队列时，需要告诉 RabbitMQ：这个队列中的消息过期后，应该转发到哪个死信交换机，以及使用什么 Routing Key。
            args[DEAD_LETTER_EXCHANGE] = "x-dead-letter-exchange";
            args[DEAD_LETTER_ROUTING_KEY] = "x-dead-letter-routing-key";
            args[MESSAGE_TTL] = "x-message-ttl";
        }
        _Declared(setting, args,false);
    }
    bool MessageQueueClient::publish(
        const std::string& exchange, 
        const std::string& routekey, 
        const std::string& body){
            return _Channel.publish(exchange,routekey,body);
    }
    void MessageQueueClient::consume(
        const std::string& queue, const MessageCallBack& callback){
            std::unique_lock<std::mutex> DeclaredLock(_DeclaredMutex);
            std::unique_lock<std::mutex> Lock(_Mutex);
            _Channel.consume(queue).onMessage([=](
                                    const AMQP::Message &message,//收到的消息
                                    uint64_t deliveryTag,//消息的唯一标识
                                    bool redelivered){ //消息是否已经被重复发过了
                                        std::string body(message.body(),message.bodySize());
                                        INF("订阅者收到消息[{}]",body);
                                        _Channel.ack(deliveryTag);
                                    }).onSuccess([=](){
                                        INF("订阅队列消息成功!");
                                    }).onError([=](const char* ErrMessage){
                                        INF("订阅队列消息失败!错误信息:[{}]",ErrMessage);
                                        abort();     
                                    });
         _Cv.wait(Lock);
    }
    void MessageQueueClient::Wait(){
        //看那张设计图,我们的线程1(业务执行线程)他是异步执行的,
        //如果线程1没有事情好执行,那么线程1先结束后
        _AsyncThread.join();//所以得在这里阻塞住
    }
    Publisher::Publisher(
        MessageQueueClient::MQPtr client, 
        DeclareSetting setting
    ):_Client(client),
      _Setting(setting){
        _Client->Declare(_Setting);
    }
    bool Publisher::publish(std::string messagebody){
        return _Client->publish(
            _Setting.ExchangeName, 
            _Setting.BindAndRouteKey, 
            messagebody);
    }
    Subscriber::Subscriber(
        MessageQueueClient::MQPtr client, 
        DeclareSetting setting
    ):_Client(client),
      _Setting(setting){
        _Client->Declare(_Setting);
    }
    void Subscriber::subscribe(MessageCallBack&& callback){
        _Callback = std::move(callback);
        if(_Setting.ExchangeType == EXCHANGETYPE_DELAYED){
            _Client->consume(_Setting.DLX_QueueName(), callback);
        }else{
            _Client->consume(_Setting.QueueName, callback);
        }
    }
}