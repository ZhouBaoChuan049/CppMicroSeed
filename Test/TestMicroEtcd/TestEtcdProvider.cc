#include "../../Include/Etcd.h"
using namespace MicroSeedEtcd;
using namespace MicroSeedLog;
const std::string CenterAddress = "http://dev-etcd:2379";
const std::string ServerName = "Name";
const std::string ServerAddress = "http://127.0.0.1:8080";

int main(){
    MicroSeedLogInit();
    EtcdServiceProvider provider(
        CenterAddress,
        "0001",
        ServerName,
        ServerAddress
    ) ;
    provider.Register();
    std::cout<<"请输入回车以退出,于是租约失效!"<<std::endl;
    getchar();
    return 0;
}
