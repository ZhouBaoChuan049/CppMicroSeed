#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>

const std::string url = "http://172.20.0.10:2379";

void CallBack(const etcd::Response& Resp){
    if(Resp.is_ok() == false){
        std::cout <<"监控的时候出现问题!详细原因:"<<Resp.error_message()<<std::endl;
        return;
    }
    const auto& events = Resp.events();
    for(auto& e : events){
        if(e.event_type() == etcd::Event::EventType::PUT){
            std::cout<<e.prev_kv().key() <<"数据发生改变:"<<
                e.prev_kv().as_string()<<"——>"<<e.kv().as_string()<<std::endl;
        }else if(e.event_type() == etcd::Event::EventType::DELETE_){
            std::cout<<e.prev_kv()<<"数据删除!"<<std::endl;
        }else{
            std::cout<<"无效的触发!"<<std::endl;
        }
    }
}

int main(){
    //创建客户端并尝试获取数据
    etcd::Client client(url);
    auto Resp = client.get("Name").get();
    if(Resp.is_ok() == false){
        std::cout<<"获取数据失败!详细原因:"<<Resp.error_message()<<std::endl;
        return -1;
    }
    std::cout<<Resp.value().as_string()<<std::endl;
    //对数据进行监控，发生变化的时候通知机制
    etcd::Watcher watcher(url,"Name",CallBack,false);
    watcher.Wait();
    return 0;
}