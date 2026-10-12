#include "../../Include/Util.h"
#include <jsoncpp/json/json.h>
#include <iostream>
#include <gtest/gtest.h>
#include <jsoncpp/json/value.h>
using namespace MicroSeedUtil ;
            // static std::string Serialize(
            //     const Json::Value& val,bool styles = false
            // );
            // static Json::Value UnSerialize(const std::string& input);
TEST(TEST_MICRO_JSONCPP,Serialize)
{
    Json::Value val;
    val["name"] = "zhangsan";
    val["age"] = 18;
    val["score"].append(77.5);
    val["score"].append(88.5);
    val["score"].append(99.5);
    std::string ret = Serialize(val, false);
    std::cout<<ret<<std::endl;
}

TEST(TEST_MICRO_JSONCPP,UnSerialize)
{
    std::string str = R"({"name":"Konata","age":18})";
    Json::Value val = UnSerialize(str);
    std::cout<< val["name"]<<std::endl;
    std::cout<<val["age"]<<std::endl;
}

int main(int argc , char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS() ;
}




