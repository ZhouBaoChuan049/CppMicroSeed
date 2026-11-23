#include <cstdlib>
#include <iterator>
#include <sw/redis++/command_options.h>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
#include <sw/redis++/redis++.h>
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
        {
            auto pipe = _redis.pipeline(false);
            //头文件要求#include <sw/redis++/redis++.h>
            pipe.set("key1", "value1");
            pipe.set("key2", "value2");
            pipe.set("key3", "value3");
            pipe.set("key4", "value4");
            pipe.get("key4");
            pipe.get("key3");
            auto reply = pipe.exec();
            std::cout<<"结果数量:"<<reply.size()<<std::endl;

            std::cout << reply.get<bool>(0)<<std::endl;
            std::cout << reply.get<bool>(1) << std::endl;
            std::cout << reply.get<bool>(2) << std::endl;
            std::cout << reply.get<bool>(3) << std::endl;
            auto val4 = reply.get<sw::redis::OptionalString>(4);
            std::cout << "key4: " << *val4 << std::endl;
            auto val3 = reply.get<sw::redis::OptionalString>(5);
            std::cout << "key3: " << *val3 << std::endl;
        }
        {
            auto pipe = _redis.pipeline(false);
            pipe.hset("stu", "name", "zhangsan");
            pipe.hset("stu", "age",        "20");
            pipe.hset("stu", "gender",   "male");
            pipe.hgetall("stu");
            auto reply = pipe.exec();

            std::unordered_map<std::string,std::string> stu;
            reply.get(3,std::inserter(stu,stu.begin()));
            for(auto& val : stu){
                std::cout<< val.first <<": "<<val.second<<std::endl;
            }
        }
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}