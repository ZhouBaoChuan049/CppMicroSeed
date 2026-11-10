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

一、项目简介

CppMicroSeed 是一个面向 C++17 的微服务项目开发脚手架，旨在整合 C++ 服务端开发过程中常用的基础组件、基础设施客户端及工程化实践，为后续项目开发提供可复用的基础能力。

项目不仅关注基础组件的功能实现，也注重组件封装、接口设计、基础设施集成以及实际运行环境中的功能验证。

二、快速开始（Quick Start）

2.1 获取项目

git clone https://github.com/ZhouBaoChuan049/CppMicroSeed.git

cd CppMicroSeed

2.2 环境要求





项目

要求





操作系统

[待补充]





C++ 标准

C++17





编译器

[待补充]





构建工具

[待补充]





其他依赖

根据实际使用的组件确定

2.3 安装依赖

[待补充：第三方依赖的安装方式、版本要求及必要的环境配置。]

2.4 编译项目

[待补充：项目构建流程、CMake 配置及编译命令。]

2.5 运行与使用

[待补充：如何使用项目中的组件，以及如何运行对应的示例程序。]

三、项目目录结构

CppMicroSeed/
├── Design/                 # 项目设计文档与技术方案
├── Example/                # 组件使用示例与集成示例
├── Include/                # 组件公共头文件
│   ├── [组件目录 A]/
│   ├── [组件目录 B]/
│   └── ...
├── Source/                 # 组件具体实现
│   ├── [组件目录 A]/
│   ├── [组件目录 B]/
│   └── ...
└── Test/                   # 测试代码
    ├── [组件测试]
    ├── [集成测试]
    └── [环境测试]

3.1 Design

[介绍项目设计文档的组织方式，以及设计文档主要涵盖哪些内容。]

3.2 Example

[介绍各组件的使用示例、集成示例及运行方式。]

3.3 Include

该目录存放项目对外提供的组件接口及公共头文件。

子目录 / 文件

说明

[待补充]

[待补充]

3.4 Source

该目录存放各组件对应的具体实现代码。

子目录 / 文件

说明

[待补充]

[待补充]

3.5 Test

该目录存放项目各个层次的测试代码，包括组件测试、联合测试以及环境测试等。

具体测试体系将在后文介绍。

四、核心组件一览

本项目围绕 C++ 服务端开发中的常见需求，对相关功能进行统一封装。

以下表格按照组件进行整理，包含组件职责、底层依赖以及对应的头文件和源文件。

组件名称

主要作用

底层依赖库

头文件

源文件

[组件名称]

[主要作用]

[依赖库]

[Include 路径]

[Source 路径]

[组件名称]

[主要作用]

[依赖库]

[Include 路径]

[Source 路径]

[组件名称]

[主要作用]

[依赖库]

[Include 路径]

[Source 路径]

五、组件设计介绍

本章节主要介绍各个组件的设计思路、实现方式及关键技术。

重点展示组件封装过程中涉及的设计考量、接口抽象、基础设施集成以及关键问题的处理方式。

5.1 [组件名称]

设计目标

[介绍该组件主要解决的问题，以及封装该组件的原因。]

核心功能

[介绍组件提供的主要能力。]

设计与实现

[介绍核心接口、类结构、设计模式及关键实现思路。]

关键技术

[介绍组件涉及的关键技术，以及相关问题的解决方式。]

使用场景

[介绍该组件在实际项目中的使用场景。]

5.2 [组件名称]

[按照上述结构介绍。]

5.3 [组件名称]

[按照上述结构介绍。]

六、测试体系

CppMicroSeed 不仅对各个组件进行独立测试，同时也通过联合测试及环境测试，验证组件之间的协同能力和整体运行环境的可用性。

6.1 组件测试（Component Test）

[介绍如何针对每一个封装组件进行独立测试，包括测试目标、测试内容及测试方式。]

6.2 联合测试（Integration Test）

[介绍多个组件或基础设施之间的联合测试，例如 brpc 与 etcd 的集成测试。]

6.3 开发环境测试（Development Environment Test）

[介绍开发环境中的整体测试内容、测试流程及验证目标。]

6.4 生产环境测试（Production Environment Test）

[介绍生产环境中的整体测试内容、测试流程及与开发环境测试的区别。]

6.5 测试执行方式

[待补充：各类测试的执行命令、测试入口及相关配置。]

七、注意事项（Notes）

7.1 环境与依赖

[待补充：编译器、第三方依赖、基础设施版本及环境配置方面的注意事项。]

7.2 配置文件

[待补充：组件配置文件的位置、配置方式以及敏感信息管理要求。]

7.3 组件使用

[待补充：组件使用过程中需要注意的接口约束、初始化顺序及资源管理问题。]

7.4 其他说明

[待补充：目前已知的问题、限制及其他需要开发者留意的事项。]

