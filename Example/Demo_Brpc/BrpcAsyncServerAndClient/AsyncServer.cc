#include <iostream>
#include "AsyncServer.pb.h"
#include <butil/logging.h>
#include <brpc/server.h>
#include <thread>
class AsyncAddServer : public AsyncCalServer::AsyncCalService{
    public:
        AsyncAddServer(){}
        ~AsyncAddServer(){}
        virtual void Add(::google::protobuf::RpcController* controller,
                    const ::AsyncCalServer::Request* request,
                    ::AsyncCalServer::Response* response,
                    ::google::protobuf::Closure* done){
                        std::thread AsyncThread([=](){
                            brpc::ClosureGuard guard(done);
                            response->set_result(request->numx() + request->numy());
                        });
                        AsyncThread.detach();
                        // if(controller->Failed() == true){
                        //    std::cout<<"RPC调用出错!"<<std::endl;
                        //    return ; 
                        // }
        }
};
int main()
{
    AsyncAddServer asyncserver ;
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;
    options.num_threads = 2 ;
    brpc::Server server ;
    int ret = server.AddService(
        &asyncserver, 
        brpc::ServiceOwnership::SERVER_DOESNT_OWN_SERVICE
    );
    if(ret == -1){
        std::cout<<"AddServerFail!"<<std::endl;
        return -1;
    }
    ret = server.Start("127.0.0.1:9000", &options);
    if(ret == -1){
        std::cout<<"StartServerFail!"<<std::endl;
        return -1 ;
    }
    server.RunUntilAskedToQuit();
    return 0 ;
}