#include "../../Include/Brpc.h"
#include "../../Include/Log.h"
#include "../../Include/Etcd.h"
#include "Calculate.pb.h"
#include <brpc/closure_guard.h>
#include <brpc/controller.h>
#include <brpc/http_header.h>
#include <brpc/http_method.h>
#include <butil/iobuf.h>
#include <memory>
#include <thread>
using namespace MicroSeedEtcd ;
using namespace MicroSeedBrpc ;
using namespace MicroSeedLog ;
int main()
{
    const std::string CenterAddress = "http://dev-etcd:2379";
    std::string ServerName_1 = "Calculate";
    //std::string ServerName_2 = "Hello";
    //只演示其中一个服务,Hello服务不去注册,上线后任然会告诉我们,但是不会去管他.
    MicroSeedLogInit();
    Channels channels;
    ChannelManager channelmanager ;
    channelmanager.SetWhatServiceNeedTobePayAttention(ServerName_1);
    //channelmanager.SetWhatServiceNeedTobePayAttention(ServerName_2);
    channelmanager.InsertChannel(ServerName_1,"127.0.0.1:9000");
    //channelmanager.InsertChannel(ServerName_2, "127.0.0.1:9000");

    auto OnlineCb = std::bind(
        &ChannelManager::InsertChannel,
        &channelmanager,
        std::placeholders::_1,
        std::placeholders::_2
    );
    auto OfflineCb = std::bind(
        &ChannelManager::DeleteChannel,
        &channelmanager,
        std::placeholders::_1,
        std::placeholders::_2
    );
    EtcdServiceWatcher watcher(CenterAddress,OnlineCb,OfflineCb);
    watcher.DataWatch();
    ChannelPtr channelptr = channelmanager.SelectNode(ServerName_1);
    //通过 BRPC 客户端,异步调用远程 AddServer 服务,并在收到响应后执行回调函数.
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
                    std::cout<<"RPC请求失败!"<<std::endl;
                    return ;
                }
                std::cout<<"请求响应:["<<response->result()<<"]"<<std::endl;
            });
    stub.Add(control,
            request,
            response,
            closure
    );
    std::cout <<"===================================================\n";
    getchar();
    return 0 ;
}