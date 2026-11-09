#include "../../Include/ElasticSearch.h"
#include <iostream>
#include <memory>
using namespace MicroSeedElastic ;
void ShowBaseOnly(){
    Base BaseValue("ikmax");
    BaseValue.Add("type", "custom");
    BaseValue.Add("tokenizer", "ik_max_word");
    std::cout<<BaseValue.ToString()<<std::endl;
}
void ShowObjectOnly(){
    auto objectptr1 = std::make_shared<Object>("ikmax") ;
    objectptr1->Add("type", "custom")
            .Add("tokenizer", "ik_max_word");
    std::shared_ptr<Object> obj = std::make_shared<Object>("analyzer");
    obj->AddElememt(objectptr1);
    std::cout<<obj->ToString()<<std::endl;
}
void ShowAnalysisOnly(){
    auto analysis = std::make_shared<Analysis>() ;
    analysis->analyzer()->tokenizer("ikmax");
    //这里非常不好理解:
    //1. 我们创建了一个Analysis，
    //    然后调用analyzer()创建了一个analyzer的智能指针，
    //    然后将这个智能指针的名字和对象存放在子对象存储器中
    //    返回这个智能指针
    //2.用这个analyzer智能指针再次重复这个操作。
    //    整个就是一个由外向内的Json构建的过程
    std::string ret = analysis->ToString();
    //然后analysis调用Tostring，Tostring里面又调用GetVal，
    // GetVal里面将Val，和这个子对象存储器放在一起。一块儿返回
    std::cout<<ret<<std::endl;
}
void ShowSettingOnly(){
    auto setting = std::make_shared<Setting>() ;
    setting->analysis()->analyzer()->tokenizer("ikmax");
    std::string ret = setting->ToString();
    std::cout<<ret<<std::endl;
}
void ShowIndexWithSetting(){
    auto index = std::make_shared<Indexer>() ;
    Tokenizer ::TokenizerPtr tokenizer = index->BuildSettingChain();
    //还可以直接使用返回值来修改Tokenizer的字段
    // tokenizer->tokenzier("TestTokenizer");
    // tokenizer->type("TestType");
    std::string ret = index->ToString();
    std::cout<<ret<<std::endl;
}
void ShowIndexWithSettingAndMapping(){
    auto index = std::make_shared<Indexer>() ;
    Tokenizer ::TokenizerPtr tokenizer = index->BuildSettingChain();
    Properties::PropertiesPtr properties = index->BuildMappingChain();
    Fields::FieldsPtr fieldptr_name = properties->field("name");
    fieldptr_name->Boost(3.0);
    fieldptr_name->Analyzer("ikmax");
    Fields::FieldsPtr fieldptr_age = properties->field("age");
    fieldptr_age->Type("integer");
    Fields::FieldsPtr fieldptr_phone = properties->field("phone");
    fieldptr_phone->Type("keyword");
    properties->field("skill");
    Fields::FieldsPtr fieldptr_birth = properties->field("birth");
    fieldptr_birth->Type("date");
    fieldptr_birth->Index(false);
    std::string ret = index->ToString();
    std::cout<<ret<<std::endl;
}
void ShowQueryWithHisSub(){
    QueryObject qobj("query") ;
    auto termptr = qobj.term("name.keyword");
    termptr->SetVal("Mr.List");
    auto termsptr = qobj.terms("phonenumber");
    termsptr->SetVal("12125648595");
    termsptr->SetVal("12456786495");
    auto matchptr = qobj.match("name");
    matchptr->SetVal("Mr.Zcom");
    auto rangeptr = qobj.range("age");
    rangeptr->Field(10,20);
    auto multimatchptr = qobj.multimatch();
    multimatchptr->AppendField("name");
    multimatchptr->AppendField("phone");
    multimatchptr->SetQuery("Mr.Zount");
    std::string QobjString = qobj.ToString();
    std::cout<<QobjString<<std::endl;
}
int main()
{
    //ShowBaseOnly() ;
    //ShowObjectOnly();
    //ShowAnalysisOnly();
    //ShowSettingOnly();
    //ShowIndexOnly_1();
    //ShowIndexWithSettingAndMapping();
    ShowQueryWithHisSub();
    return 0 ; 
}