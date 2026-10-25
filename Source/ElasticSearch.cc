#include "../Include/ElasticSearch.h"
#include <cstddef>
#include <jsoncpp/json/value.h>
#include <memory>

namespace MicroSeedElastic
{
    //BASE 
    Base::Base(std::string key)
        :_Key(key){}
    std::string Base::ToString(){
        Json::Value root ;
        root[_Key] = _Val ;
        MicroSeedUtil::JsonUtil util ;
        return util.Serialize(root, true);
    }
    std::string Base::GetKey(){ return _Key;}
    Json::Value Base::GetVal(){ return _Val;}
    //BASE END
    //OBJECT 
    Object::Object(std::string val)
        :Base(val){}
    void Object::AddElememt(const BasePtr& baseptr){
        _Objects[baseptr->GetKey()] = baseptr;
    }
    std::string Object::ToString(){
        Json::Value root ;
        for(auto i : _Objects){
            root[i.first] = i.second->GetVal() ;
        }
        MicroSeedUtil::JsonUtil util ;
        return util.Serialize(root, true);
    }
    Json::Value Object::GetVal(){
        Json::Value root = _Val ;
        for(auto obj : _Objects)
            root[obj.first] = obj.second->GetVal();
        //假如这样调用: std::string ret = analysis->ToString();
        //那么Analysis调用GetVal，在子对象存储器里面调用了Analyzer的GetVal，
        //Analyzer的GetVal又调用了Tokenizer的GetVal(),将Tokenzer的Val向上传递.
        return root ;
    }
    //OBJECT END
    //TOKENIZER 
    Tokenizer::Tokenizer(std::string str)
        :Object(str){
            this->type("custom");
            this->tokenzier("ik_max_word");
    }
    void Tokenizer::type(std::string val){
        this->Add("type", val);
    }
    void Tokenizer::tokenzier(std::string val){
        this->Add("tokenizer", val);
    }
    //TOKENIZER END
    //ANALYZER 
    Analyzer::Analyzer()
        :Object("analyzer"){}
    Tokenizer::TokenizerPtr Analyzer::tokenizer(std::string name){
        Tokenizer::TokenizerPtr ptr; 
        ptr = this->GetElememt<Tokenizer>(name);
        if(ptr != nullptr)
            return ptr;
        auto tokenizer = std::make_shared<Tokenizer>(name);
        this->AddElememt(tokenizer);
        return tokenizer;
    } 
    //ANALYZER END
    //ANALYSIS
    Analysis::Analysis()
        :Object("analysis"){}
    Analyzer::AnalyzerPtr Analysis::analyzer(){
        Analyzer::AnalyzerPtr ptr; 
        ptr = this->GetElememt<Analyzer>("analyzer");
        if(ptr != nullptr)
            return ptr;
        auto analyzer = std::make_shared<Analyzer>();
        this->AddElememt(analyzer);
        return analyzer ;
    }
    //ANALYSIS END
    //SETTING 
    Setting::Setting()
        :Object("setting"){}
    Analysis::AnalysisPtr Setting::analysis(){
        Analysis::AnalysisPtr ptr; 
        ptr = this->GetElememt<Analysis>("analysis");
        if(ptr != nullptr)
            return ptr;
        auto analysis = std::make_shared<Analysis>();
        this->AddElememt(analysis);
        return analysis ;
    }
    //SETTING END
    //FIELDS 
    Fields::Fields(std::string fieldname)
        :Object(fieldname){
        this->Type("text");
    }
    void Fields::Type(std::string typrname){
        this->Add("type", typrname);}
    void Fields::Boost(double boostnum){
        this->Add("boost", boostnum);}
    void Fields::Analyzer(std::string analyzername){
        this->Add("analyzer", analyzername);}
    void Fields::Index(bool indexnum){
        this->Add("index", indexnum);}
    //FIELDS END
    //PROPERTIES
    Properties::Properties()
        :Object("properties"){}
    Fields::FieldsPtr Properties::field(std::string fieldname){
        Fields::FieldsPtr ptr; 
        ptr = this->GetElememt<Fields>(fieldname);
        if(ptr != nullptr)
            return ptr;
        auto field = std::make_shared<Fields>(fieldname);
        this->AddElememt(field);
        return field;
    }
    //PROPERTIES END
    //MAPPING
    Mappings::Mappings()
        :Object("mapping"){
        Dynamic(false);
    }
    void Mappings::Dynamic(bool dynamicnum){
        this->Add("dynamic", dynamicnum);
    }
    Properties::PropertiesPtr Mappings::properties(){
        Properties::PropertiesPtr ptr; 
        ptr = this->GetElememt<Properties>("properties");
        if(ptr != nullptr)
            return ptr;
        auto properties = std::make_shared<Properties>();
        this->AddElememt(properties);
        return properties;
    }
    //MAPPING END
    //INDEXER
    Indexer::Indexer()
        :Object("")
    {}
    Setting::SettingPtr Indexer::setting(){
        Setting::SettingPtr ptr; 
        ptr = this->GetElememt<Setting>("setting");
        if(ptr != nullptr)
            return ptr;
        auto setting = std::make_shared<Setting>();
        this->AddElememt(setting);
        return setting ;
    }
    Tokenizer::TokenizerPtr Indexer::BuildSettingChain(){
        return this->
        setting()->
        analysis()->
        analyzer()->
        tokenizer("ikmax") ;
    }
    Properties::PropertiesPtr Indexer::BuildMappingChain(){
        return this->
        mapping()->
        properties();
        //向外给一个Properties::PropertiesPtr的指针，
        // 然后用户可以拿着这个指针去添加各种他需要的Field对象
    }
    Mappings::MappingsPtr Indexer::mapping(){
        Mappings::MappingsPtr ptr; 
        ptr = this->GetElememt<Mappings>("mapping");
        if(ptr != nullptr)
            return ptr;
        auto mapping = std::make_shared<Mappings>();
        this->AddElememt(mapping);
        return mapping;
    }
    //INDEXER END
    //QUERY
    Term::Term(std::string field) 
        :Object("term"){
        _field = field ;
    }
    Terms::Terms(std::string field) 
        :Object("terms"){
        _field = field ;
    }
    Match::Match(std::string field) 
        :Object("match"){
        _field = field ;
    }
    MultiMatch::MultiMatch() 
        :Object("multi_match"){}
    Range::Range(std::string field) 
        :Object("range"){
            _field = field;
    }
    QueryObject::QueryObject(std::string key) 
        :Object(key){}
    void MultiMatch::AppendField(std::string val) {
        this->Append("fields", val);
    }
    void Range::Field(int gte, int lte) {
        this->Add("gte", gte);
        this->Add("lte",lte);
    }
    Range::RangePtr QueryObject::range(std::string field) {
        Range::RangePtr ptr; 
        ptr = this->GetElememt<Range>("range");
        if(ptr != nullptr)
            return ptr;
        auto range = std::make_shared<Range>(field);
        this->AddElememt(range);
        return range;
    }
    MultiMatch::MultiMatchPtr QueryObject::multimatch() {
        MultiMatch::MultiMatchPtr ptr; 
        ptr = this->GetElememt<MultiMatch>("multi_match");
        if(ptr != nullptr)
            return ptr;
        auto multimatch = std::make_shared<MultiMatch>();
        this->AddElememt(multimatch);
        return multimatch;
    }
    Match::MatchPtr QueryObject::match(std::string field) {
        Match::MatchPtr ptr; 
        ptr = this->GetElememt<Match>("match");
        if(ptr != nullptr)
            return ptr;
        auto match = std::make_shared<Match>(field);
        this->AddElememt(match);
        return match;
    }

    Terms::TermsPtr QueryObject::terms(std::string field) {
        Terms::TermsPtr ptr; 
        ptr = this->GetElememt<Terms>("terms");
        if(ptr != nullptr)
            return ptr;
        auto terms = std::make_shared<Terms>(field);
        this->AddElememt(terms);
        return terms;
    }

    Term::TermPtr QueryObject::term(std::string field) {
        Term::TermPtr ptr; 
        ptr = this->GetElememt<Term>("term");
        if(ptr != nullptr)
            return ptr;
        auto term = std::make_shared<Term>(field);
        this->AddElememt(term);
        return term;
    }
    //QUERY END
}