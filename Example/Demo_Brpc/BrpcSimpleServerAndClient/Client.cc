#include <google/protobuf/stubs/callback.h>
#include <iostream>
#include "Server.pb.h"
#include <brpc/channel.h>
#include <brpc/controller.h>

int main(){
    brpc::Channel channel ;
    brpc::ChannelOptions options ;
    options.connect_timeout_ms = 500 ;
    options.timeout_ms = 500 ;
    options.max_retry = 5 ;
    options.protocol = "baidu_std";
    if(channel.Init(
        "127.0.0.1:9000",
        &options)!=0){
            std::cout<<"客户端的信道初始化失败!"<<std::endl;
            return -1 ;
    }
    AddServer::AddServer_Stub stub(&channel) ;
    AddServer::MessageReq request ;
    request.set_num1(56);
    request.set_num2(562);
    AddServer::MessageResp response ;

    brpc::Controller control ;
    stub.Add(
        &control,
        &request,
        &response,
        nullptr
    );
    if(control.Failed() == true){
        std::cout<<"Add执行失败!"<<std::endl;
        return -1 ;
    }
    std::cout<<"["<<"Result:"<<response.result()<<"]"<<std::endl;
    return 0 ;
}