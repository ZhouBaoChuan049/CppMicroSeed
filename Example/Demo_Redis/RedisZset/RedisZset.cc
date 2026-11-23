#include <cstdlib>
#include <iterator>
#include <string>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
#include <sw/redis++/errors.h>
#include <sw/redis++/redis.h>
#include <iostream>
#include <sw/redis++/utils.h>
#include <vector>

void Print(sw::redis::Redis& redis, const std::string& key){
    sw::redis::Cursor cursor = 0;
    std::vector<std::pair<std::string,double>> Member;
    while(true){
        cursor = redis.zscan(
            key,
            cursor,
            "*",
            10,
            std::back_inserter(Member));
        if(cursor == 0) break;
    }
    for(auto& mem : Member){
        std::cout<<mem.first<<mem.second<<std::endl;
    }
}

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
        
        //新增数据
        _redis.zadd("score","ZhangSan",33);
        std::unordered_map<std::string,double> m = {
            {"zhangsan", 91}, {"lisi", 95    }, {"wangwu", 56  },
            {"zhaoliu", 78 }, {"tianqi", 89  }, {"xiaohong", 99},
            {"xiaoming", 88}, {"xiaolan", 77 }, {"xiaowang", 66},
            {"lihua", 83   }, {"liwei", 72   }, {"chenhao", 94 },
            {"chenxi", 65  }, {"yangyang", 87}, {"zhouyu", 59  },
            {"wanglei", 76 }, {"sunhao", 92  }, {"sunli", 68   },
            {"zhangwei", 81}, {"zhangyu", 73 }, {"huangli", 97 },
            {"huangwei", 54}, {"liuyang", 86 }, {"liujun", 63  },
            {"xuchen", 90  }, {"xufeng", 79  }, {"gaoming", 58 },
            {"gaoyang", 84 }, {"hejun", 71   }, {"tangxin", 98 }
        };
        //批量添加
        _redis.zadd("score",m.begin(),m.end());
        Print(_redis,"score");
        std::cout<<"#######################插入元素结束##########################"<<std::endl;
        std::cout<<"分数集合元素数量:"<<_redis.zcard("score")<<std::endl;
        auto score = _redis.zscore("score","xiaohong");
        if(score){
            std::cout<<"xiaohong得分:"<<*score<<std::endl;
        }
        auto rank = _redis.zrank("score","xiaohong");
        if(rank){ std::cout<<"xiaohong从低到高排名:"<<*rank<<std::endl;}
        auto revrank = _redis.zrevrank("score","xiaohong");
        if(revrank){ std::cout<<"xiaohong从高到低排名:"<<*revrank<<std::endl;}
        std::vector<std::string> result;
        _redis.zrange("score",0,2,std::back_inserter(result));
        std::cout<<"从低到高排名前3名:";
        for(auto& member : result){
            std::cout<<member<<" ";
        }
        std::cout<<std::endl;
        result.clear();
        _redis.zrevrange("score",0,2,std::back_inserter(result));
        std::cout<<"从高到低排名前3名:";
        for(auto& member : result){
            std::cout<<member<<" ";
        }
        std::cout<<std::endl;

        //以得分获取区间元素(从低到高/从高到低)
        result.clear();
        auto interval = sw::redis::BoundedInterval<double>(0,60,sw::redis::BoundType::RIGHT_OPEN);
        _redis.zrangebyscore("score",interval,std::back_inserter(result));
        std::cout<<"60分以下的学生(从低到高):";
        for(auto& member : result){
            std::cout<<member<<" ";
        }
        std::cout<<std::endl;

        result.clear();
        _redis.zrevrangebyscore("score",interval,std::back_inserter(result));
        std::cout<<"60分以下的学生(从高到低):";
        for(auto& member : result){
            std::cout<<member<<" ";
        }
        std::cout<<std::endl;

        //删除指定元素
        std::cout<<"---------------------删除xiaohong------------------"<<std::endl;
        _redis.zrem("score","xiaohong");
        Print(_redis,"score");
        std::cout<<"---------------------删除数据完毕---------------"<<std::endl;
        //弹出集合中最高/最低分元素
        auto max = _redis.zpopmax("score");
        if(max){
            std::cout<<"弹出最高分:"<<max->first<<":"<<max->second<<std::endl;
        }
        auto min = _redis.zpopmin("score");
        if(min){
            std::cout<<"弹出最低分:"<<min->first<<":"<<min->second<<std::endl;
        }
        Print(_redis,"score");

        //以权重得分删除指定区间元素
        std::cout<<"---------------------删除70~80的学生------------------"<<std::endl;
        auto interval1 = sw::redis::BoundedInterval<double>(70,80,sw::redis::BoundType::OPEN);
        _redis.zremrangebyscore("score",interval1);
        Print(_redis,"score");
        std::cout<<"---------------------删除数据完毕---------------"<<std::endl;

        _redis.zremrangebyrank("score",0,2);
        Print(_redis,"score");

        _redis.zincrby("score",10,"tianqi");
        Print(_redis,"score");
        _redis.zincrby("score",-20,"tianqi");
        Print(_redis,"score");
        _redis.del("score");
    }catch(sw::redis::Error& e){
        std::cout<<"Redis Err:"<<e.what()<<std::endl;
        abort();
    }
    return 0;
}
