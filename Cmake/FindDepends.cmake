# =============================================================================
# FindDepends.cmake —— 本项目专用的“第三方依赖查找”子模块
#
# 作用：一个 C++ 项目通常会依赖很多第三方库(日志、RPC、网络、编解码等)。
#       CMake 本身不知道这些库装在哪里，需要我们告诉它去哪里找、找到后怎么用。
#       本文件就是干这件事的：
#         1. 列出一份“需要查找的库”名单(libs)；
#         2. 用一个宏(macro)逐个去系统里 find_library 查找；
#         3. 找到后把每个库包装成 imported target(导入目标)；
#         4. 用一个函数(function)遍历整个名单，最后统一链接到 MicroSeed 上。
#
# 本文件会被两处 include 加载：
#   1. 顶层 CMakeLists.txt                  —— 编译本项目自身时；
#   2. 安装后的 MicroSeedConfig.cmake       —— 别的项目 find_package(MicroSeed) 时。
# 所以它既要能被“构建本项目”使用，也要能被“使用本项目的下游项目”使用。
# =============================================================================


# -----------------------------------------------------------------------------
# 一、定义“需要查找的库”列表
#     list 里每个元素都是一个库名，后面会逐个去系统里查找。
#     库名就是链接命令 -l<name> 后面的那个 <name>，
#     例如 spdlog 对应 libspdlog.so，curl 对应 libcurl.so。
# -----------------------------------------------------------------------------
set(libs
    # ============ 日志模块 ============
    spdlog        # 高性能 C++ 日志库(对应 libspdlog)
    fmt           # spdlog 底层的格式化库(编译版 spdlog 依赖 fmt)

    # ============ RPC 框架 brpc 及其传递依赖 ============
    brpc          # 百度开源的 RPC 框架
    protobuf      # Protocol Buffers 序列化库(brpc 依赖，源码里也直接用了 google::protobuf)
    gflags        # 命令行参数解析库(brpc 的依赖)
    leveldb       # 键值存储库(brpc 的依赖)
    ssl           # OpenSSL 的 SSL/TLS 库(brpc 与邮件发送都依赖)
    crypto        # OpenSSL 的加密算法库
    dl            # 系统动态链接加载库 libdl
    pthread       # 系统 POSIX 线程库 libpthread

    # ============ 服务发现 etcd ============
    etcd-cpp-api  # etcd 的 C++ 客户端库
    cpprest       # 微软 C++ REST SDK(casablanca)，etcd 客户端依赖

    # ============ 搜索引擎 ElasticSearch ============
    elasticlient  # Elasticsearch 的 C++ 客户端库
    cpr           # 现代 C++ HTTP 请求库(elasticlient 依赖)
    jsoncpp       # JSON 解析/生成库

    # ============ 邮件发送 ============
    curl          # 底层 HTTP/SMTP 等协议传输库 libcurl

    # ============ 音视频转码 FFmpeg ============
    avcodec       # FFmpeg 编解码库
    avformat      # FFmpeg 封装/解封装库
    avutil        # FFmpeg 通用工具库

    # -------------------------------------------------------------------------
    # 【以下这些库当前项目暂时用不到，属于“以后要写”的功能，故先注释保留】
    # 等后续实现对应功能模块时，直接取消下面这几行的注释即可重新启用。
    # -------------------------------------------------------------------------
    # amqpcpp      # AMQP-CPP，RabbitMQ 消息队列客户端(目前只在 Example 演示里用)
    # ev           # libev 事件循环库，AMQP-CPP 的底层依赖
    # fdfsclient   # FastDFS 客户端，分布式文件存储
    # fastcommon   # FastDFS 的基础公共库
    # gtest        # GoogleTest 单元测试框架(属于 Test 目录，不属于核心库)
    # odb          # ODB，C++ 对象关系映射(ORM)框架核心库
    # odb-mysql    # ODB 的 MySQL 数据库驱动
    # odb-boost    # ODB 的 Boost 支持库
    # hiredis      # Redis 官方 C 客户端
    # redis++      # Redis 的 C++ 客户端封装
    # boost_system # Boost.System 库(当前源码没有直接使用)
)
# 小结：上面被注释掉的 11 个库(amqpcpp、ev、fdfsclient、fastcommon、gtest、
#       odb、odb-mysql、odb-boost、hiredis、redis++、boost_system)是暂时多余的，
#       删除(注释)它们是怕编译时系统里没装这些库而导致 find_library 失败报错。
#       等以后真的实现消息队列、FastDFS、Redis、ODB 等功能时再取消注释。


