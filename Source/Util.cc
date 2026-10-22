#include "../Include/Util.h"
#include "../Include/Log.h"
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <memory>
#include <fstream>
using namespace MicroSeedLog ;
namespace MicroSeedUtil
{
    //JSONUTIL
    std::string JsonUtil::Serialize(
        const Json::Value& val,bool styles){
            Json::StreamWriterBuilder builder ;
            if(styles == false)
                builder["indentation"] = "" ;
            std::unique_ptr<Json::StreamWriter> swp(builder.newStreamWriter());
            std::stringstream ss ;
            int ret = swp->write(val, &ss);
            if(ret != 0){
                ERR("序列化失败!");
                return "";
            }
            return ss.str();
    }
    Json::Value JsonUtil::UnSerialize(const std::string& input){
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
    //FILEUTIL
    bool FileUtil::ReadFile(std::string filename, std::string& buffer) 
    {
        std::ifstream ifs ;
        ifs.open(filename, std::ios::in | std::ios::binary);
        if(ifs.is_open() == false){
            ERR("打开文件{}失败!",filename);
            return false;
        }
        ifs.seekg(0, std::ios::end); // 这是让读写位置到达文件末尾
        size_t filelen = ifs.tellg(); // 这是记录文件读写位置到开头的距离
        ifs.seekg(0, std::ios::beg); //这是让读写位置回到文件的开头
        buffer.resize(filelen);
        ifs.read(&buffer[0], filelen);
        if(ifs.good() == false){
            ERR("读取文件{}数据失败!",filename);
            ifs.close();
            return false ;
        }
        ifs.close();
        return true ;
    }
    bool FileUtil::WriteFile(std::string filename, const std::string& buffer)
    {
        std::ofstream ofs ;
        ofs.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if(ofs.is_open() == false){
            ERR("打开文件{}失败!",filename);
            return false;
        }
        ofs.write(buffer.c_str(), buffer.size());
        if(ofs.good() == false){
            ERR("写入文件{}数据失败",filename);
            ofs.close();
            return false;
        }
        ofs.close();
        return true ;
    }

    //STRINGUTIL
    bool StringUtil::SplitString(std::string& sourcestring,
        const std::string& sep,std::vector<std::string>& deststrings){
            if(sourcestring == "" || sep == "") return false ;
            size_t Pos = 0 , Cur = 0 ;
            while(true){
                Pos = sourcestring.find(sep, Cur);
                if(Pos == std::string::npos) break;
                if(Pos == Cur){ Cur += sep.size(); continue; }
                std::string substring = sourcestring.substr(Cur,(Pos - Cur));
                deststrings.push_back(substring);
                Cur = Pos + sep.size() ;
            }
            if(Cur < sourcestring.size())
                deststrings.push_back(sourcestring.substr(Cur));
            return true ;
    }


}