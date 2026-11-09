#include "../../Include/ElasticSearch.h"
#include "../../Include/Log.h"
#include <iostream>
#include <vector>
using namespace MicroSeedElastic;
using namespace MicroSeedLog;
using namespace MicroSeedUtil;

const std::string DEFAULT_INDEX_NAME = "defaultindex";
const std::string URL = "http://elastic:123456@dev-elasticsearch:9200/";
static size_t ID = 1 ;//全局自增
void CreateANewIndex(ESClient& client){
    Indexer index(DEFAULT_INDEX_NAME) ;
    Tokenizer::TokenizerPtr tokenizeer = index.BuildSettingChain();
    Properties::PropertiesPtr properties = index.BuildMappingChain();
    tokenizeer->tokenzier("ik_max_word");
    properties->field("user_id")->Type("keyword");
    properties->field("nickname")->Type("text");
    properties->field("nickname")->Analyzer("ikmax");
    properties->field("phone")->Type("keyword");
    properties->field("description")->Type("text");
    properties->field("description")->Analyzer("ikmax");
    properties->field("avatar_id")->Type("keyword");
    properties->field("avatar_id")->Index(false);
    bool check = client.CreateIndex(index);
    if(check == false) ERR("创建索引异常!");
}

void InsertANewDate(
    ESClient& client, 
    std::string name, 
    std::string age, 
    std::string phone,
    std::vector<std::string> skillcollect,
    std::string birthday
){
    Insert insert(DEFAULT_INDEX_NAME, std::to_string(ID++));
    insert.Add("name", name);
    insert.Add("age", age);
    insert.Add("phone", phone);
    insert.Add("birthday", birthday);
    for(auto skill : skillcollect)
        insert.Append("skills", skill);
    bool check = client.InsertDate(insert);
    if(check == false) ERR("插入数据异常!");
}
void UpdateANewDate(ESClient& client, std::string updateid){
    Update update(DEFAULT_INDEX_NAME,updateid);
    update.doc();
    update.Add("phone", "6666666666666666");
    update.Add("Age","999");
    bool check = client.UpdateDate(update);
    if(check == false) ERR("更新数据异常-S!");
}
void UpdateANewDateByIsert(ESClient& client){
    Insert insert(DEFAULT_INDEX_NAME, "1");
    insert.Add("name", "Mr.Zhaoliu");
    insert.Add("phone", "9999999999");
    insert.Add("age", 18);
    insert.Add("birth", "2018-08-19 22:22:33");
    insert.Append("skills", "C++");
    insert.Append("skills", "Java");
    insert.Append("skills", "Python");
    bool check = client.InsertDate(insert);
    if (check == false) ERR("更新数据异常-J!");
}
void SearchALLDate(ESClient& client)
{
    Search search(DEFAULT_INDEX_NAME);
    search.query()->match_all();
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void SearchANewDateByTermExampleNAME(ESClient& client, std::string name)
{
    Search search(DEFAULT_INDEX_NAME);
    Query::QueryPtr _query = search.query();
    _query->term("name.keyword")->SetVal(name);
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void SearchANewDateByTermsExamplePhone(ESClient& client, std::vector<std::string> member)
{
    Search search(DEFAULT_INDEX_NAME);
    Query::QueryPtr _query = search.query();
    for(auto m:member)
        _query->terms("phone")->SetVal(m);
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void SearchANewDateByMatchExampleNAME(ESClient& client, std::string member)
{
    Search search(DEFAULT_INDEX_NAME);
    Query::QueryPtr _query = search.query();
    _query->match("name")->SetVal(member);
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void SearchANewDateByRangeExampleAge(ESClient& client)
{
    Search search(DEFAULT_INDEX_NAME);
    Query::QueryPtr _query = search.query();
    _query->range("age")->Field(10, 20);
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void SearchANewDateByMultiMatchExampleNameAndPhone(ESClient& client)
{
    Search search(DEFAULT_INDEX_NAME);
    Query::QueryPtr _query = search.query();
    _query->multimatch()->AppendField("name");
    _query->multimatch()->AppendField("phone");
    _query->multimatch()->SetQuery("Mr.ZahngSan");
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void SearchANewDateByMoreInformation(ESClient& client)
{
    Search search(DEFAULT_INDEX_NAME);
    Query::QueryPtr _query = search.query();
    QueryBool::QueryBoolPtr qbool = _query->Qbool();
    Must::MustPtr mustptr = qbool->must();
    mustptr->match("skills")->SetVal("C++");
    MustNot::MustNotPtr mustnotptr = qbool->mustnot();
    mustnotptr->terms("phone")->SetVal("19111111111");
    mustnotptr->terms("phone")->SetVal("19444444444");
    Should::ShouldPtr shouldptr = qbool->should();
    shouldptr->match("name")->SetVal("Mr.list");
    shouldptr->terms("phone")->SetVal("19333333333");
    shouldptr->terms("phone")->SetVal("19222222222"); 
    Json::Value resp = client.SearchDate(search);
    JsonUtil util ;
    std::string ret = util.Serialize(resp);
    std::cout<<ret<<std::endl;
}
void DeleteANewDate(ESClient& client,std::string idtodelete){
    Delete _delete(DEFAULT_INDEX_NAME, idtodelete);
    bool check = client.DeleteDate(_delete);
    if (check == false) ERR("删除数据异常-J!");
}
void DeleteTheIndex(ESClient& client){
    bool check = client.DeleteIndex(DEFAULT_INDEX_NAME);
    if (check == false) ERR("删除数据异常-J!");
}
int main(){
    MicroSeedLog::MicroSeedLogInit();
    std::vector<std::string> UrlCollect ;
    UrlCollect.push_back(URL);
    ESClient client(UrlCollect);
    //[测试编排计划]:
    //创建索引——>插入“Mr.ZangSan”数据——>插入“Mr.list”数据——>插入“Mr.Wangwu”数据
    //——>查找一次——>更新“Mr.ZangSan”,“Mr.Wangwu”,“Mr.list”的数据——>再查找一次——>删除“Mr.ZangSan”数据
    //——>再查找一次——>删除索引——>最后再查找一次。    
    //# Insert:
    CreateANewIndex(client);
    std::vector<std::string> ZangSkills = {"C++","Java","Mysql"};
    InsertANewDate(client,"Mr.ZangSan","18","19111111111"
            ,ZangSkills,"1999-1-1");
    std::vector<std::string> ListSkills = {"Python", "Go", "Redis"};
    InsertANewDate(client, "Mr.list", "22", "19222222222"
            , ListSkills, "2000-5-20");
    std::vector<std::string> WangwuSkills = {"C#", "JavaScript", "Oracle"};
    InsertANewDate(client, "Mr.Wangwu", "25", "19333333333"
            , WangwuSkills, "1998-8-15");
    SearchALLDate(client);
    
    //#Search:
    SearchANewDateByTermExampleNAME(client,"Mr.ZhangSan");
    SearchANewDateByTermsExamplePhone(client,{"19111111111","19222222222","19333333333"});
    SearchANewDateByMatchExampleNAME(client,"l");
    SearchANewDateByRangeExampleAge(client);
    SearchANewDateByMultiMatchExampleNameAndPhone(client);
    SearchANewDateByMoreInformation(client);
    //#Update1:
    UpdateANewDate(client,"1");
    UpdateANewDate(client,"2");
    UpdateANewDate(client,"3");
    SearchALLDate(client);
    //#Update2:
    UpdateANewDateByIsert(client);
    SearchALLDate(client);
    //#Delete1:
    DeleteANewDate(client,"1");
    SearchALLDate(client);
    //#Delete2:
    DeleteTheIndex(client);
    return 0 ; 
}