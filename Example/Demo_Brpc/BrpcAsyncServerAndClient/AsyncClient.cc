#include <brpc/callback.h>
#include <cstdio>
#include <google/protobuf/stubs/callback.h>
#include <iostream>
#include <brpc/channel.h>
#include <brpc/controller.h>
#include "AsyncServer.pb.h"
#include <functional>
#include <memory>

using func_t = std::function<void()> ;

struct Object{ func_t _obj ; };
void Callback(Object obj){ obj._obj();}

int main()
{
    brpc::Channel channel ;
    brpc::ChannelOptions options ;
    options.backup_request_ms = 5000 ;
    options.connect_timeout_ms = 300 ;
    options.max_retry = 5 ;
    options.protocol = "baidu_std" ;
    channel.Init("127.0.0.1:9000", &options);
    AsyncCalServer::AsyncCalService_Stub stub(&channel); 

    AsyncCalServer::Request* request = new AsyncCalServer::Request ;
    request->set_numx(10);
    request->set_numy(20);
    AsyncCalServer::Response* response = new AsyncCalServer::Response() ;
    brpc::Controller* control = new brpc::Controller() ;

    Object object ;
    object._obj = [=](){
        std::unique_ptr<AsyncCalServer::Request> ReqPtr(request);
        std::unique_ptr<AsyncCalServer::Response> RespPtr(response);
        std::unique_ptr<brpc::Controller> ControlPtr(control);
        if(ControlPtr->Failed() == true){
            std::cout<<"RP请求失败!"<<std::endl;
            return ;
        }
        std::cout<<"请求响应:["<<response->result()<<"]"<<std::endl;
    };
    google::protobuf::Closure* closure = brpc::NewCallback(Callback, object);
    stub.Add(control, request, response, closure);
    std::cout <<"===================================================\n";
    getchar();
    return 0 ;
}
