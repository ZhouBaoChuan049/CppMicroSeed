#include <iostream>
#include <fastcommon/logger.h>
#include <fastdfs/fdfs_client.h>
int main(int argc , char* argv[]){
    if(argc != 2){
        std::cout<<"命令行参数错误!正确样例: ./Download fileid"<<std::endl;
        return -1;
    }
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
    //进行文件的下载
    int64_t FileSize;
    ret = storage_download_file_to_file1(
        TrackerServer, 
        nullptr, 
        argv[1], "./Buffer.bak", 
        &FileSize);
    if(ret != 0){
        std::cout<<"下载文件失败:"<<STRERROR(ret)<<std::endl;
        return -1;
    }
    //释放资源（关闭tracker服务器连接句柄， 销毁客户端全局配置）
    tracker_close_connection(TrackerServer);
    fdfs_client_destroy();
    return 0;
}
