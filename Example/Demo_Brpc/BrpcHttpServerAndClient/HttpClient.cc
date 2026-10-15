#include <brpc/http_header.h>
#include <brpc/http_method.h>
#include <brpc/options.pb.h>
#include <google/protobuf/stubs/callback.h>
#include <iostream>
#include <memory>
#include "HttpService.pb.h"
#include <brpc/channel.h>
#include <brpc/controller.h>

int main(int argc,char* argv[]){
    brpc::Channel channel ;
    brpc::ChannelOptions options ;
    options.connect_timeout_ms = 500 ;
    options.timeout_ms = 500 ;
    options.max_retry = 5 ;
    options.protocol = brpc::PROTOCOL_HTTP;
    if(channel.Init(
        "127.0.0.1:9000",
        &options)!=0){
            std::cout<<"客户端的信道初始化失败!"<<std::endl;
            return -1 ;
    }
    std::shared_ptr<brpc::Controller> control = std::make_shared<brpc::Controller>() ;

    brpc::HttpHeader& header = control->http_request();
    header.set_method(brpc::HTTP_METHOD_POST);
    header.uri().set_path("/HelloService/Hello");
    header.SetHeader("Content-Type", "text/plain");
    control->request_attachment().append("Ciallo World!");

    channel.CallMethod(
        nullptr, 
        control.get(), 
        nullptr, 
        nullptr, 
        nullptr
    );
    if(control->Failed() == true){
        std::cout<<"RPC服务出现异常!["<<control->ErrorText()<<"]"<<std::endl;
    }else{
        std::cout<<control->response_attachment()<<std::endl;
    }
    return 0 ;
}