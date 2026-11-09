#pragma once
#include "Log.h"
#include "Util.h"
#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

namespace MicroSeedEtcd{
    using callback = std::function<void(std::string ServiceName , std::string ServiceAddress)>;
    //灵感来自官方样例的一个连接请求等待接口
    //没连上就睡觉，循环的睡觉std::this_thread::sleep_for(std::chrono::seconds(1));
    extern void WaitingConnection(etcd::Client& client);
    class EtcdServiceProvider{
    public:
        using EtcdServiceProviderPtr = std::shared_ptr<EtcdServiceProvider>;
        EtcdServiceProvider(
            std::string center,
            std::string sign,
            std::string service,
            std::string address
        );
        bool Register();
    private:
        //制作Key -> / 服务名称 / 
        std::string MakeKey(std::string servicename);
    private:
        std::string _ServiceRegisterCenter ;
        std::string _CurrentPositionSign ;
        std::string _ServiceofThisPosition ;
        std::string _AddressofThisPosition ;
        std::shared_ptr<etcd::KeepAlive> _keep ;
    };
    class EtcdServiceWatcher{
    public:
        EtcdServiceWatcher(
            std::string center,
            callback online,
            callback offline
        );
        using EtcdServiceWatcherPtr = std::shared_ptr<EtcdServiceWatcher>;
        bool DataWatch();
    private:
        void Callback(etcd::Response response);   
        std::string GetServiceNameFromKey(std::string key);
    private:
        std::string _ServiceRegisterCenter ;
        callback ServerOnline;
        callback ServerOffline;
        std::shared_ptr<etcd::Watcher> _watch ;
    };
}