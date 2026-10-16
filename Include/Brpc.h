#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/channel.h>
#include <brpc/controller.h>
#include <google/protobuf/stubs/callback.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
namespace MicroSeedBrpc
{
    using ChannelPtr = std::shared_ptr<brpc::Channel> ; 
    class Channels{
        public:
            Channels()
                :_Index(0)
            {}
            ~Channels(){}
            void InsertChannel(std::string address);
            void DeleteChannel(std::string address);
            ChannelPtr SelectChannel();
            std::string SelectAddress();
        private:
            std::mutex _Mutex ;
            std::string _ServiceName ;
            int _Index ; // 轮转下标
            std::vector<std::pair<std::string, ChannelPtr>> _Channels ; //高速轮转
            std::unordered_map<std::string, ChannelPtr> _ChannelMap ;//高速增删
            //服务与信道之间的映射关系
    };

    class ChannelManager{
        public:
            ChannelManager() = default ;
            ~ChannelManager(){}
            void SetWhatServiceNeedTobePayAttention(std::string& servername);//这里也许可以设计成数组？
            //Insert
            void InsertChannel(std::string address, std::string servername);
            //Delete
            void DeleteChannel(std::string address, std::string servername);
            ChannelPtr SelectNode(std::string& servername);
            std::string SelectAddress(std::string& servername);
            std::shared_ptr<Channels> GetChannel(const std::string& servername);
        private:
            std::mutex _Mutex ;
            std::unordered_map<std::string, std::shared_ptr<Channels>> _Map ;
            //我们说，相同的一个服务可能维护在多个Brpc服务器里面，
            //所以服务与信道的关系是1:N.
    };
    class ClosureFactory{
        ClosureFactory() = default ;
        ~ClosureFactory(){}
        using CallBack = std::function<void()>;
        public:
            google::protobuf::Closure* Create(CallBack& cbk);
            //Closure在外面和lambda不是很适配，需要包装一遍.
        private:
            struct Object{
                using ObjPtr = std::shared_ptr<Object>;
                CallBack _callback ; 
            };
            static void InternalCallBack(Object::ObjPtr objectpointer){
                objectpointer->_callback() ;
            }
    };
    class RpcServerFactory{
        RpcServerFactory() = default ;
        ~RpcServerFactory(){} 
        public:
            std::shared_ptr<brpc::Server> Create(
                int Port, 
                google::protobuf::Service* service
            );
    };
}