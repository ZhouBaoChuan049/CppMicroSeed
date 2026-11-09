#include <iostream>
#include "Server.pb.h"
#include <butil/logging.h>
#include <brpc/server.h>

class CalSimpleService : public AddServer::AddServer{
    public:
    CalSimpleService(){}
    ~CalSimpleService(){}
    virtual void Add(
        google::protobuf::RpcController* controller,
        const ::AddServer::MessageReq* request,
        ::AddServer::MessageResp* response,
        ::google::protobuf::Closure* done){
            brpc::ClosureGuard done_guard(done);
            int num = request->num1() + request->num2();
            response->set_result(num);
            //sdone->Run();
        }
};

int main()
{
    CalSimpleService CalService ;
    brpc::ServerOptions options ;
    options.idle_timeout_sec = -1 ;
    brpc::Server server ;
    int ret = server.AddService(&CalService, brpc::SERVER_DOESNT_OWN_SERVICE);
    if(ret == -1){
        std::cout<<"添加服务失败!"<<std::endl;
        return -1 ;
    }
    ret = server.Start(9000,&options);
    if(ret == -1){
        std::cout<<"服务器启动失败!"<<std::endl;
        return -1 ;
    }
    server.RunUntilAskedToQuit();
    return 0 ; 
}