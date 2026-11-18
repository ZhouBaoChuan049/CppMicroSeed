#include "../../Include/MessageQueue.h"
using namespace MicroSeedLog;
using namespace MicroSeedMessageQueue;

int main(){
    MicroSeedLogInit();
    const std::string Url = "amqp://admin:123456@dev-rabbitmq:5672";
    DeclareSetting Setting = {
        .ExchangeName = "TestSimpleExchange",
        .ExchangeType = "direct",
        .QueueName = "TestSimpleQueue",
        .BindAndRouteKey = "TestBindKey"
    };
    auto MqClient = 
        MessageQueueFactory::Create<MessageQueueClient>(Url);
    auto publisher= 
        MessageQueueFactory::Create<Publisher>(MqClient,Setting);
    publisher->publish("Ciallo World!");
    //RabitMQ的机制和Etcd的机制差别很大:
    //消息发布到RabitMQ服务器,设置TTL = 0的时候不需要保活
    //消息直到被消费后被移除。
    //或者一种特殊的情况:消息队列满了，
    //RabitMQ可以选择不接收新消息，或者可以选择移除队伍头部的最旧的消息
    //如果设置了死信队列，那么就会扔到死信队列里面去
    //如果死信队列也满了呢？扔掉。或者不接收新消息。等待消费者消费。
    return 0;
}