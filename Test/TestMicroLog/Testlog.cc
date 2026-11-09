#include "../../Include/Log.h"
#include <gflags/gflags.h>
#include <iostream>
using namespace MicroSeedLog ;
    // struct SetInfo
    // {
    //     bool async = false ; //异步启动
    //     int level = 1 ;  //debug , info , warn , error , critical , off (1~6)
    //     std::string format = "[%H:%M:%S][%-8l]:%v";
    //     std::string path = "stdout" ;
    // };
DEFINE_bool(_async, false, "是否启动异步日志");
DEFINE_int64(_level, 1, "日志等级");
DEFINE_string(_format, "[%H:%M:%S][%-8l]:%v", "日志格式");
DEFINE_string(_path, "stdout", "日志模式");


int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    SetInfo information ;
    information.async = FLAGS__async;
    information.format = FLAGS__format;
    information.level = FLAGS__level;
    information.path = FLAGS__path ;
    MicroSeedLogInit(information);
    DBG("小明今年{}岁",17);
    INF("小明今年{}岁",17);
    ERR("小明今年{}岁",17);
    WRN("小明今年{}岁",17);
    CTL("小明今年{}岁",17);
    return 0 ;
}