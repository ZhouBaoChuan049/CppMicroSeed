#include <cstddef>
#include <sstream>
#include <string>
#include "../Include/Log.h"
#include "../Include/FastDFS.h"
namespace MicroSeedFastDFS{
    void FastDFSClient::Init(const FastDFSSetting& settings){
        g_log_context.log_level = LOG_ERR;
        log_init();
        std::stringstream ss;
        for(auto trackeraddress : settings.TrackerServers)
            ss << "tracker_server = " << trackeraddress << "\n";
        ss << "connect_timeout = "               << settings.ConnectTimeout            << "\n";
        ss << "network_timeout = "               << settings.NetworkTimeout            << "\n";
        ss << "use_connection_pool = "           << settings.IsUsingConnectionPool     << "\n";
        ss << "connection_pool_max_idle_time = " << settings.ConnectionPoolMaxIdleTime << "\n";
        int ret = fdfs_client_init_from_buffer(ss.str().c_str());
        if(ret != 0) ERR("初始化全局配置失败!失败原因:[{}]",STRERROR(ret));
    }
    void FastDFSClient::Destroy(){ fdfs_client_destroy(); }
    std::optional<std::string> FastDFSClient::UploadFromBuff(const std::string& buff){
        auto TrackerServer = tracker_get_connection();
        if(TrackerServer == nullptr){
            ERR("获取Tracker服务器连接句柄失败!");
            return std::optional<std::string>();
        }
        char FileId[256];
        int ret = storage_upload_by_filebuff1(
            TrackerServer, nullptr, 0, 
            buff.c_str(), 
            buff.size(), nullptr, nullptr,0, nullptr, FileId);
        if(ret != 0){
            ERR("文件上传失败!错误原因:[{}]",STRERROR(ret));
            return std::optional<std::string>();
        }
        INF("文件上传成功,文件id:{}",std::string(FileId));
        tracker_close_connection(TrackerServer);
        return std::string(FileId);
    }
    std::optional<std::string> FastDFSClient::UploadFromFile(const std::string& path){
        auto TrackerServer = tracker_get_connection();
        if(TrackerServer == nullptr){
            ERR("获取Tracker服务器连接句柄失败!");
            return std::optional<std::string>();
        }
        char FileId[256];
        int ret = storage_upload_by_filename1(
            TrackerServer, nullptr, 0, 
            path.c_str(), 
            nullptr, nullptr, 0, nullptr, FileId);
        if(ret != 0){
            ERR("文件上传失败!错误原因:[{}]",STRERROR(ret));
            return std::optional<std::string>();
        }
        INF("文件上传成功,文件id:{}",std::string(FileId));
        tracker_close_connection(TrackerServer);
        return std::string(FileId);
    }
    bool FastDFSClient::DownloadToBuff(const std::string& fileid, std::string& buff) {
        auto TrackerServer = tracker_get_connection();
        if(TrackerServer == nullptr){
            ERR("获取Tracker服务器连接句柄失败!");
            return false;
        }
        int64_t FileSize = 0;
        char* filebuff = nullptr;
        int ret = storage_download_file1(
            TrackerServer, 
            nullptr, 
            fileid.c_str(), &filebuff, 
            &FileSize);
        if(ret != 0){
            ERR("文件下载失败!错误原因:[{}]",STRERROR(ret));
            return false;
        }
        buff.assign(filebuff,FileSize);
        free(filebuff);
        tracker_close_connection(TrackerServer);
        return true;
    }
    bool FastDFSClient::DownloadToFile(const std::string& fileid, const std::string& path){
        auto TrackerServer = tracker_get_connection();
        if(TrackerServer == nullptr){
            ERR("获取Tracker服务器连接句柄失败!");
            return false;
        }
        int64_t FileSize;
        int ret = storage_download_file_to_file1(
            TrackerServer, 
            nullptr, 
            fileid.c_str(), path.c_str(), 
            &FileSize);
        if(ret != 0){
            ERR("文件下载失败!错误原因:[{}]",STRERROR(ret));
            return false;
        }
        tracker_close_connection(TrackerServer);
        return true;
    }
    bool FastDFSClient::Remove(const std::string& fileid){
        auto TrackerServer = tracker_get_connection();
        if(TrackerServer == nullptr){
            ERR("获取Tracker服务器连接句柄失败!");
            return false;
        }
        int ret = storage_delete_file1(TrackerServer, nullptr, fileid.c_str());
        if(ret != 0){
            ERR("文件删除失败!错误原因:[{}]",STRERROR(ret));
            return false;
        }
        tracker_close_connection(TrackerServer);
        return true;
    }
}
