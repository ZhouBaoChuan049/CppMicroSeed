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
void ShowIndexOnly(){
    auto index = std::make_shared<Indexer>() ;
    Tokenizer ::TokenizerPtr tokenizer = index->BuildChain();
    //还可以直接使用返回值来修改Tokenizer的字段
    // tokenizer->tokenzier("TestTokenizer");
    // tokenizer->type("TestType");
    std::string ret = index->ToString();
    std::cout<<ret<<std::endl;
}
int main()
{
    //ShowBaseOnly() ;
    //ShowObjectOnly();
    //ShowAnalysisOnly();
    //ShowSettingOnly();
    ShowIndexOnly();
    return 0 ; 
}