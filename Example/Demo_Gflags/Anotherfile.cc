#include "Anotherfile.h"
#include <gflags/gflags_declare.h>
#include <iostream>
DECLARE_bool(_isavailable);
DECLARE_int64(AIntVal);
DECLARE_string(ipaddr);
DECLARE_double(pai);

void func()
{
    std::cout<<FLAGS__isavailable<<std::endl;
    std::cout<<FLAGS_AIntVal<<std::endl;
    std::cout<<FLAGS_ipaddr<<std::endl;
    std::cout<<FLAGS_pai<<std::endl;
}