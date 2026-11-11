 <div align="center">
  <h1>
    <img src="https://raw.githubusercontent.com/ZhouBaoChuan049/ImageOfBlog/main/CppSeed.jpg" width="200" alt="CppMicroSeed Logo"/>
    <br/>
    CppMicroSeed
  </h1>

  <p>
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

<p align="right">
  <a href="./README.md">简体中文</a> |
  <a href="./README_EN.md">English</a> |
  <a href="./README_JP.md">日本語</a>
</p>

## 1. Project Introduction

CppMicroSeed is a C++17 microservice project scaffolding tool designed to integrate commonly used fundamental components, infrastructure clients, and engineering practices in C++ server-side development. It provides reusable foundational capabilities for subsequent project development.

The project focuses not only on implementing individual components, but also on component encapsulation, interface design, infrastructure integration, and functional verification in actual runtime environments.

## 2. Quick Start

### 2.1 Requirements

The project environment relies heavily on the containerized development environment. For detailed deployment instructions, please refer to [Microservices-Dev-Environment](https://github.com/ZhouBaoChuan049/Microservices-Dev-Environment).

- The development environment and infrastructure services are provided through pre-built Docker images. There is no need to install the corresponding middleware separately on the host machine.
- Docker Compose is used to orchestrate and manage all service containers.
- On the first deployment, the required Docker images must be pulled from GHCR.

### 2.2 Clone the Repository

After deploying the development environment as described above, navigate to the workspace directory and clone the repository:

```bash
git clone https://github.com/ZhouBaoChuan049/CppMicroSeed.git
cd CppMicroSeed
```

### 2.3 Build the Static Library

Create and enter a `build` directory in the project root, then run the following CMake commands:

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

After compilation, the generated static library will be located in the `lib/` directory under the build directory.

### 2.4 Install the Static Library and Header Files

Run the following command from the `build` directory:

```bash
sudo make install
```

After installation, the static library and public header files will be installed into `/usr/lib` and `/usr/include`, respectively.

### 2.5 Uninstall the Static Library and Header Files

To remove the installed static library and header files, run the following command from the `build` directory:

```bash
sudo make uninstall
```

## 3. Project Structure

```text
CppMicroSeed/
├── Cmake/                          # CMake configuration and helper modules
│   ├── cmake_uninstall.cmake.in
│   ├── FindDepends.cmake
│   └── MicroSeedConfig.cmake.in
├── Design/                         # Design documents and diagrams
│   └── Brpc&Etcd.bmp
├── Example/                        # Usage examples for native components and third-party libraries
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
├── Include/                         # Public header files exposed to external projects
│   ├── Brpc.h
│   ├── ElasticSearch.h
│   ├── EmailSend.h
│   ├── Etcd.h
│   ├── HLSTranscoder.h
│   ├── Log.h
│   └── Util.h
├── Source/                          # Component implementation files
│   ├── Brpc.cc
│   ├── ElasticSearch.cc
│   ├── EmailSend.cc
│   ├── Etcd.cc
│   ├── HLSTranscoder.cc
│   ├── Log.cc
│   └── Util.cc
├── Test/                            # Project test code
│   ├── EnvironmentTest/              # Development and production environment tests
│   │   ├── DevEnvironmentTest/
│   │   └── ProdEnvironmentTest/
│   ├── IntegrationTestofBrpcAndEtcd/ # brpc and etcd integration tests
│   ├── TestMicroBrpc/                # brpc component tests
│   ├── TestMicroElasticSearch/       # Elasticsearch component tests
│   ├── TestMicroEmailSend/           # Email sending component tests
│   ├── TestMicroEtcd/                # etcd component tests
│   ├── TestMicroFFmpeg/              # FFmpeg component tests
│   ├── TestMicroLog/                 # Logging component tests
│   └── TestMicroUtilJsoncpp/         # JSON utility tests
├── CMakeLists.txt
└── README.md
```

## 4. Core Components

The `Include` and `Source` directories contain the public interfaces and corresponding implementations of the project's components, respectively.

The project organizes its components by functionality and encapsulates underlying third-party libraries and infrastructure operations behind unified interfaces, reducing the dependency of application code on low-level implementations.

**The main components currently provided by the project are:**

| Component | Public Header | Implementation File | Description |
|---|---|---|---|
| MicroSeedLog | Log.h | Log.cc | Encapsulates spdlog and provides logging initialization, configuration, and output capabilities. |
| MicroSeedBrpc | Brpc.h | Brpc.cc | Encapsulates the brpc framework and provides reusable components for building RPC clients and servers. |
| MicroSeedElasticSearch | ElasticSearch.h | ElasticSearch.cc | Encapsulates the third-party elasticlient library and provides an Elasticsearch client with simplified request construction and API operations. |
| MicroSeedEmailSend | EmailSend.h | EmailSend.cc | Encapsulates libcurl and provides email sending functionality, including the sending workflow and related communication operations. |
| MicroSeedEtcd | Etcd.h | Etcd.cc | Encapsulates the third-party etcd-cpp-apiv3 library and provides etcd key-value operations, lease management, KeepAlive, and Watch functionality. |
| MicroSeedHLS | HLSTranscoder.h | HLSTranscoder.cc | Encapsulates FFmpeg and provides video encoding and decoding, as well as M3U8 file processing. |
| MicroSeedUtil | Util.h | Util.cc | A collection of utility functions providing serialization and deserialization, file I/O, string splitting, and random number generation. |
