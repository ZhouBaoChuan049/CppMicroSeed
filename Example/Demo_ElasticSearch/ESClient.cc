#include <cpr/response.h>
#include <elasticlient/client.h>
#include <iostream>
#include <thread>

const std::string DefaultIndex = "studentindex" ;
const std::string DefaultTypeName = "dictionary" ;
const std::string DefaultID = "numberx" ;

void CreateIndex(elasticlient::Client& client)
{
    const std::string Body = R"({
            "settings": {
                "analysis": {
                    "analyzer": {
                        "ikmax" : {
                            "type": "custom",
                            "tokenizer" : "ik_max_word"
                        }
                    }
                }
            },
            "mapping": {
                "dynamic": false,
                "properties": {
                    "name": {
                        "type": "text",
                        "boost": 3.0,
                        "analyzer": "ikmax"
                    },
                    "age": {
                        "type": "integer"
                    },
                    "phone": {
                        "type": "keyword", 
                        "boost": 1.0
                    },
                    "skills": { 
                        "type": "text"
                    },
                    "birth": {
                        "type": "date",
                        "index": false
                    }
                }
            }
        })";
        auto response = client.index(
            DefaultIndex, 
            DefaultTypeName, 
            DefaultID, //为什么创建索引需要ID？
            Body
        );
        if(response.status_code < 200 || response.status_code >= 300){
            std::cout<<"Create Index Fail!"<<response.status_code<<response.text<<std::endl;
            return ;
        }
        return ;
}
void AddData(elasticlient::Client& client)
{
    const std::string Body = R"({
            "name": "张三",
            "age": 19,
            "phone": "123456789",
            "skills": ["java","php","go"],
            "birth": "2007-05-21 12:35:32"
        })";
    auto response = client.index(DefaultIndex, DefaultTypeName, "2", Body);
    if(response.status_code < 200 || response.status_code >= 300){
        std::cout<<"Insert Data Fail!"<<response.status_code<<response.text<<std::endl;
        return ;
    }
    return ;
}
void ModData(elasticlient::Client& client)
{
    const std::string Body = R"({
        "name": "李四"
    })";
    //更新元素信息——类似于删除对应ID的原文档信息，然后添加新的对应ID的文档信息。
    auto response = client.index(DefaultIndex, DefaultTypeName, "2", Body);
    if(response.status_code < 200 || response.status_code >= 300){
        std::cout<<"Mod Data Fail!"<<response.status_code<<response.text<<std::endl;
        return ;
    }
    return ;
}
void RemoveData(elasticlient::Client& client){
    auto response = client.remove(DefaultIndex, DefaultTypeName, "2");
    if(response.status_code < 200 || response.status_code >= 300){
        std::cout<<"Remove Data Fail!"<<response.status_code<<response.text<<std::endl;
        return ;
    }
    return ;
}
void RemoveIndex(elasticlient::Client& client){
    auto response = client.performRequest(elasticlient::Client::HTTPMethod::DELETE, DefaultIndex, "");
    if(response.status_code < 200 || response.status_code >= 300){
        std::cout<<"Remove Index Fail!"<<response.status_code<<response.text<<std::endl;
        return ;
    }
    return ;
}
void GetAddData(elasticlient::Client& client)
{
    const std::string Body = R"({
        "query" : {
            "match_all" : {}
        }
    })";
    auto response = client.search(DefaultIndex, DefaultTypeName, Body);
    if(response.status_code < 200 || response.status_code >= 300){
        std::cout<<"GetAdd Data Fail!"<<response.status_code<<response.text<<std::endl;
        return ;
    }
    std::cout<<"Response["<<response.text<<"]"<<std::endl;
    return ;
}
int main()
{
    const std::string URL = "http://elastic:123456@dev-elasticsearch:9200/";
    elasticlient::Client client({URL});
    
    CreateIndex(client);
    std::this_thread::sleep_for(std::chrono::seconds(1));//避免还未处理完毕休眠一下
    AddData(client);
    std::this_thread::sleep_for(std::chrono::seconds(1));//避免还未处理完毕休眠一下
    GetAddData(client);
    std::this_thread::sleep_for(std::chrono::seconds(1));//避免还未处理完毕休眠一下
    ModData(client);
    std::this_thread::sleep_for(std::chrono::seconds(1));//避免还未处理完毕休眠一下
    GetAddData(client);
    std::this_thread::sleep_for(std::chrono::seconds(1));//避免还未处理完毕休眠一下
    RemoveData(client);
    std::this_thread::sleep_for(std::chrono::seconds(1));//避免还未处理完毕休眠一下
    RemoveIndex(client);
    return 0 ;
}

