#include <iostream>
#include <fastcommon/logger.h>
#include <fastdfs/fdfs_client.h>
int main(){
    //初始化日志输出模块
    g_log_context.log_level = LOG_ERR;
    //初始化全局日志上下文，return: 0 for success, != 0 fail
    log_init();
    //初始化全局配置
    int ret = fdfs_client_init("./Initializer.conf");
    if(ret != 0){
        std::cout<<"初始化全局配置失败!失败原因:["<<STRERROR(ret)<<"]"<<std::endl;
        return -1;
    }
    //获取tracker服务器连接句柄
    auto TrackerServer = tracker_get_connection();
    if(TrackerServer == nullptr){
        std::cout<<"获取Tracker服务器连接句柄失败!"<<std::endl;
        return -1;
    }
    //进行文件的上传
    char FileId[256];
    ret = storage_upload_by_filename1(
        TrackerServer, nullptr, 0, 
        "WhatisTheFastdfs.md", 
        nullptr, nullptr, 0, nullptr, FileId);
    if(ret != 0){
        std::cout<<"文件上传失败!错误原因:["<<STRERROR(ret)<<"]"<<std::endl;
        return -1;
    }
    std::cout<<"文件上传成功,文件id:"<<FileId<<std::endl;
    //释放资源（关闭tracker服务器连接句柄， 销毁客户端全局配置）
    tracker_close_connection(TrackerServer);
    fdfs_client_destroy();
    return 0;
}

