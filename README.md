<div align="center">
  <h1>
    <img src="https://raw.githubusercontent.com/ZhouBaoChuan049/ImageOfBlog/main/CppSeed.jpg" width="200" alt="CppMicroSeed Logo"/>
    <br/>
    CppMicroSeed
  </h1>

  <p>
    一个面向 C++17 的微服务项目开发脚手架，帮助开发者快速构建规范、易于维护的微服务项目。
    <br/>
    A C++17 microservice project scaffolding tool for standardized and maintainable service development.
  </p>
  <p>
    <i>Build a solid foundation. Focus on what matters.</i>
  </p>

  <p>
    <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white" alt="C++17"/>
    <img src="https://img.shields.io/badge/CMake-Build-064F8C?logo=cmake&logoColor=white" alt="CMake"/>
    <img src="https://img.shields.io/badge/gflags-CLI-4285F4" alt="gflags"/>
    <img src="https://img.shields.io/badge/spdlog-Logging-333333" alt="spdlog"/>
    <img src="https://img.shields.io/badge/GoogleTest-Testing-4285F4?logo=googletest&logoColor=white" alt="GoogleTest"/>
    <img src="https://img.shields.io/badge/JsonCpp-JSON-333333" alt="JsonCpp"/>
    <img src="https://img.shields.io/badge/Protocol_Buffers-Serialization-4285F4?logo=google&logoColor=white" alt="Protobuf"/>
    <img src="https://img.shields.io/badge/FFmpeg-Media-007808?logo=ffmpeg&logoColor=white" alt="FFmpeg"/>
    <img src="https://img.shields.io/badge/HTTP-httplib-2C5F2D" alt="httplib"/>
    <img src="https://img.shields.io/badge/WebSocket-Communication-333333?logo=websocket&logoColor=white" alt="WebSocket"/>
    <img src="https://img.shields.io/badge/brpc-RPC-00599C" alt="brpc"/>
    <img src="https://img.shields.io/badge/RabbitMQ-Messaging-FF6600?logo=rabbitmq&logoColor=white" alt="RabbitMQ"/>
    <img src="https://img.shields.io/badge/AMQP--CPP-Client-FF6600" alt="AMQP-CPP"/>
    <img src="https://img.shields.io/badge/etcd-Service_Discovery-419EDA" alt="etcd"/>
    <img src="https://img.shields.io/badge/etcd--api--v3-Client-419EDA" alt="etcd-api-v3"/>
    <img src="https://img.shields.io/badge/MySQL-Database-4479A1?logo=mysql&logoColor=white" alt="MySQL"/>
    <img src="https://img.shields.io/badge/ODB-ORM-6A5ACD" alt="ODB"/>
    <img src="https://img.shields.io/badge/Redis-Cache-DC382D?logo=redis&logoColor=white" alt="Redis"/>
    <img src="https://img.shields.io/badge/redis%2B%2B-Client-DC382D" alt="redis++"/>
    <img src="https://img.shields.io/badge/FastDFS-File_Storage-4A5568" alt="FastDFS"/>
    <img src="https://img.shields.io/badge/fastdfs--api-Client-4A5568" alt="fastdfs-api"/>
    <img src="https://img.shields.io/badge/Elasticsearch-Search-005571?logo=elasticsearch&logoColor=white" alt="Elasticsearch"/>
    <img src="https://img.shields.io/badge/elasticlient-Client-005571" alt="elasticlient"/>
    <img src="https://img.shields.io/badge/Aliyun-SMS-FF6A00" alt="Aliyun SMS"/>
  </p>
</div>


## 一、项目简介

CppMicroSeed 是一个面向 C++17 的微服务项目开发脚手架，旨在整合 C++ 服务端开发过程中常用的基础组件、基础设施客户端及工程化实践，为后续项目开发提供可复用的基础能力。
项目不仅关注基础组件的功能实现，也注重组件封装、接口设计、基础设施集成以及实际运行环境中的功能验证。

## 二、快速开始（Quick Start）

### 2.1 环境要求

