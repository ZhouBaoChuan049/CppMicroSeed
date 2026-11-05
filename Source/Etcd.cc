#include "../Include/Etcd.h"
#include <butil/logging.h>
#include <cstddef>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <memory>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
namespace MicroSeedEtcd{
    void WaitingConnection(etcd::Client& client){
        while(client.head().get().is_ok() == false){
           WRN("暂时未连接上Etcd服务器");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    EtcdServiceProvider::EtcdServiceProvider(
        std::string center,
        std::string sign,
        std::string service,
        std::string address
    ):_ServiceRegisterCenter(center),
        _CurrentPositionSign(sign),
        _ServiceofThisPosition(service),
        _AddressofThisPosition(address){
        }
    bool EtcdServiceProvider::Register(){
        //std::cout<<"#####段错误在这里？#####"<<std::endl;
        etcd::Client client(_ServiceRegisterCenter);
        WaitingConnection(client);
        auto response = client.leasegrant(3).get();
        if(response.is_ok() == false){
            //std::cout<<"#####段错误在这里？-J#####"<<std::endl;
            ERR("获取租约失败!{}",response.error_message());
            //std::cout<<"#####段错误在这里？-R#####"<<std::endl;
            return false;
        }
        //std::cout<<"#####段错误在这里？#####"<<std::endl;
        auto leaseid = response.value().lease();
        std::string key = MakeKey(_ServiceofThisPosition);
        auto ret = client.put(key,_AddressofThisPosition,leaseid).get();
        if(ret.is_ok() == false){
            ERR("添加数据失败!{}",ret.error_message());
            return false;
        }    
        auto handler = [this](const std::exception_ptr &eptr){
            //这里会引发死锁问题:这个问题非常非常隐蔽!就是说:
            //_keep.reset()实际上做了两步操作而不是简单的说就是让_keep指针指向一个新的KeepAlive对象,不是的.
            // 1. 先创建新对象
            // 2. 销毁旧对象,调用 ~KeepAlive()里面 join 续租线程
            // 3. 让 _keep 指向新对象
            //_keep.reset()会销毁_keep指向的原来的那个keepAlive,但是KeepAlive内部有一个续租线程,
            //这个续租线程还是原来那个续租线程,在销毁这个续租线程的时候,就会调用Join,去等到当前这个线程,
            //于是就会出现自己等待自己的死锁问题.
            auto leasethread = std::thread([this](){ this->Register(); });
            leasethread.detach();
            // this->Register();  
        };
        _keep.reset(new etcd::KeepAlive(_ServiceRegisterCenter,handler,3,leaseid));
        return true;
    }
    //制作Key -> / 服务名称 / 
    std::string EtcdServiceProvider::MakeKey(std::string servicename){
        std::stringstream ss ;
        ss << "/" << servicename <<"/"<<_CurrentPositionSign ;
        return ss.str();
    }
    EtcdServiceWatcher::EtcdServiceWatcher(
        std::string center,
        callback online,
        callback offline
    ):_ServiceRegisterCenter(center),
        ServerOnline(online),
        ServerOffline(offline){
        }
    bool EtcdServiceWatcher::DataWatch(){
        etcd::Client client(_ServiceRegisterCenter);
        WaitingConnection(client);
        auto response = client.ls("/").get();
        if(response.is_ok() == true){
            auto arreys = response.values();
            for(auto& num : arreys){
                std::string ServiceName = GetServiceNameFromKey(num.key());
                std::string ServiceAddress = num.as_string();
                std::cout<<"Key:"<<ServiceName<<
                    "Value:"<<ServiceAddress<<std::endl;
                if(ServerOnline) ServerOnline(ServiceName,ServiceAddress);
            }
        }
        auto cb = std::bind(&EtcdServiceWatcher::Callback, this,std::placeholders::_1);
        _watch.reset(new etcd::Watcher(_ServiceRegisterCenter,"/",cb,true));
        _watch->Wait([this](bool cond){
            if(cond == true){
                DBG("Watcher以正常的方式退出了");
                return;
            }
            this->DataWatch();
        });
        return true;
    }
    void EtcdServiceWatcher::Callback(etcd::Response response){
        if(response.is_ok() == false){
            std::cout <<"监控的时候出现问题!\
                详细原因:"<<response.error_message()<<std::endl;
            return ;
        }
        auto Eve = response.events();
        for(auto eve : Eve){
            if(eve.event_type() == etcd::Event::EventType::PUT){
                std::string ServiceName = GetServiceNameFromKey(eve.kv().key());
                std::string ServiceAddress = eve.kv().as_string();
                if(ServerOnline) ServerOnline(ServiceName,ServiceAddress);
            }else if(eve.event_type() == etcd::Event::EventType::DELETE_){
                std::string ServiceName = GetServiceNameFromKey(eve.kv().key());
                std::string ServiceAddress = eve.prev_kv().as_string();
                if(ServerOffline) ServerOffline(ServiceName,ServiceAddress);
            }else{
                WRN("无效的触发!");
            }
        }
    }
    std::string EtcdServiceWatcher::GetServiceNameFromKey(std::string key){
        MicroSeedUtil::StringUtil util ;
        std::vector<std::string> deststrings ;
        bool ret = util.SplitString(key, "/", deststrings);
        if(ret == false){
            ERR("解析服务名称失败");
            return key; 
        }
        return deststrings[0];
    }
}