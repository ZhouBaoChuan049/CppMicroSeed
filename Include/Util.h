#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include <random>
#include <iomanip>
#include <atomic>
namespace MicroSeedUtil
{
    std::string Serialize(
        const Json::Value& val,bool styles = false
    );//styles是否采用格式化输出
    Json::Value UnSerialize(const std::string& input);
}