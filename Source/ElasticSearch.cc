#include "../Include/ElasticSearch.h"
#include <cstddef>
#include <jsoncpp/json/value.h>
#include <memory>

namespace MicroSeedElastic
{
    //BASE 
    Base::Base(std::string key)
        :_Key(key){}
    const std::string Base::ToString()const{
        Json::Value root ;
        root[_Key] = _Val ;
        MicroSeedUtil::JsonUtil util ;
        return util.Serialize(root, true);
    }
    std::string Base::GetKey()const{ return _Key;}
    const Json::Value Base::GetVal()const{ return _Val;}
    //BASE END
    //OBJECT 
    Object::Object(std::string val)
        :Base(val){}
    void Object::AddElememt(const BasePtr& baseptr){
        _Objects[baseptr->GetKey()] = baseptr;
    }
    const std::string Object::ToString()const {
        Json::Value root ;
        for(auto i : _Objects){
            root[i.first] = i.second->GetVal() ;
        }
        MicroSeedUtil::JsonUtil util ;
        return util.Serialize(root, true);
    }
    const Json::Value Object::GetVal()const {
        Json::Value root = _Val ;
        for(auto obj : _Objects)
            root[obj.first] = obj.second->GetVal();
        //假如这样调用: std::string ret = analysis->ToString();
        //那么Analysis调用GetVal，在子对象存储器里面调用了Analyzer的GetVal，
        //Analyzer的GetVal又调用了Tokenizer的GetVal(),将Tokenzer的Val向上传递.
        return root ;
    }
    std::shared_ptr<Object> Object::NewObject(const std::string& key){
        Object::ObjectPtr ptr; 
        ptr = this->GetElememt<Object>(key);
        if(ptr != nullptr)
            return ptr;
        auto newobj = std::make_shared<Tokenizer>(key);
        this->AddElememt(newobj);
        return newobj;
    }
    std::shared_ptr<Array> Object::NewArray(const std::string& key){
        Array::ArrayPtr ptr; 
        ptr = this->GetElememt<Array>(key);
        if(ptr != nullptr)
            return ptr;
        auto newarr = std::make_shared<Array>(key);
        this->AddElememt(newarr);
        return newarr;
    }
    //OBJECT END

    //ARRAY
    Array::Array(const std::string key)
        :Base(key){}
    std::shared_ptr<Object> Array::NewObject(const std::string& key){
        //在数组里面，我认为不需要去判断我加入
        //进来的这个对象是否存在，完全可以重复出现.
        auto newobj = std::make_shared<Tokenizer>(key);
        this->AddElememt(newobj);
        return newobj;
    }
    std::shared_ptr<Array> Array::NewArray(const std::string& key){
        auto newarr = std::make_shared<Array>(key);
        this->AddElememt(newarr);
        return newarr;
    }
    void Array::AddElememt(const Base::BasePtr& baseptr){
        _Objects.push_back(baseptr);
    }
    const Json::Value Array::GetVal() const {
        Json::Value root ;
        for(auto sub : _Objects)
            root.append(sub->GetVal());
        return root ;
    }
    const std::string Array::ToString() const {
        Json::Value root ;
        for(auto sub : _Objects)
            root.append(sub->GetVal());
        MicroSeedUtil::JsonUtil util ;
        return util.Serialize(root, true);
    }
    //ARRAY END
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
    //QUERYOBJECT
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
    //QUERYOBJECT END

    //QUERYARRAY
    QueryArray::QueryArray(std::string key)
        :Array(key){}
    Range::RangePtr QueryArray::range(std::string field){
        auto range = std::make_shared<Range>(field);
        auto obj = NewObject("");
        obj->AddElememt(range);
        return range ;
    }
    MultiMatch::MultiMatchPtr QueryArray::multimatch(){
        auto multimatch = std::make_shared<MultiMatch>();
        auto obj = NewObject("");
        obj->AddElememt(multimatch);
        return multimatch ;
    }
    Match::MatchPtr QueryArray::match(std::string field){
        auto match = std::make_shared<Match>(field);
        auto obj = NewObject("");
        obj->AddElememt(match);
        return match;
    }
    Terms::TermsPtr QueryArray::terms(std::string field){
        auto terms = std::make_shared<Terms>(field);
        auto obj = NewObject("");
        obj->AddElememt(terms);
        return terms;
    }
    Term::TermPtr QueryArray::term(std::string field){
        auto term = std::make_shared<Term>(field);
        auto obj = NewObject("");
        obj->AddElememt(term);
        return term;
    }
    //QUERYARRAY END
    //MUST MUSTNOT STUDENT
    Must::Must(std::string key)
        :QueryArray(key){}
    MustNot::MustNot(std::string key)
        :QueryArray(key){}
    Should::Should(std::string key)
        :QueryArray(key){}
    //MUST MUSTNOT STUDENT END

    //QUERYBOOL
    QueryBool::QueryBool(std::string key)
        :QueryObject(key){}
    Must::MustPtr QueryBool::must(){
        auto ret = this->GetElememt<Must>("must");
        if(ret != nullptr)
            return ret ;
        auto _must = std::make_shared<Must>();
        this->AddElememt(_must);
        return _must;
    }
    MustNot::MustNotPtr QueryBool::mustnot(){
        auto ret = this->GetElememt<MustNot>("must_not");
        if(ret != nullptr)
            return ret ;
        auto _mustnot = std::make_shared<MustNot>();
        this->AddElememt(_mustnot);
        return _mustnot;
    }
    Should::ShouldPtr QueryBool::should(){
        auto ret = this->GetElememt<Should>("should");
        if(ret != nullptr)
            return ret ;
        auto _should = std::make_shared<Should>();
        this->AddElememt(_should);
        return _should;
    }
    void QueryBool::MiniMumShouldMatch(const int num){
        this->Add("mininum_should_match", num);
    }
    //QUERYBOOL END

}