本项目的环境要求深度依赖该容器开发环境,具体部署细则详见[Microservices-Dev-Environment](https://github.com/ZhouBaoChuan049/Microservices-Dev-Environment)

- 开发环境及基础设施服务均通过预构建 Docker 镜像提供，无需在宿主机单独安装相关中间件。
- Docker Compose 负责统一编排和管理各个服务容器。
- 首次部署需要从 GHCR 拉取项目所需的 Docker 镜像。

### 2.2 获取项目

按照上述要求部署好环境后,进入dev-environment/dev-environment/workspace/,获取项目源码:

```bash
git clone https://github.com/ZhouBaoChuan049/CppMicroSeed.git
cd CppMicroSeed
```
### 2.3 编译静态库

在项目根目录下创建并进入 build 目录，执行 CMake 配置与编译：
```bash
mkdir build
cd build

cmake ..
cmake --build .
```
编译完成后，生成的静态库文件位于当前构建目录下的 lib/ 目录中。

### 2.4 安装静态库和头文件

在 build 目录下执行：
```bash
sudo make install
```
安装完成后，静态库和公共头文件将被安装到/usr/include和/usr/lib中。

### 2.5 卸载静态库和头文件

如果需要移除已经安装的静态库和头文件，可以在 build 目录下执行：
```bash
sudo make uninstall
```

## 三、项目目录结构

```text
CppMicroSeed/
├── Cmake/                          # CMake 配置及辅助模块
│   ├── cmake_uninstall.cmake.in
│   ├── FindDepends.cmake
│   └── MicroSeedConfig.cmake.in                          
├── Design                          # 项目设计文档及设计图
│   └── Brpc&Etcd.bmp                        
├── Example/                        # 第三方库及组件使用示例(该目录存放项目各个原生组件的使用示例及集成示例,主要用于展示原生的组件的实际使用方式，帮助开发者快速理解组件接口并将其应用于自己的项目。)
│   ├── Demo_AmqpCpp/
│   ├── Demo_Brpc/
│   │   ├── BrpcAsyncServerAndClient/
│   │   ├── BrpcHttpServerAndClient/
│   │   └── BrpcSimpleServerAndClient/
│   ├── Demo_ElasticSearch/
│   ├── Demo_Etcd/
│   ├── Demo_FFmpeg/
│   ├── Demo_Gflags/
│   ├── Demo_Gtest/
│   ├── Demo_Httplib/
│   ├── Demo_Jsoncpp/
│   ├── Demo_MailSending/
│   ├── Demo_Protobuf/
│   ├── Demo_Spdlog/
│   └── Demo_WebSocketpp/
├── Include/                         # 对外提供的公共头文件
│   ├── Brpc.h
│   ├── ElasticSearch.h
│   ├── EmailSend.h
│   ├── Etcd.h
│   ├── HLSTranscoder.h
│   ├── Log.h
│   └── Util.h
├── Source/                         # 各组件具体实现代码
│   ├── Brpc.cc
│   ├── ElasticSearch.cc
│   ├── EmailSend.cc
│   ├── Etcd.cc
│   ├── HLSTranscoder.cc
│   ├── Log.cc
│   └── Util.cc                          
├── Test/                             # 项目测试代码
│   ├── EnvironmentTest/              # 开发环境与生产环境测试
│   │   ├── DevEnvironmentTest/
│   │   └── ProdEnvironmentTest/
│   ├── IntegrationTestofBrpcAndEtcd/ # brpc 与 Etcd 联合测试
│   ├── TestMicroBrpc/                # brpc 组件测试
│   ├── TestMicroElasticSearch/       # Elasticsearch 组件测试
│   ├── TestMicroEmailSend/           # 邮件发送组件测试
│   ├── TestMicroEtcd/                # Etcd 组件测试
│   ├── TestMicroFFmpeg/              # FFmpeg 组件测试
│   ├── TestMicroLog/                 # 日志组件测试
│   └── TestMicroUtilJsoncpp/         # JSON 工具测试
├── CMakeLists.txt
```


## 四、核心组件一览


Include 和 Source 分别存放项目各组件对外提供的公共接口及其具体实现代码。项目按照功能划分组件，通过统一的接口封装底层第三方库及基础设施操作，降低业务代码对底层实现的依赖。

**目前主要封装了以下组件：**

| 组件名称 | 公共接口 | 实现文件 | 主要功能 |
|---|---|---|---|
| MicroSeedLog | Log.h | Log.cc | 封装spdlog,提供日志初始化、日志配置及日志输出能力。 |
| MicroSeedBrpc | Brpc.h | Brpc.cc | 封装Brpc库,提供Brpc远程过程调用客户端和服务器的搭建组件。 |
| MicroSeedElasticSearch | ElasticSearch.h | ElasticSearch.cc | 封装第三方elasticlient库提供ES客户端组件,对ES的请求正文做层层封装。 |
| MicroSeedEmailSend | EmailSend.h | EmailSend.cc | 封装libcurl库,提供邮件发送组件，封装邮件发送流程及相关通信功能。 |
| MicroSeedEtcd | Etcd.h | Etcd.cc | 封装第三方库etcd-cpp-apiv3, Etcd 客户端组件，提供键值操作、租约管理、KeepAlive 及 Watch 等功能。 |
| MicroSeedHLS | HLSTranscoder.h | HLSTranscoder.cc | 封装libffmpeg库,提供视频编解码,M3U8文件读写功能。 |
| MicroSeedUtil | Util.h | Util.cc | 小工具集合,提供序列与反序列化、文件读写、字符串分割与随机数生成能功能。 |

