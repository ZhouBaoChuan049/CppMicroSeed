#pragma once
#include <cstdlib>
#include <string>
#include <jsoncpp/json/json.h>
#include <random>
#include <vector>
#include <iomanip>
#include <atomic>
namespace MicroSeedUtil{
    class JsonUtil{
    public:
        std::string Serialize(
            const Json::Value& val,bool styles = false
        );//styles是否采用格式化输出
        Json::Value UnSerialize(const std::string& input);
    };
    class FileUtil{
    public:
        bool ReadFile(std::string filename, std::string& buffer) ;
        bool WriteFile(std::string filename, const std::string& buffer);
    };
    class StringUtil{
    public:
        bool SplitString(
            std::string& sourcestring,
            const std::string& sep,
            std::vector<std::string>& deststrings
        );
    };
    enum class CodeType{
        STRING = 0,
        INT = 1 ,
        MIX = 2
    };
    class RandomUtil{
    public:
        std::string RandomCode(size_t length, const CodeType type);
    };
}