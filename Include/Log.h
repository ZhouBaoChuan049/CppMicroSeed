#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace MicroSeedLog
{
    struct SetInfo
    {
        bool async = false ; //异步启动
        int level = 1 ;  //debug , info , warn , error , critical , off (1~6)
        std::string format = "[%H:%M:%S][%-8l]:%v";
        std::string path = "stdout" ;
    };

    extern std::shared_ptr<spdlog::logger> G_logger ;
    extern void MicroSeedLogInit(const SetInfo& settinginfo = SetInfo());

    #define FORMATE std::string("[{}:{}]:")
    #define DBG(fmt, ...) MicroSeedLog::G_logger->debug(FORMATE + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
    #define INF(fmt, ...) MicroSeedLog::G_logger->info(FORMATE + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
    #define WRN(fmt, ...) MicroSeedLog::G_logger->warn(FORMATE + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
    #define ERR(fmt, ...) MicroSeedLog::G_logger->error(FORMATE + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
    #define CTL(fmt, ...) MicroSeedLog::G_logger->critical(FORMATE + fmt, __FILE__, __LINE__, ##__VA_ARGS__)
}