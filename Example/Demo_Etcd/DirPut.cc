#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

const std::string url = "http://172.20.0.10:2379";
const std::string Key = "/Name/1324";
const std::string Value = "Mr.Ming";

void Put(std::shared_ptr<etcd::KeepAlive> keep ,const std::string url , std::string Key , std::string Val){
    etcd::Client client(url);
    auto resp = client.leasegrant(3).get();//获取租约
    if(resp.is_ok() == false){
        std::cout<<"创建租约失败!失败原因:"<<resp.error_message()<<std::endl;
        return ;
    }//获取租约ID
    auto leaseid = resp.value().lease();
    //放数据
    //std::cout<<Key<<Val<<leaseid<<std::endl;
    auto ret = client.put(Key,Val).get();
    if(ret.is_ok() == false){
        std::cout<<"添加数据失败,失败原因:"<<ret.error_message()<<std::endl;
        return ;
    }
    auto handler = [&](const std::exception_ptr& eptr){
        Put(keep, url, Key, Value);
    };
    //传入一个保活对象指针的作用是为了让保活对象不再依赖函数栈帧的生命周期。
    //如果我们的数据在etcd服务器上到达了保活时间限制，那么就会触发handler回调
    //然后再去放入一个新的数据，继续传入保活对象的指针，全程只有一个保活对象指针。
    //这个保活对象指针每次reset，本质上是在：
    //1. 干掉指针原来指向的那个对象
    //2. 换一个新的保活对象指向。
    keep.reset(new etcd::KeepAlive(url,handler,3,leaseid));
}

int main(){
    std::shared_ptr<etcd::KeepAlive> keep;
    Put(keep, url, Key, Value);
    std::cout<<"请输入回车以退出,于是租约失效!"<<std::endl;
    getchar();
    return 0 ;
}