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
        //左侧插入数据
        _redis.lpush("Skills","C++");

        std::vector<std::string> Arr = {"C","Linux","MYSQL"};
        _redis.lpush("Skills",Arr.begin(),Arr.end());
        //从右侧获取数据
        //获取元素的总个数
        std::cout<<"########################################"<<std::endl;
        int len = _redis.llen("Skills");
        for(int i = len-1;i >= 0 ; i --){
            //通过下标获取Key对应的Val
            auto Val = _redis.lindex("Skills",i);
            std::cout<<*Val<<std::endl;
        }
        std::cout<<"########################################"<<std::endl;
        std::vector<std::string> skills;
        //获取从[start,stop]的所有数据,stop = -1默认表示获取一切数据
        //std::back_inserter是一个尾插器,封装了底层尾插的详细调用方式.
        _redis.lrange("Skills",0,-1,std::back_inserter(skills));
        for(auto& val :skills){
            std::cout<< val <<std::endl;
        }
        std::cout<<"########################################"<<std::endl;
        _redis.lpop("Skills");
        _redis.rpop("Skills");
        skills.clear();
        _redis.del("Skills");
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}