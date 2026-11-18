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
        .BindAndRouteKey = "TestBindKey",
    };
    auto MqClient = 
        MessageQueueFactory::Create<MessageQueueClient>(Url);
    auto subscriber= 
        MessageQueueFactory::Create<Subscriber>(MqClient,Setting);
    subscriber->subscribe([](const char* Message, size_t len){
        std::cout<<"ReceiveMessage:["<<std::string(Message,len)<<std::endl;
    });
    MqClient->Wait();
    //线程2阻塞在这里。里面是join,去join等待这个异步线程(线程1)
    return 0;
}