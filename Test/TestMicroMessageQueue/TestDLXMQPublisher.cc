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
    auto publisher= 
        MessageQueueFactory::Create<Publisher>(MqClient,Setting);
    publisher->publish("Ciallo World!");
    return 0;
}