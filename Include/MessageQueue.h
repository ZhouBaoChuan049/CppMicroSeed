#pragma once
#include <amqpcpp.h>
#include <amqpcpp/exchangetype.h>
#include <amqpcpp/libev.h>
#include <ev.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <unordered_map>
#include "Log.h"
namespace MicroSeedMessageQueue{
    const std::string DLX_PERFIX = "dlx_";
    const std::string EXCHANGETYPE_DIRECT  = "direct" ;
    const std::string EXCHANGETYPE_FANOUT  = "fanout" ;
    const std::string EXCHANGETYPE_TOPIC   = "topic"  ;
    const std::string EXCHANGETYPE_HEADERS = "headers";
    const std::string EXCHANGETYPE_DELAYED = "delayed" ;
    const std::string DEAD_LETTER_EXCHANGE = "x-dead-letter-exchange" ;
    const std::string DEAD_LETTER_ROUTING_KEY = "x-dead-letter-routing-key" ;
    const std::string MESSAGE_TTL = "x-message-ttl" ;


    struct DeclareSetting{
        std::string ExchangeName ;
        std::string ExchangeType ;
        std::string QueueName ;
        std::string BindAndRouteKey ;
        size_t DelaiedTTL = 0 ;

        std::string DLX_ExchangeName() const;
        std::string DLX_QueueName() const;
        std::string DLX_BindAndRouteKey() const;
    };
    static AMQP::ExchangeType TransformTypeFromString(const std::string type);
    using MessageCallBack = std::function<void(const char*, size_t)>;
    class MessageQueueClient{
    public:
        using MQPtr = std::shared_ptr<MessageQueueClient>;
        MessageQueueClient(const std::string url);
        ~MessageQueueClient();
        bool publish(
            const std::string& exchange, 
            const std::string& routekey, 
            const std::string& body
        );
        void Declare(const DeclareSetting& setting);
        void consume(const std::string& queue, const MessageCallBack& callback);
        void Wait();

    private:
        static void _AsyncCallback(struct ev_loop* loop, ev_async* watcher, int32_t revents);
        void _Declared(const DeclareSetting& setting, AMQP::Table& args, bool is_dlx = false);
    private:
        std::mutex _DeclaredMutex ;
        std::mutex _Mutex ;
        std::condition_variable _Cv ;
        
        struct ev_loop* _EvLoop ;
        struct ev_async _EvAsync ;

        AMQP::LibEvHandler _Handler ;
        AMQP::TcpConnection _Connection ;
        AMQP::TcpChannel _Channel ;
        
        std::thread _AsyncThread ;
    };
    class Publisher{
    public:
        Publisher(MessageQueueClient::MQPtr client, 
            DeclareSetting setting);
        bool publish(std::string messagebody);
    private:
        MessageQueueClient::MQPtr _Client ;
        DeclareSetting _Setting ;
    };
    class Subscriber{
    public: 
        Subscriber(MessageQueueClient::MQPtr client, 
            DeclareSetting setting);
        void subscribe(MessageCallBack&& callback);
    private:
        MessageQueueClient::MQPtr _Client ;
        DeclareSetting _Setting ;
        MessageCallBack _Callback ;
    };

    class MessageQueueFactory {
    public:
        // 通用对象工厂:统一封装不同类型对象的创建过程。
        template<typename R, typename... Args>
        // R：需要创建的对象类型。
        // Args...:可变模板参数，可以接收任意数量、任意类型的参数。
        static std::shared_ptr<R> Create(Args&&... args) {
            // Args&&... args：可变参数函数形参包。
            // Args&& 在模板参数推导时属于转发引用，可以接收左值和右值。
            return std::make_shared<R>(std::forward<Args>(args)...);
            //   std::make_shared<R>：创建 R 类型对象，并返回 shared_ptr。
            //   std::forward<Args>(args)...：完美转发参数包。
            //   保留每个参数原本的左右值属性，避免不必要的拷贝。
            //   最后的 ... 表示展开参数包，将所有参数传递给构造函数。
        }
    };

}