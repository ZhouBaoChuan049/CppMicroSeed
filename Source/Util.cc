#include "../Include/Util.h"
#include "../Include/Log.h"
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <memory>
using namespace MicroSeedLog ;
namespace MicroSeedUtil
{
    std::string Serialize(
        const Json::Value& val,bool styles){
            Json::StreamWriterBuilder builder ;
            if(styles == false)
                builder["indication"] = "" ;
            std::unique_ptr<Json::StreamWriter> swp(builder.newStreamWriter());
            std::stringstream ss ;
            int ret = swp->write(val, &ss);
            if(ret != 0){
                ERR("序列化失败!");
                return "";
            }
            return ss.str();
    }
    Json::Value UnSerialize(const std::string& input){
        Json::CharReaderBuilder builder ;
        std::unique_ptr<Json:: CharReader> reader(builder.newCharReader());
        Json::Value val ;
        std::string errs ;
        bool ret = reader->parse(input.c_str(), input.c_str()+input.size(), &val, &errs);
        if(ret == false){
            ERR("反序列化失败!");
            return Json::Value();
        }
        return val ;
    }
}