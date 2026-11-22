#include "../../../Include/Util.h"
#include <iostream>
using namespace MicroSeedUtil;

void TestRandomNum(int min,int max){
    std::cout<<"#########################################"<<std::endl;
    for(int i = 0 ; i < 10 ;i ++){
        RandomUtil util;
        size_t ret = util.RandomNum(min, max);
        std::cout<<"ret: ["<<ret<<"]"<<std::endl;
    }
    std::cout<<"#########################################"<<std::endl;
}
void TestRandomCode(int len,const CodeType type){
    std::cout<<"#########################################"<<std::endl;
    for(int i = 0 ; i < 10 ; i ++){
        RandomUtil util;
        std::string ret = util.RandomCode(len, type);
        std::cout<<"ret: ["<<ret<<"]"<<std::endl;
    }
    std::cout<<"#########################################"<<std::endl;
}
int main(){
    TestRandomNum(100,900);
    TestRandomCode(20, CodeType::MIX);
    return 0;
}
