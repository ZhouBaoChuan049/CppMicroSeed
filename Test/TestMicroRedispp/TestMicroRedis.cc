#include "../../Include/Redispp.h"

int main()
{
    try {
        MicroSeedRedispp::RedisSettings settings = {
            .host = "192.168.65.128",
            .passwd = "123456",
            .connection_pool_size = 3
        };
        auto redis = MicroSeedRedispp::RedisFactory::create(settings);
        {
            // 事务中的命令先入队，调用 exec 时才真正提交执行
            auto tx = redis->transaction(false, false);
            tx.set("name", "zhangsan");
            tx.get("name");
            auto replies = tx.exec();
            // 1. 添加键值对
            bool ret = replies.get<bool>(0);
            if (!ret) {
                std::cout << "添加键值对失败" << std::endl;
                return -1;
            }
            // 2. 获取一个键值对
            auto val = replies.get<sw::redis::Optional<std::string>>(1);
            if (!val) {
                std::cout << "获取键值对失败" << std::endl;
                return -1;
            }
            std::cout << "name: " << *val << std::endl;
        }
        redis->flushall();
    }catch (const sw::redis::Error &e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}