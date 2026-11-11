# 1. 声明 CMake 最低版本
cmake_minimum_required(VERSION 3.1.3)

# 2. 设置工程项目名称
project(DevEnvironmentTest VERSION 1.0)

# 3. 编译标准：C++17
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# 4. 添加两个可执行目标：只编译测试自身的源码，
add_executable(Server ServiceProvider.cc Calculate.pb.cc)
add_executable(Client ServiceConsumer.cc Calculate.pb.cc)

# 5. 链接：已安装的静态库 libMicroSeed.a + 第三方依赖。
#    说明：
#      - <MicroSeed/xxx.h> 头文件已安装到 /usr/include/MicroSeed，走系统默认 include 即可找到；
#      - libMicroSeed.a 已安装到 /usr/lib，链接时通过 -lMicroSeed 找到；
#      - 静态库不会内嵌它的第三方依赖，所以测试程序仍需显式链接 brpc/protobuf/etcd 等。
target_link_libraries(Server PRIVATE
    MicroSeed brpc pthread protobuf fmt gflags spdlog leveldb jsoncpp dl ssl crypto cpprest etcd-cpp-api)
target_link_libraries(Client PRIVATE
    MicroSeed brpc pthread protobuf fmt gflags spdlog leveldb jsoncpp dl ssl crypto cpprest etcd-cpp-api)
