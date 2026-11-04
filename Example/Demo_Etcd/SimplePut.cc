#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <exception>
#include <functional>
#include <stdexcept>
const std::string url = "http://dev-etcd::2379";

int main(){
    //创建客户端对象
    etcd::Client client(url) ;
    //创建租约
    auto LeaseResp = client.leasegrant(3).get();
    if(LeaseResp.is_ok() == false){
        std::cout<<"创建租约失败!失败详细信息:"<<LeaseResp.error_message()<<std::endl;
        return -1;
    }
    //获取一个租约的ID
    auto LeaseId = LeaseResp.value().lease();
    //添加数据并设置租约
    auto Resp = client.put("Name","Mr.Ming",LeaseId).get();
    if(Resp.is_ok() == false){
        std::cout<<"添加数据并设置租约失败!失败详细信息:"<<Resp.error_message()<<std::endl;
        return -1 ;
    }
    auto Hander = [](const std::exception_ptr& eptr){
        try{
            if(eptr){
                std::rethrow_exception(eptr);
            }
        }catch(const std::runtime_error& e){
            std::cerr<<"连接失败!"<<e.what()<<std::endl;
        }catch(const std::out_of_range& e){
            std::cerr<<"租约异常"<<e.what()<<std::endl;
        }
    };
    //租约的保活
    etcd::KeepAlive alive(url,Hander, 3 , LeaseId);
    std::cout<<"请输入回车以退出,于是租约失效!"<<std::endl;
    getchar();
    return 0;
}

