#include <brpc/closure_guard.h>
#include <brpc/controller.h>
#include <brpc/http_method.h>
#include <brpc/options.pb.h>
#include <iostream>
#include "HttpService.pb.h"
#include <butil/logging.h>
#include <brpc/server.h>

class CalSimpleService :
    public AddServer::HelloService {
    public:
    CalSimpleService(){}
    ~CalSimpleService(){}
    // void Add(
    //     ::google::protobuf::RpcController* controller,
    //     const ::AddServer::AddRequest* request,
    //     ::AddServer::AddResponse* response,
    //     ::google::protobuf::Closure* done)override {
    //         brpc::ClosureGuard done_guard(done);
    //         int num = request->numx() + request->numy();
    //         response->set_result(num);
    //     }
    void Hello(
        ::google::protobuf::RpcController* controller,
        const ::AddServer::HelloRequest* request,
        ::AddServer::HelloResponse* response,
        ::google::protobuf::Closure* done)override{
            brpc::ClosureGuard guard(done);
            brpc::Controller* cntl = (brpc::Controller*) controller ;
            const auto& headers = cntl->http_request();
            std::cout<<"Method:"<<brpc::HttpMethod2Str(headers.method())<<std::endl;
            std::cout<<"Body:"<<cntl->request_attachment().to_string()<<std::endl;

            cntl->response_attachment().append("#回显:" + cntl->request_attachment().to_string());
            cntl->http_response().set_status_code(200);
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