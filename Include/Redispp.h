#pragma once
#include <sw/redis++/redis.h>
#include <sw/redis++/queued_redis.h>
#include <cstddef>
#include <iostream>
#include <chrono>
#include <memory>
#include <string>
#include <unordered_set>

namespace MicroSeedRedispp {
    struct RedisSettings {
        int db = 0;
        int port = 6379;
        std::string host;
        std::string user = "default";
        std::string passwd;
        size_t connection_pool_size = 3;
    };
    class RedisFactory {
        public:
            static std::shared_ptr<sw::redis::Redis> create(const RedisSettings& settings);
    };
}