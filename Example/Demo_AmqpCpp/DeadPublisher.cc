#include <amqpcpp.h>
#include <amqpcpp/address.h>
#include <amqpcpp/channel.h>
#include <amqpcpp/connection.h>
#include <amqpcpp/exchangetype.h>

#include <amqpcpp/table.h>
#include <cstdlib>
#include <ev.h>
#include <amqpcpp/libev.h>
#include <iostream>

const std::string RabbitMqUrl = "amqp://admin:123456@dev-rabbitmq:5672/";
const std::string ExchangeName = "NumberNine";
const std::string QueueName = "NumberSeven";
const std::string BindAndRouteKey = "DefaultBindAndRouteKey";

const std::string DeadExchangeName = "DeathExchange";
const std::string DeadQueueName = "DeathQueue";
const std::string DeadBindAndRouteKey = "DeathBindAndRouteKey";

void DeclareExchangeAndQueue(AMQP::TcpChannel& channel){
    channel.declareExchange(
        DeadExchangeName,
        AMQP::ExchangeType::direct,
        AMQP::autodelete 
    ).onError([&](const char* ErrMessage){
        std::cout<<"死信交换机声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
        abort();
    }).onSuccess([&](){
        std::cout<<"死信交换机"<<DeadExchangeName<<"声明成功"<<std::endl;
        channel.declareQueue(DeadQueueName)
            .onSuccess([&](const std::string& Name,
                uint32_t MessageCount,
                uint32_t ConsumerCount){
                    std::cout << "声明队列成功:" << DeadQueueName << std::endl;
                    std::cout << "队列中有" << MessageCount << "个消息" << std::endl;
                    std::cout << "队列中有" << ConsumerCount << "个消费者" << std::endl;
                    channel.bindQueue(
                        DeadExchangeName,
                        DeadQueueName,
                        DeadBindAndRouteKey)
                    .onSuccess([&](){
                        std::cout<<"绑定交换机和队列成功!"<<std::endl;
                    }).onError([&](const char* ErrMessage){
                        std::cout<<"绑定交换机和队列失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
                        abort();
                    });
            })
            .onError([&](const char* ErrMessage){
                std::cout<<"队列声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
                abort();
            });
    });
}
int main(){
    auto EvLoop= EV_DEFAULT;
    AMQP::LibEvHandler Handler(EvLoop);
    AMQP::TcpConnection connection(
        &Handler,
        AMQP::Address(RabbitMqUrl)
    );
    AMQP::TcpChannel channel(&connection);
    DeclareExchangeAndQueue(channel);
    channel.declareExchange(
        ExchangeName,
        AMQP::ExchangeType::direct,
        AMQP::autodelete 
    ).onError([&](const char* ErrMessage){
        std::cout<<"交换机声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
        abort();
    }).onSuccess([&](){
        std::cout<<"交换机"<<ExchangeName<<"声明成功"<<std::endl;
        AMQP::Table args ;
        args["x-dead-letter-exchange"] = DeadExchangeName;
        args["x-dead-letter-routing-key"] = DeadBindAndRouteKey;
        args["x-message-ttl"] = 5000;
        channel.declareQueue(QueueName,args)
            .onSuccess([&](const std::string& Name,
                uint32_t MessageCount,
                uint32_t ConsumerCount){
                    std::cout << "声明队列成功:" << QueueName << std::endl;
                    std::cout << "队列中有" << MessageCount << "个消息" << std::endl;
                    std::cout << "队列中有" << ConsumerCount << "个消费者" << std::endl;
                    channel.bindQueue(
                        ExchangeName,
                        QueueName,
                        BindAndRouteKey)
                    .onSuccess([&](){
                        std::cout<<"绑定交换机和队列成功!"<<std::endl;
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
            })
            .onError([&](const char* ErrMessage){
                std::cout<<"队列声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
                abort();
            });
    });
    ev_run(EvLoop);
    return 0;
}