#include <cstdlib>
#include <iterator>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
#include <sw/redis++/redis.h>
#include <iostream>
#include <sw/redis++/utils.h>
#include <vector>
#include <unordered_set>
int main(){
    try{
        sw::redis::ConnectionOptions ConnOpt{
            .host = "dev-redis",
            .password = "123456"};
        sw::redis::ConnectionPoolOptions PoolPot{
            .size = 10,
            .connection_idle_time = std::chrono::milliseconds(3600)};
        sw::redis::Redis _redis(ConnOpt,PoolPot);

        //向集合添加一个元素
        _redis.sadd("Skills","Cpp");
        //批量添加
        std::unordered_set<std::string> skills = {"Python","Java","Go"};
        _redis.sadd("Skills",skills.begin(),skills.end());
        
        //获取集合中元素的个数
        auto count = _redis.scard("Skills");
        std::cout<<"Skills Size is"<<count<<std::endl;
        //判断集合中是否包含某个元素
        bool ret1 = _redis.sismember("Skills", "Cpp");
        std::cout<<"Skill have member"<<ret1<<std::endl;
        bool ret2 = _redis.sismember("Skills", "C++");
        std::cout<<"Skill have member"<<ret2<<std::endl;
        //获取集合中的所有元素
        std::unordered_set<std::string> skills2;
        _redis.smembers("Skills", std::inserter(skills2,skills2.begin()));
        for(auto& val :skills2){
            std::cout<<"Val"<<":"<<val<<std::endl;
        }
        _redis.del("Skills");
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}