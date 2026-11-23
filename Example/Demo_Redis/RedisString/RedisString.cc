#include <cstdlib>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
#include <sw/redis++/errors.h>
#include <sw/redis++/redis.h>
#include <iostream>
#include <sw/redis++/utils.h>
int main(){
    try{
        sw::redis::ConnectionOptions ConnOpt{
            .host = "dev-redis",
            .password = "123456"
        };
        sw::redis::ConnectionPoolOptions PoolPot{
            .size = 10,
            .connection_idle_time = std::chrono::milliseconds(3600)
        };
        sw::redis::Redis _redis(ConnOpt,PoolPot);
        //我们要尝试的:
        //添加、获取、删除、自增、自减
        bool ret = _redis.set("Name","Mr.Han");
        if(!ret){
            std::cout<<"Add a Key-Value Fail!"<<std::endl;
            abort();
        }
        auto val = _redis.get("Name");
        if(!val){
            std::cout<<"Get Key-Value Fail!"<<std::endl;
            abort();
        }
        std::cout<<"Val:"<<*val<<std::endl;
        _redis.del("Name");

        std::cout<<"#########################################"<<std::endl;
        _redis.set("age","999");
        std::cout << "age: " << *_redis.get("age") << std::endl;
        _redis.incrby("age", 1000);
        std::cout << "age: " << *_redis.get("age") << std::endl;
        _redis.decrby("age", 100);
        std::cout << "age: " << *_redis.get("age") << std::endl;
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}