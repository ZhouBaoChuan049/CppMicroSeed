#include "../Include/ElasticSearch.h"
#include <cstddef>
#include <elasticlient/client.h>
#include <jsoncpp/json/value.h>
#include <memory>
#include "../Include/Log.h"

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
    //QUERY

    Query::Query()
        :QueryObject("query")
    {}
    void Query::match_all(){}
    QueryBool::QueryBoolPtr Query::Qbool(){
        auto ret = this->GetElememt<QueryBool>("bool");
        if(ret != nullptr)
            return ret ;
        auto _bool = std::make_shared<QueryBool>();
        this->AddElememt(_bool);
        return _bool;
    }
    Must::MustPtr Query::must(){ return this->Qbool()->must();}
    MustNot::MustNotPtr Query::mustnot(){ return this->Qbool()->mustnot();}
    Should::ShouldPtr Query::should(){ return this->Qbool()->should();}
    //QUERY END
    //REQUEST
    Request::Request(std::string _Operatoion,std::string _Index,
        std::string _Type,std::string _Id )
        :_Operatoion(_Operatoion),
         _Index(_Index),
         _Type(_Type),
         _Id(_Id){}
    void Request::SetIndex(std::string index){ _Index = index;}
    void Request::SetOperation(std::string Operation){ _Operatoion = Operation;}
    void Request::SetType(std::string Type){ _Type = Type;}
    void Request::SetID(std::string id){ _Id = id;}
    const std::string Request::Index()const{ return _Index;}
    const std::string Request::Type()const{ return _Type;}
    const std::string Request::Operatoion()const{ return _Operatoion;}
    const std::string Request::ID()const{ return _Id;}
    //REQUEST END
    //INDEXER
    Indexer::Indexer(std::string index)
        :Object(""),
         Request("index",
            index,
            "_doc",
            "index"//传递与否无所谓
        ){}
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
    //INSERT
    Insert::Insert(std::string index, std::string id)
        :Object(""),
         Request("_insert",
            index,
            "_doc",
            id
        ){}
    //INSERT END
    //UPDATE
    Update::Update(std::string index, std::string id)
        :Object(""),
        Request("_update",
        index,
        "_doc",
        id
    ){}
    Object::ObjectPtr Update::doc(){
        Object::ObjectPtr ptr; 
        ptr = this->GetElememt<Object>("_doc");
        if(ptr != nullptr)
            return ptr;
        auto doc = std::make_shared<Object>("_doc");
        this->AddElememt(doc);
        return doc;
    }
    //UPDATE END
    //DELETE 
    Delete::Delete(std::string index, std::string id)
        :Object(""),
         Request("_delete",
            index,
            "_doc",
            id
        ){}
    //DELETE END
    //SEARCH
    Search::Search(std::string index)
        :Object(""),
         Request("_search",
            index,
            "_doc",
            ""
        ){}
    Query::QueryPtr Search::query(){
        Query::QueryPtr ptr; 
        ptr = this->GetElememt<Query>("query");
        if(ptr != nullptr)
            return ptr;
        auto query = std::make_shared<Query>();
        this->AddElememt(query);
        return query;
    }
    void Search::size(size_t count){ this->Add("size", count);}
    void Search::form(size_t offset){ this->Add("form", offset);}
    //SEARCH END
    //ESCLIENT
    ESClient::ESClient(std::vector<std::string> host)
        :_Client(std::make_shared<elasticlient::Client>(host))
    {}
    bool ESClient::CreateIndex(const Indexer index){
        const std::string _Index = index.Index(); 
        const std::string _Type = index.Type();
        const std::string _ID = index.ID();
        const std::string _Body = index.ToString();
        auto response = _Client->index(
            _Index, 
            _Type, 
            _ID, 
            _Body
        );
        if(response.status_code < 200 || response.status_code >= 300){
            ERR(
                "{}/{} - {} Create Index Fail! ERR information:{}",
                _Index,_Type,_ID,_Body, response.text
            );
            return false;
        }
        return true;
    }
    bool ESClient::InsertDate(const Insert index) {
        const std::string _Index = index.Index(); 
        const std::string _Type = index.Type();
        const std::string _ID = index.ID();
        const std::string _Body = index.ToString();
        auto response = _Client->index(
            _Index, 
            _Type, 
            _ID, 
            _Body);
        if(response.status_code < 200 || response.status_code >= 300){
            ERR(
                "{}/{}/{} - {} Insert fail! ERR information:{}",
                _Index,_Type,_ID,_Body, response.text
            );
            return false;
        }
        return true;
    }
    bool ESClient::UpdateDate(const Update index) {
        const std::string _Index = index.Index(); 
        const std::string _Type = index.Type();
        const std::string _ID = index.ID();
        const std::string _Body = index.ToString();
        const std::string url = _Index + "_update" + _ID;
        auto response = 
            _Client->performRequest(
                elasticlient::Client::HTTPMethod::POST, 
                url, _Body
        );
        if(response.status_code < 200 || response.status_code >= 300){
            ERR("{}/{}/{} - {} Update fail! ERR information:{}",
                _Index,_Type,_ID,_Body, response.text
            );
            return false;
        }
        return true;
    }
    bool ESClient::DeleteDate(const Delete index) {
        const std::string _Index = index.Index(); 
        const std::string _Type = index.Type();
        const std::string _ID = index.ID();
        auto response = 
            _Client->remove(_Index,_Type, _ID);
        if(response.status_code < 200 || response.status_code >= 300){
            std::cout<<"!"<<response.status_code<<response.text<<std::endl;
            ERR("{}/{}/{} - {} Remove Data Fail ! ERR information:{}",
                _Index,_Type,_ID, response.text
            );
            return false;
        }
        return true;
    }
    bool ESClient::DeleteIndex(const std::string index) {
        auto response = _Client->performRequest(elasticlient::Client::HTTPMethod::DELETE, index, "");
        if(response.status_code < 200 || response.status_code >= 300){
            ERR("Remove Index {} Fail ! ERR information:{}",
                index, response.text
            );
            return false;
        }
        return true;
    }
    Json::Value ESClient::SearchDate(const Search index) {
        const std::string _Index = index.Index(); 
        const std::string _Type = index.Type();
        const std::string _ID = index.ID();
        const std::string _Body = index.ToString();
        auto response = _Client->search(
            _Index, 
            _Type, 
            _ID, 
            _Body
        );
        if(response.status_code < 200 || response.status_code >= 300){
            ERR(
                "{}/{}/{} - {} Search Date fail! ERR information:{}",
                _Index,_Type,_ID,_Body, response.text
            );
            return false;
        }
        MicroSeedUtil::JsonUtil Jutil;
        auto rawresp = Jutil.UnSerialize(response.text);
        if(!rawresp){
            ERR(
                "{}/{}/{} - {} Get Response fail! ERR information:{}",
                _Index,_Type,_ID,_Body, response.text
            );
            return false;
        }
        if(rawresp["hits"].isNull() || 
            rawresp["hits"]["hits"].isNull() ||
                rawresp["hits"]["hits"][0]["_source"].isNull()){
                    ERR("{}/{}/{} - {} Get Response fail! ERR information:{}",
                        _Index,_Type,_ID,_Body, response.text
                    );
                    Json::Value root ;
                    return root;
                }
        Json::Value root ;
        for(int i = 0 ; i < rawresp["hits"]["hits"].size() ; i ++)
            root.append(rawresp["hits"]["hits"][i]["_source"]);
        return root ;
    }
    //ESCLIENT END
}
