#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include "../../Include/Util.h"
#include "../../Include/Log.h"
#include "../../Include/FastDFS.h"
using namespace MicroSeedFastDFS;
using namespace MicroSeedUtil;
using namespace MicroSeedLog;
//#define __SIMPLE_TEST__
#define __MULTITHREAD_TEST__
void TestFileUpAndDown(){
    std::optional<std::string> fileid 
        = FastDFSClient::UploadFromFile("./Test.txt");
    bool ret = FastDFSClient::DownloadToFile(fileid.value(), "Download_file.txt");
    if(ret == false){
        ERR("TestFileUpAndDown()下载文件失败!");
        abort();
    }
    INF("TestFileUpAndDown()下载文件成功");
}
void TestBufferUpAndDown(){
    std::string UploadBuff;
    FileUtil util;
    util.ReadFile("./Test.txt",UploadBuff);
    std::optional<std::string> fileid 
        = FastDFSClient::UploadFromBuff(UploadBuff);
    std::string DownloadBuff;
    bool ret = FastDFSClient::DownloadToBuff(fileid.value(), DownloadBuff);
    if(ret == false){
        ERR("TestBufferUpAndDown()下载文件失败!");
        abort();
    }
    util.WriteFile("./Download_buffer.txt", DownloadBuff);
    INF("TestBufferUpAndDown()下载文件成功");
}
void TestRemove(){
    std::optional<std::string> fileid 
        = FastDFSClient::UploadFromFile("./Test.txt");
    bool ret = FastDFSClient::Remove(fileid.value());
    if(ret == false){
        ERR("TestRemove()删除文件失败!");
        abort();
    }
    INF("TestRemove()删除文件成功");
}

void MultithreadedConcurrentAccess(
    std::vector<std::string>& ids,
    std::mutex& mtx){
        std::vector<std::thread> Threads ;
        for(int i = 0 ; i < 5 ; i ++){
            Threads.emplace_back(std::thread([&](){
                for(int i = 0 ; i < 5 ; i ++){
                        std::optional<std::string> fileid 
                            = FastDFSClient::UploadFromFile("./Test.txt");
                        //我们设计的是线程安全的,因为默认每一次文件的上传都是创建一个新的Tracker连接句柄
                        //所以这句话不需要加锁
                        {
                            std::lock_guard<std::mutex> lock(mtx);
                            ids.push_back(fileid.value());
                            //但是这个临界资源需要加锁处理
                        }
                }
            }));
        }
    for(int i = 0 ; i < Threads.size() ; i ++)
        Threads[i].join();
}
void RemoveForMultiThreadTest(std::vector<std::string> ids){
    for(int i = 0 ; i < ids.size() ; i ++){
        bool ret = FastDFSClient::Remove(ids[i]);
        if(ret == false){
            ERR("TestRemove()删除文件{}失败!",ids[i]);
            abort();
        }
        INF("TestRemove()删除文件{}成功",ids[i]);
    }
}
#ifdef __SIMPLE_TEST__
int main(){
    MicroSeedLogInit();
    FastDFSSetting setting;
    setting.TrackerServers.push_back("dev-tracker:22122");
    FastDFSClient::Init(setting);
    //基于文件的上传下载操作.
    TestFileUpAndDown();
    //基于缓冲区的上传下载操作.
    TestBufferUpAndDown();
    //文件删除操作.
    TestRemove();
    FastDFSClient::Destroy();
    return 0;
}
#endif

#ifdef __MULTITHREAD_TEST__
int main(){
    MicroSeedLogInit();
    FastDFSSetting setting;
    setting.TrackerServers.push_back("dev-tracker:22122");
    FastDFSClient::Init(setting);
    std::vector<std::string> ids;
    std::mutex mtx;
    std::cout<<"================"<<std::endl;
    MultithreadedConcurrentAccess(ids,mtx);
    std::cout<<"================"<<std::endl;
    RemoveForMultiThreadTest(ids);
    FastDFSClient::Destroy();
    return 0;
}
#endif

