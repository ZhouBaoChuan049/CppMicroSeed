#include <cstdlib>
#include <iterator>
#include <sw/redis++/command_options.h>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
#include <sw/redis++/errors.h>
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
        //watch操作
        //  1. 样例： 对stu中的age字段进行增长
        //操作流程：
        //  1. 通过redis对象，创建transaction对象
        //  2. 通过transaction对象，创建redis对象（这个redis对象与transaction对象共享连接）
        //  3. 通过redis对象执行watch操作( 1. transaction中没有watch操作； 2. transaction操作无法立即获取结果，但是redis操作可以)
        //  4. 通过redis对象执行exists判断字段是否存在
        //  5. 若字段存在，则通过transaction对象，执行incrby操作（事务执行过程中，若key值发生改变，则会抛出异常）
        {
            auto transacoion = _redis.transaction();
            auto redis = transacoion.redis();
            //不要使用外面的那个redis对象，因为要避免并发线程安全
            while(true){
                try{
                    redis.watch("score");
                    //如果数值被别的客户端修改，一定会抛异常，下面的都不会执行
                    bool ret = redis.exists("score");
                    if(!ret){
                        break;
                        //一般的，如果不存在我们会将数据从数据库加载到缓存，然后再执行事务
                        //但是我们现在选择什么都不做
                    }
                    transacoion.incrby("score", 1);
                    transacoion.exec();
                    break;
                }catch(const sw::redis::WatchError& e){
                    //原因有很多，最可能的：key数值被别的客户都安修改
                    std::cout<<"Watch Err:"<<e.what()<<std::endl;
                    //输出错误原因后循环回去重新操作，直到操作成功跳出循环
                }
            }
        }
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}