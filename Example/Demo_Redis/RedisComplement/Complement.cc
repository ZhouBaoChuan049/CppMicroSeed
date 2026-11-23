#include <cstdlib>
#include <iterator>
#include <sw/redis++/command_options.h>
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
        //字段的声明周期控制
        _redis.set("Name","Zhangsan");
        _redis.expire("Name",std::chrono::seconds(3));
        _redis.persist("Name");
        //判断是否存在该字段
        bool ret = _redis.exists("Name");
        std::cout<<"Name is Exist"<< ret<<std::endl;
        //zset字典序
        //1. 元素字典序区间数据获取-注意事项：元素权重必须相同；  2. 设置区间数据获取时的偏移量和个数
        // ZSet 的默认排序规则就是：
        // 1. 分数不同：按照 score 从小到大排列。
        // 2. 分数相同：按照 member 的字典序从小到大排列。
        _redis.zadd("score", "a", 100);
        _redis.zadd("score", "d", 100);
        _redis.zadd("score", "c", 100);
        _redis.zadd("score", "b", 100);
        _redis.zadd("score", "e", 100);
        _redis.zadd("score", "f", 100);
        sw::redis::LimitOptions limit;
        limit.offset = 1;//跳过第几个元素开始.这里是跳过第一个元素，所以是从第二个元素开始
        limit.count = 3;
        sw::redis::BoundedInterval<std::string> boundedinterval("b","f",sw::redis::BoundType::CLOSED);
        std::vector<std::string> Member;
        _redis.zrangebylex("score",boundedinterval,limit,std::back_inserter(Member));
        for(auto& mem : Member){
            std::cout<< mem <<" ";
        }
        std::cout<<std::endl;
        _redis.flushall();

    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}