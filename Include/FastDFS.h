#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include <optional>
extern "C" {
    #include <fastdfs/fdfs_client.h>
    #include <fastcommon/logger.h>
}
namespace MicroSeedFastDFS{
    struct FastDFSSetting{
        std::vector<std::string> TrackerServers;
        //我们可以定义多个Tracker服务的地址
        int ConnectTimeout = 30;
        int NetworkTimeout = 60;
        bool IsUsingConnectionPool = true;
        int ConnectionPoolMaxIdleTime = 3600;
        //连接池中空闲连接的最长存活时间
    };
    class FastDFSClient{
    public:
        static void Init(const FastDFSSetting& settings);
        static void Destroy();
        static std::optional<std::string> UploadFromBuff(const std::string& buff);
        static std::optional<std::string> UploadFromFile(const std::string& path);
        static bool DownloadToBuff(const std::string& fileid, std::string& buff);
        static bool DownloadToFile(const std::string& fileid, const std::string& path);
        static bool Remove(const std::string& fileid);
    };
}
