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
    //////////////////////////公共基类////////////////////////////////////
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
    ///////////////////////////////////////////////////////////////////////
    ////////////////////////创建索引/////////////////////////////////////
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
    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////查询:单条件检索////////////////////////////////////////////////
    //单个字段的精确匹配
    class Term : public Object{
    public :
        using TermPtr = std::shared_ptr<Term> ;
        Term(std::string field);
        template<typename T>
        void SetVal(const T val){
            this->Add(_field, val);
        }
    private:
        std::string _field ;
    };
    //多个字段的精确匹配
    class Terms : public Object{
    public :
        using TermsPtr = std::shared_ptr<Terms> ;
        Terms(std::string field);
        template<typename T>
        void SetVal(const T val){
            this->Append(_field, val);
        }
    private:
        std::string _field ;
    };
    //单个字段的模糊匹配
    class Match : public Object{
    public :
        using MatchPtr = std::shared_ptr<Match> ;
        Match(std::string field);
        template<typename T>
        void SetVal(const T val){
            this->Add(_field, val);
        }
    private:
        std::string _field ;
    };
    //多个字段的模糊匹配
    class MultiMatch : public Object{
    public :
        using MultiMatchPtr = std::shared_ptr<MultiMatch> ;
        MultiMatch();
        void AppendField(std::string val);
        template<typename T>
        void SetQuery(const T val){
            this->Add("query", val);//这里的val可以是数组的类型
        }
    };
    //数字区间的匹配
    class Range : public Object{
    public :
        using RangePtr = std::shared_ptr<Range> ;
        Range(std::string field);
        void Field(int gte, int lte);
    private:
        std::string _field ;
    };
    class QueryObject: public Object{
    public:
        QueryObject(std::string key);
        Range::RangePtr range(std::string field);
        MultiMatch::MultiMatchPtr multimatch();
        Match::MatchPtr match(std::string field);
        Terms::TermsPtr terms(std::string field);
        Term::TermPtr term(std::string field);
    };
}