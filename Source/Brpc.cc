#include "../Include/Brpc.h"
#include <brpc/callback.h>
#include <brpc/channel.h>
#include <brpc/server.h>
#include <memory>
#include <mutex>
#include "../Include/Log.h"
namespace MicroSeedBrpc
{
    //Channels Functions===========================================

    void Channels::InsertChannel(std::string address)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        auto it = _ChannelMap.find(address);
        if(it != _ChannelMap.end()){
            ERR("信道已存在,不需要新增!");
            return ;
        }
        auto channel = std::make_shared<brpc::Channel>();
        brpc::ChannelOptions options ;
        options.timeout_ms = 30000 ;
        options.protocol = "baidu_std"; 
        channel->Init(address.c_str(), &options);
        _ChannelMap.insert({address,channel});
        _Channels.push_back(std::make_pair(address, channel));
    }

    void Channels::DeleteChannel(std::string address)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        auto it = _ChannelMap.find(address);
        if(it == _ChannelMap.end()){
            std::cout<<"信道不存在,无法删除!"<<std::endl;
            return ;
        }
        _ChannelMap.erase(it);
        for(auto vit = _Channels.begin(); vit != _Channels.end(); vit++){
            if(address == vit->first){
                _Channels.erase(vit);
                break ;
            }
        }
    }
    ChannelPtr Channels::SelectChannel()
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        if(_Channels.size() == 0)
            return ChannelPtr();
        int RetIndex = (_Index++)%_Channels.size();
        return _Channels[RetIndex].second ;
    }
    std::string Channels::SelectAddress()
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        if(_Channels.size() == 0)
            return "";
        int RetIndex = (_Index++)%_Channels.size();
        return _Channels[RetIndex].first;
    }
    //Channels Functions===========================================

    //ChannelManager Functions=====================================
    std::shared_ptr<Channels> ChannelManager::GetChannel(const std::string& servername){
        auto it = _Map.find(servername);
        if(it == _Map.end()){
            return std::make_shared<Channels>();
        }
        return it->second ;
    }
    void ChannelManager::SetWhatServiceNeedTobePayAttention(std::string& servername)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        auto channel = std::make_shared<Channels>();
        _Map.insert(std::make_pair(servername, channel));
    }

    void ChannelManager::InsertChannel(std::string address, std::string servername)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        std::shared_ptr<Channels> channels = GetChannel(servername);
        if(!channels){
            ERR("{}有结点{}上线!,未找到管理对象!",servername,address);
            return;
        } 
        channels->InsertChannel(address);
        DBG("添加{}服务结点{}管理",servername, address);
        return;
    }

    void ChannelManager::DeleteChannel(std::string address, std::string servername)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        std::shared_ptr<Channels> channel = GetChannel(servername);
        if(!channel){
            ERR("{}有结点{}下线!,未找到管理对象!",servername,address);
            return;
        }
        channel->DeleteChannel(address);
        DBG("移除{}服务结点{}管理",servername, address);
        return;
    }
    ChannelPtr ChannelManager::SelectNode(std::string& servername)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        std::shared_ptr<Channels> channels = GetChannel(servername);
        if(!channels)
            return ChannelPtr();
        return channels->SelectChannel();
    }
    std::string ChannelManager::SelectAddress(std::string& servername)
    {
        std::unique_lock<std::mutex> Lock(_Mutex);
        std::shared_ptr<Channels> channels = GetChannel(servername);
        if(!channels)
            return "";
        return channels->SelectAddress();
    }
    //ChannelManager Functions=====================================

    //ClosureFactory Functions=====================================

    google::protobuf::Closure* ClosureFactory::Create(CallBack& cbk)
    {
        auto obj = std::make_shared<Object>();
        obj->_callback = cbk ;
        return brpc::NewCallback(ClosureFactory::InternalCallBack, obj);
    }

    //ClosureFactory Functions=====================================

    //RpcServerFactory Functions===================================
    std::shared_ptr<brpc::Server> RpcServerFactory::Create(
        int Port, 
        google::protobuf::Service* service
    ){
        brpc::ServerOptions options ;
        options.idle_timeout_sec = -1 ;
        auto server = std::make_shared<brpc::Server>();
        int ret = server->AddService(service, brpc::SERVER_OWNS_SERVICE);
        if(ret == -1){
            ERR("添加RPC服务器失败!");
            return nullptr ;
        }
        ret = server->Start(Port, &options);
        if(ret == -1){
            ERR("启动服务器失败!");
            return nullptr ;
        }
        return server ;
    }
    //RpcServerFactory Functions===================================
}