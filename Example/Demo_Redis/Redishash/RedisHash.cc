#include <cstdlib>
#include <iterator>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
#include <sw/redis++/redis.h>
#include <iostream>
#include <sw/redis++/utils.h>
#include <vector>
int main(){
    try{
        sw::redis::ConnectionOptions ConnOpt{
            .host = "dev-redis",
            .password = "123456"};
        sw::redis::ConnectionPoolOptions PoolPot{
            .size = 10,
            .connection_idle_time = std::chrono::milliseconds(3600)};
        sw::redis::Redis _redis(ConnOpt,PoolPot);
        //添加
        _redis.hset("User","Name","ZhangSan");
        std::unordered_map<std::string,std::string> Uinfo = {
            {"age","19"},
            {"gender","Man"},
            {"hobby","music"}
        };
        //批量添加
        _redis.hmset("User",Uinfo.begin(),Uinfo.end());
        auto val = _redis.hget("User","Name");
        if(!val){
            std::cout<<"获取键值对失败!"<<std::endl;
            abort();
        }
        std::cout<<"Name"<<*val<<std::endl;
        //获取所有
        std::unordered_map<std::string,std::string> Results;
        _redis.hgetall("User", std::inserter(Results,Results.begin()));
        for(auto& Val : Results){
            std::cout<< Val.first<< ":"<<Val.second<<std::endl;
        }
        //自增自减
        _redis.hincrby("User","age",1);
        std::cout<<"Age:"<<*_redis.hget("User","age")<<std::endl;
        _redis.hincrby("User","age",-5);
        std::cout<<"Age:"<<*_redis.hget("User","age")<<std::endl;
        _redis.del("User");
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}