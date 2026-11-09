#include <gflags/gflags.h>
#include <iostream>
#include "Anotherfile.h"
DEFINE_bool(_isavailable , false , "服务器是否启动成功");
DEFINE_string(ipaddr, "0.0.0.0", "服务器IP地址");
DEFINE_int64(AIntVal, 32, "一个整数数值");
DEFINE_double(pai, 3.1415564, "圆周率数值");

int main(int argc,char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    // std::cout<<FLAGS__isavailable<<std::endl;
    // std::cout<<FLAGS_AIntVal<<std::endl;
    // std::cout<<FLAGS_ipaddr<<std::endl;
    // std::cout<<FLAGS_pai<<std::endl;
    func();
    return 0 ;
}

