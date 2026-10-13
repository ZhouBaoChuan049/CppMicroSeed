#include "Person.pb.h"
#include <google/protobuf/util/json_util.h>

std::string Serialize_Test(){
    person::Student stu ;
    stu.set_sn(1234546);
    stu.set_name("张三");
    stu.set_sex(person::SexType::man);
    stu.add_score(99);
    auto score = stu.mutable_score();
    score->Add(88);
    score->Add(77);
    score->Add(66);
    auto other= stu.mutable_other();
    (*other)[0] = "abc";
    (*other)[1] = "def";
    other->insert({1,"def"});

    auto str = stu.SerializeAsString();
    std::cout<< str <<std::endl;
    return str ;
}
void UnSerialize_Test(const std::string& str){
    person::Student stu ;
    bool ret = stu.ParseFromString(str);
    if(ret == false){
        std::cout<<"反序列化失败!"<<std::endl;
        return ;
    }
    std::cout<<stu.sn()<<std::endl;
    std::cout<<stu.name()<<std::endl;
    std::cout<<stu.sex()<<std::endl;
    for(int i = 0 ; i < stu.score_size(); i ++)
        std::cout<< stu.score(i)<<std::endl;
    auto it = stu.other().find(1);
    if(it != stu.other().end())
        std::cout<< it->second <<std::endl;
    for(auto it : stu.other())
        std::cout<<it.first<<" "<<it.second<<std::endl;
}
void JsonToProtobuf_Test(){
    std::string JsonString = R"({
        "sn": 123456,
        "name": "张三",
        "sex": 2,
        "score": [
            99,88,77,66,55
        ],
        "other":{
            "1":"abc",
            "2":"def",
            "3":"ghi"
        }
    })";
    person::Student stu ;
    auto ret = 
        google::protobuf::util::JsonStringToMessage(
            JsonString,
            &stu
        );
    if(ret.ok() == false){
        std::cout<<"JsonToProtobuf Fail!"<<ret.ToString()<<std::endl;
        return ;
    }
    std::cout<<stu.name()<<std::endl;
    std::cout<<stu.sex()<<std::endl;
    std::cout<<stu.sn()<<std::endl;
    for(int i = 0 ; i < stu.score_size();i++)
        std::cout<< stu.score(i)<<std::endl;
    for(auto it : stu.other())
        std::cout<<it.first<<it.second<<std::endl;
}
void ProtobufToJson_Test(){
    person::Student stu ;
    stu.set_name("张三");
    stu.set_sn(123456);
    stu.set_sex(person::SexType::man);
    stu.add_score(99);
    stu.add_score(88);
    stu.add_score(77);
    stu.add_score(66);
    auto other = stu.mutable_other();
    other->insert({1,"abc"});
    other->insert({2,"def"});
    other->insert({3,"ghi"});

    std::string JsonString ;
    google::protobuf::util::JsonPrintOptions options ;
    options.add_whitespace = true ;
    auto ret = google::protobuf::util::MessageToJsonString(stu,&JsonString);
    if(ret.ok() == false){
        std::cout<<"ProtobufToJson Fail!"<<std::endl;
        return ;
    }
    std::cout<<JsonString<<std::endl;
}

int main(){
    auto str = Serialize_Test() ;
    UnSerialize_Test(str);
    std::cout<<std::endl;
    JsonToProtobuf_Test();
    std::cout<<std::endl;
    ProtobufToJson_Test();
    return 0 ;
}