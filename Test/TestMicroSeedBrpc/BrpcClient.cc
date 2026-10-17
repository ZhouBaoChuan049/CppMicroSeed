#include "../../Include/Brpc.h"
#include "../../Include/Log.h"
#include "Calculate.pb.h"
#include <brpc/closure_guard.h>
#include <brpc/controller.h>
#include <brpc/http_header.h>
#include <brpc/http_method.h>
#include <butil/iobuf.h>
#include <memory>
#include <thread>
using namespace MicroSeedBrpc ;
using namespace MicroSeedLog ;
int main()
{
    MicroSeedLogInit();
    Channels channels;
    ChannelManager channelmanager ;
    std::string Server_1("Add");
    std::string Server_2("Hello");
    channelmanager.SetWhatServiceNeedTobePayAttention(Server_1);
    channelmanager.SetWhatServiceNeedTobePayAttention(Server_2);
    channelmanager.InsertChannel("127.0.0.1:9000","Add");
    channelmanager.InsertChannel("127.0.0.1:9000", "Hello");

    ChannelPtr channelptr = channelmanager.SelectNode(Server_1);
    AddServer::AddService_Stub stub(channelptr.get()) ;
    brpc::Controller* control = new brpc::Controller();
    AddServer::AddRequest* request = new AddServer::AddRequest() ;
    request->set_numx(10);
    request->set_numy(20);
    AddServer::AddResponse* response = new AddServer::AddResponse();
    ClosureFactory closurefactory;

    google::protobuf::Closure* closure = 
        closurefactory.Create([=](){
                std::unique_ptr<AddServer::AddRequest> ReqPtr(request);
                std::unique_ptr<AddServer::AddResponse> RespPtr(response);
                std::unique_ptr<brpc::Controller> ControlPtr(control);
                if(ControlPtr->Failed() == true){
                    std::cout<<"RP请求失败!"<<std::endl;
                    return ;
                }
                std::cout<<"请求响应:["<<response->result()<<"]"<<std::endl;
            });
    stub.Add(control,
            request,
            response,
            closure
    );
    if(control->Failed() == true){
        ERR("RPC服务调用失败!");
        return -1 ;
    }
    std::cout <<"===================================================\n";
    getchar();
    return 0 ;
}