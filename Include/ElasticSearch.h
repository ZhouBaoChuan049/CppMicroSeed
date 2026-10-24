#include <cpr/response.h>
#include <elasticlient/client.h>
#include <iostream>
#include <jsoncpp/json/value.h>
#include <thread>
#include <string>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <memory>
#include "Log.h"
#include "Util.h"
namespace MicroSeedElastic
{
    class Base{
    public:
        using BasePtr = std::shared_ptr<Base> ;
        Base() = default ;
        Base(std::string key);
        virtual std::string ToString();
        std::string GetKey();
        virtual Json::Value GetVal();
        template<typename T>
        Base& Add(const std::string key, const T val){
            _Val[key] = val;
            return *this;
        }
        template<typename T>
        Base& Append(const std::string key, const T val){
            _Val[key].append(val);
            return *this;
        }
    protected:
        std::string _Key ;
        Json::Value _Val ;
    };  
    class Object : public Base{
    public:
        using ObjectPtr = std::shared_ptr<Object> ;
        Object(std::string val);
        void AddElememt(const BasePtr& baseptr);
        template<typename R>
        std::shared_ptr<R> GetElememt(std::string str){
            auto i = _Objects.find(str);
            if(i ==_Objects.end()){
                return nullptr;
            }
            return std::dynamic_pointer_cast<R>(_Objects[str]);
        }
        Json::Value GetVal() override;
        std::string ToString() override;
    private:
        std::unordered_map<std::string, BasePtr> _Objects ;
    };
    class Tokenizer : public Object{
    public:
        using TokenizerPtr = std::shared_ptr<Tokenizer> ;
        Tokenizer(std::string str = "ikmax");
        void type(std::string val);
        void tokenzier(std::string val);
    };
    class Analyzer : public Object{
    public:
        using AnalyzerPtr = std::shared_ptr<Analyzer> ;
        Analyzer();
        Tokenizer::TokenizerPtr tokenizer(std::string name);
    };
    class Analysis : public Object{
    public:
        using AnalysisPtr = std::shared_ptr<Analysis> ;
        Analysis();
        Analyzer::AnalyzerPtr analyzer();
    };
    class Setting : public Object{
    public:
        using SettingPtr = std::shared_ptr<Setting> ;
        Setting();
        Analysis::AnalysisPtr analysis();
    };
    class Fields : public Object{
    public:
        using FieldsPtr = std::shared_ptr<Fields> ;
        Fields(std::string fieldname);
        void Type(std::string typrname);
        void Boost(double boostnum);
        void Analyzer(std::string analyzername);
        void Index(bool indexnum);
    };
    class Properties : public Object{
    public:
        using PropertiesPtr = std::shared_ptr<Properties> ;
        Properties();
        Fields::FieldsPtr field(std::string fieldname);
    };
    class Mappings : public Object{
    public:
        using MappingsPtr = std::shared_ptr<Mappings> ;
        Mappings();
        void Dynamic(bool dynamicnum);
        Properties::PropertiesPtr properties();  
    };
    class Indexer :public Object{
    public:
        Indexer();
        Setting::SettingPtr setting();
        Tokenizer::TokenizerPtr BuildSettingChain();
        Properties::PropertiesPtr  BuildMappingChain();
        Mappings::MappingsPtr mapping();
    };
}