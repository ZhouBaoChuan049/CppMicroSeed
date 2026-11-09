#include "../Include/Log.h"
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace MicroSeedLog
{
    std::shared_ptr<spdlog::logger> G_logger ;
    void MicroSeedLogInit(const SetInfo& settinginfo)
    {
        if(settinginfo.async == true){
            if(settinginfo.path == "stdout"){
                G_logger = spdlog::stdout_color_mt<spdlog::async_factory>("AsyncStdoutLogger");
            }else{
                G_logger = spdlog::rotating_logger_mt<spdlog::async_factory>(
                    "AsyncRotatingFileLogger",
                     settinginfo.path,
                      1024,
                       3
                    );
            }
        }else{
            if(settinginfo.path == "stdout"){
                G_logger = spdlog::stdout_color_mt("StdoutLogger");
            }else {
                G_logger = spdlog::rotating_logger_mt(
                    "RotatingFileLogger",
                    settinginfo.path,
                    1024,
                    3
                    );
            }
        }
        G_logger->set_level(spdlog::level::level_enum(settinginfo.level));
        G_logger->set_pattern(settinginfo.format);
    }
}