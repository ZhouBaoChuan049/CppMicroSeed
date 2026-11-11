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
const std::string DeadExchangeName = "DeathExchange";
const std::string DeadQueueName = "DeathQueue";
const std::string DeadBindAndRouteKey = "DeathBindAndRouteKey";


int main(){
    auto EvLoop= EV_DEFAULT;
    AMQP::LibEvHandler Handler(EvLoop);
    AMQP::TcpConnection connection(
        &Handler,
        AMQP::Address(RabbitMqUrl)
    );
    AMQP::TcpChannel channel(&connection);
    channel.declareExchange(
        DeadExchangeName,
        AMQP::ExchangeType::direct,
        AMQP::autodelete 
    ).onError([&](const char* ErrMessage){
        std::cout<<"交换机声明失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
        abort();
    }).onSuccess([&](){
        std::cout<<"交换机"<<DeadExchangeName<<"声明成功"<<std::endl;
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
                        channel.consume(DeadQueueName)
                        .onMessage([&]
                            (const AMQP::Message &message,
                            uint64_t deliveryTag,
                            bool redelivered 
                        ){
                            std::string body(message.body(),message.bodySize());
                            std::cout<<"收到的消息["<<body<<"]"<<std::endl;
                            channel.ack(deliveryTag);
                        })
                        .onSuccess([&](){
                            std::cout<<"订阅队列消息成功!"<<std::endl; 
                        }).onError([&](const char* ErrMessage){
                            std::cout<<"订阅队列消息失败!错误信息:["<<ErrMessage<<"]"<<std::endl;
                            abort();     
                        });
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