# -----------------------------------------------------------------------------
# 二、定义辅助宏 find_libraries(lib)
#     作用：查找一个名为 <lib> 的库；若找到，就把它包装成一个 imported target。
#
# 宏(macro)与函数(function)的区别(了解即可)：
#   宏相当于“文本替换”，调用处会把宏体直接展开，宏内变量会“泄漏”到调用处作用域；
#   函数则有自己独立的作用域。这里用宏是为了让 ${lib}_FOUND 等变量能被外部读到。
# -----------------------------------------------------------------------------
macro(find_libraries lib)
    # find_library：在系统指定路径里搜索某个库文件，把找到的“完整路径”存进一个变量。
    #   语法：find_library(<结果变量> <库名> [PATHS 搜索路径...])
    #   这里把结果存进 ${lib}_LIBRARY，例如查找 spdlog 后得到：
    #       spdlog_LIBRARY = /usr/lib/x86_64-linux-gnu/libspdlog.so
    find_library(${lib}_LIBRARY ${lib} PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)

    # if(<变量>)：如果变量非空(即上面真的找到了库文件)，就进入下面分支
    if(${lib}_LIBRARY)
        # 设置一个标志变量 ${lib}_FOUND = TRUE，表示“这个库找到了”
        set(${lib}_FOUND TRUE)

        # 防止重复创建：只有 MicroSeed::<lib> 这个目标还不存在时才去创建。
        # if(NOT TARGET ...) 里的 TARGET 是判断“目标是否已存在”的语法。
        if(NOT TARGET MicroSeed::${lib})
            # add_library(... INTERFACE IMPORTED)：
            #   创建一个“导入的、纯接口”目标。它自己不编译任何代码，
            #   只是用来代表一个外部第三方库，并记录“链接它时要加什么参数”。
            #   INTERFACE 表示它只携带“接口/链接信息”，没有实际构建产物。
            add_library(MicroSeed::${lib} INTERFACE IMPORTED)

            # set_target_properties：给上面这个 imported target 设置属性。
            #   INTERFACE_LINK_LIBRARIES 表示：谁链接了 MicroSeed::<lib>，
            #   谁的链接命令里就要自动加上 ${${lib}_LIBRARY} 这个库文件路径。
            set_target_properties(MicroSeed::${lib} PROPERTIES
                INTERFACE_LINK_LIBRARIES "${${lib}_LIBRARY}")

            # 打印一条状态信息，方便编译时看到“找到了哪个库、在什么路径”
            message(STATUS "找到依赖库: " ${lib} " - " ${${lib}_LIBRARY})
        endif()
    endif()
endmacro()


# -----------------------------------------------------------------------------
# 三、定义入口函数 find_my_depends()
#     作用：遍历 libs 列表，逐个查找所有库；全部找齐后统一链接到 MicroSeed。
#     顶层 CMakeLists.txt 和安装后的 MicroSeedConfig.cmake 都会调用它。
# -----------------------------------------------------------------------------
function(find_my_depends)
    # foreach(<循环变量> <列表>)：遍历列表，把每个元素依次放进循环变量 lib。
    foreach(lib ${libs})
        # 调用上面定义的宏，查找当前这个库并创建 imported target
        find_libraries(${lib})

        # 如果这个库没找到(${lib}_FOUND 不是 TRUE)，说明系统缺关键依赖
        if(NOT ${lib}_FOUND)
            # FATAL_ERROR：直接终止整个配置过程并报错，
            # 防止带着残缺依赖继续往下编译。
            message(FATAL_ERROR "丢失关键依赖库: ${lib}")
        endif()
    endforeach()

    # 把 libs 里所有库统一链接到目标上。
    #   PUBLIC 表示：这些库不仅 MicroSeed 自己用，
    #   任何链接了 MicroSeed 的下游目标也会自动获得它们(即“传递依赖”)。
    #
    # 注意这里用的是 ${PROJECT_NAME}：
    #   - 编译本项目时，PROJECT_NAME 是 MicroSeed，即给 MicroSeed 链接这些库；
    #   - 别人 find_package(MicroSeed) 时，PROJECT_NAME 是对方项目的名字，
    #     于是会把依赖链接到对方的目标上，从而把第三方依赖也“传递”给下游。
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libs})
endfunction()
