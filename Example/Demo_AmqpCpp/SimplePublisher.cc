#include <amqpcpp.h>
#include <amqpcpp/address.h>
#include <amqpcpp/channel.h>
#include <amqpcpp/connection.h>
#include <amqpcpp/exchangetype.h>

#include <cstdlib>
#include <ev.h>
#include <amqpcpp/libev.h>
#include <iostream>

const std::string RabbitMqUrl = "amqp://admin:123456@dev-rabbitmq:5672/";
const std::string ExchangeName = "NumberNine";
const std::string QueueName = "NumberSeven";
const std::string BindAndRouteKey = "DefaultBindAndRouteKey"; //绑定键 == 路由键必须。
//绑定键用在那个绑定队列里面,路由键用在那个发布消息里面
//就是说。我们两个客户端先约定好队列和交换机的名称，然后再声明交换机和队列，
// 然后发送消息到交服务器上制定的交换机，交换机检查绑定键和路由键，
// 知道了——’哦，你这个消息是要发送到这个队列里面的，‘于是就把你的消息放到这个队列里面了
int main(){
    //1. 实例化libev网络通信模块句柄
    auto EvLoop= EV_DEFAULT;
    AMQP::LibEvHandler Handler(EvLoop);
    //2. 实例化Connection对象
    AMQP::TcpConnection connection(
        &Handler,
        AMQP::Address(RabbitMqUrl)
    );
    //3. 实例化Channel对象
    AMQP::TcpChannel channel(&connection);
    //4. 声明交换机
    channel.declareExchange(
        ExchangeName,
        AMQP::ExchangeType::direct,//直接交换机吧，简单一点
        AMQP::autodelete // 交换机可以自己删,条件是当最后一个绑定失效
    ).onError([&](const char* ErrMessage){
        std::cout<<"交换机声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
        abort();
    }).onSuccess([&](){
        std::cout<<"交换机"<<ExchangeName<<"声明成功"<<std::endl;
        // 5. 声明队列
        channel.declareQueue(QueueName)
            .onSuccess([&](const std::string& Name,
                uint32_t MessageCount,
                uint32_t ConsumerCount){
                    std::cout << "声明队列成功:" << QueueName << std::endl;
                    std::cout << "队列中有" << MessageCount << "个消息" << std::endl;
                    std::cout << "队列中有" << ConsumerCount << "个消费者" << std::endl;
                    // 6. 绑定交换机和队列(采用直接交换)
                    channel.bindQueue(
                        ExchangeName,
                        QueueName,
                        BindAndRouteKey)
                    .onSuccess([&](){
                        std::cout<<"绑定交换机和队列成功!"<<std::endl;
                        // 7. 发送消息到交换机
                        bool ret = channel.publish(
                            ExchangeName,
                            BindAndRouteKey,
                            "Ciallo World !"
                        );
                        if(ret == false){
                            std::cout<<"发送消息失败"<<std::endl;
                        }
                    }).onError([&](const char* ErrMessage){
                        std::cout<<"绑定交换机和队列失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
                        abort();
                    });
            })//这个调用链还有先后顺序,onError返回的不是DeferredQueue调用不了onSuccess
            .onError([&](const char* ErrMessage){
                std::cout<<"队列声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
                abort();
            });
    });
    //启动事件循环
    ev_run(EvLoop);
    return 0;
}