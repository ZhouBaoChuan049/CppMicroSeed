#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
const std::string url = "http://dev-etcd:2379";
const std::string Key = "/";

void Callback(etcd::Response response){
    if(response.is_ok() == false){
        std::cout <<"监控的时候出现问题!详细原因:"<<response.error_message()<<std::endl;
        return ;
    }
    auto Eve = response.events();
    for(auto eve : Eve){
        if(eve.event_type() == etcd::Event::EventType::PUT){
            std::cout<<eve.prev_kv().key() <<"数据发生改变:"<<
                eve.prev_kv().as_string()<<"——>"<<eve.kv().as_string()<<std::endl;
        }else if(eve.event_type() == etcd::Event::EventType::DELETE_){
            std::cout<<eve.prev_kv()<<"数据删除!"<<std::endl;
        }else{
            std::cout<<"无效的触发!"<<std::endl;
        }
    }
}


void Get(std::shared_ptr<etcd::Watcher> watch , std::string url , std::string key){
    etcd::Client client(url);
    auto response = client.ls("/Name").get();
    if(response.is_ok() == true){
        //std::cout<<"##############1"<<std::endl;
        auto arreys = response.values();
        for(auto num : arreys){
            //std::cout<<"##############2"<<std::endl;
            std::cout<<"Key:"<<num.key()<<
                "Value:"<<num.as_string()<<std::endl;
        }
    }else{
        std::cout<<"获取数据失败,失败原因:"<<response.error_message()<<std::endl;
        return ;
    }
    watch.reset(new etcd::Watcher(client, key,Callback,true));//true表示递归监控目录下所有数据
    watch->Wait([&](bool cond){
        if(cond == true){
            return;//如果Watch被主动停止，那么就会传入true;
        }
        Get(watch , url,Key);
    });
}
int main(){
    std::shared_ptr<etcd::Watcher> watch; 
    Get(watch , url,Key);
    std::cout<<"请输入回车以退出,于是租约失效!"<<std::endl;
    getchar();
    return 0 ; 
}