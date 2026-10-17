#include "../../Include/Brpc.h"
#include "Calculate.pb.h"
#include <brpc/closure_guard.h>
#include <brpc/controller.h>
#include <brpc/http_header.h>
#include <brpc/http_method.h>
#include <butil/iobuf.h>
#include <memory>
#include <thread>
using namespace MicroSeedBrpc ;
class RealCalServer 
    : public AddServer :: AddService 
{
    public:
        RealCalServer(){}
        ~RealCalServer(){}
        void Add(::google::protobuf::RpcController* controller,
                    const ::AddServer::AddRequest* request,
                    ::AddServer::AddResponse* response,
                    ::google::protobuf::Closure* done)override{
                        std::thread Thread(
                            [=](){
                                brpc::ClosureGuard guard(done);
                                int x = request->numx();
                                int y = request->numy();
                                response->set_result(x+y);
                            }
                        );
                        Thread.detach();
    }
};
class RealHelloServer    
    : public AddServer :: HelloService{
        public:
            void Hello(::google::protobuf::RpcController* controller,
                const ::AddServer::HelloRequest* request,
                ::AddServer::HelloResponse* response,
                ::google::protobuf::Closure* done)override{
                    brpc::ClosureGuard guard(done);
                    brpc::Controller* control = (brpc::Controller*)controller;
                    const auto& requestheader = control->http_request();
                    std::cout<<"Method:"<<brpc::HttpMethod2Str(requestheader.method())<<std::endl;
                    std::cout<<"Body:"<<control->request_attachment().to_string()<<std::endl;

                    butil::IOBuf responsebuffer = control->response_attachment();
                    responsebuffer.append("#回显:"+control->request_attachment().to_string());
                    control->http_response().set_status_code(200);
    }
};
int main(){
    RpcServerFactory rpcserver_1 ;
    std::shared_ptr<RealCalServer> calservice = std::make_shared<RealCalServer>();
    std::shared_ptr<brpc::Server> serv_1 = rpcserver_1.Create(9000, calservice);
    if(!serv_1){
        return -1 ;
    }
    
    RpcServerFactory rpcserver_2 ;
    std::shared_ptr<RealHelloServer> helloservice = std::make_shared<RealHelloServer>();
    std::shared_ptr<brpc::Server> serv_2 = rpcserver_2.Create(9001, helloservice);
    if(!serv_2){
        return -1 ;
    }
    
    std::thread t1([&](){ serv_1->RunUntilAskedToQuit(); });
    std::thread t2([&](){ serv_2->RunUntilAskedToQuit(); });
    t1.join();
    t2.join();
    return 0 ;
}
