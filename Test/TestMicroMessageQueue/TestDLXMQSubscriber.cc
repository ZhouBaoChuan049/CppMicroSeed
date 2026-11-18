#include "../../Include/MessageQueue.h"
using namespace MicroSeedLog;
using namespace MicroSeedMessageQueue;

int main(){
    MicroSeedLogInit();
    const std::string Url = "amqp://admin:123456@dev-rabbitmq:5672";
    DeclareSetting Setting = {
        .ExchangeName = "TestDLXExchange",
        .ExchangeType = "delayed",
        .QueueName = "TestDLXQueue",
        .BindAndRouteKey = "TestDLXBindKey",
        .DelaiedTTL = 5000 
    };
    auto MqClient = 
        MessageQueueFactory::Create<MessageQueueClient>(Url);
    auto subscriber= 
        MessageQueueFactory::Create<Subscriber>(MqClient,Setting);
    subscriber->subscribe([](const char* Message, size_t len){
        std::cout<<"ReceiveMessage:["<<std::string(Message,len)<<std::endl;
    });
    MqClient->Wait();
    return 0;
}