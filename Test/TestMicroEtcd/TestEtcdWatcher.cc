#include "../../Include/Etcd.h"
using namespace MicroSeedEtcd;
using namespace MicroSeedLog;
const std::string CenterAddress = "http://dev-etcd:2379";
//void(std::string ServiceName , std::string ServiceAddress)
void OnlineServer(std::string ServiceName , std::string ServiceAddress){
    INF("服务{}已于{}上线",ServiceName,ServiceAddress);
}
void OfflineServer(std::string ServiceName , std::string ServiceAddress){
    INF("服务{}已于{}下线",ServiceName,ServiceAddress);
}
int main(){
    MicroSeedLogInit();
    EtcdServiceWatcher watchclient(
        CenterAddress,
        OnlineServer,
        OfflineServer
    ) ;
    watchclient.DataWatch();
    std::cout<<"请输入回车以退出,于是租约失效!"<<std::endl;
    getchar();
    return 0;
}


