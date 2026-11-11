# =============================================================================
# CMakeLists.txt —— 本项目(MicroSeed)的顶层构建脚本
#
# 这个文件是 CMake 的“总指挥”，它告诉 CMake：
#   1. 用什么 CMake 版本、项目叫什么名字、版本号是多少；
#   2. 用哪些源文件编译出什么库(静态库还是动态库)；
#   3. 编译时要用 C++ 的哪个标准；
#   4. 去系统里查找哪些第三方依赖并链接进来；
#   5. 安装(install)时把哪些文件(头文件、库文件、配置)装到哪些位置，
#      好让别的项目以后能通过 find_package(MicroSeed) 来使用我们。
#
# CMake 的核心概念：CMake 里到处都是“命令”(command)，命令的格式是：
#   命令名(参数1 参数2 ...)
# 例如 project(MicroSeed VERSION 1.0) 中的 project 就是一个命令。
# 而 ${XXX} 表示“取变量 XXX 的值”。
# =============================================================================


# -----------------------------------------------------------------------------
# 1. 声明本项目要求的最低 CMake 版本
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1.3)
# 作用：如果用户本机的 CMake 版本低于 3.1.3，配置阶段会直接报错并停止。
# 因为后面的某些命令/特性需要这个版本才支持。它是一个“门槛”声明，必须写在最前面。


# -----------------------------------------------------------------------------
# 2. 声明项目名称与版本号
# -----------------------------------------------------------------------------
project(MicroSeed VERSION 1.0)
# 作用：告诉 CMake 这个项目的名字叫 MicroSeed，版本是 1.0。
# 执行完 project() 之后，CMake 会自动帮我们生成一堆内置变量，例如：
#   PROJECT_NAME        -> MicroSeed（项目名，之后大量使用）
#   PROJECT_VERSION     -> 1.0      （完整版本号）
#   PROJECT_VERSION_MAJOR -> 1      （主版本号）
#   PROJECT_SOURCE_DIR  -> 本项目 CMakeLists.txt 所在的源码目录
#   PROJECT_BINARY_DIR  -> 构建(编译)目录
# 所以后文里的 ${PROJECT_NAME} 就是 MicroSeed，${PROJECT_VERSION} 就是 1.0。


# -----------------------------------------------------------------------------
# 3. 固定安装前缀为 /usr
# -----------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "安装前缀，固定为 /usr" FORCE)
# CMAKE_INSTALL_PREFIX：安装前缀。install(...) 里那些相对 DESTINATION 的路径，
# 最终都会拼在这个前缀后面。CMake 默认是 /usr/local，所以之前装到了 /usr/local。
# 这里强制改成 /usr，于是下次 make install 时会装到：
#   头文件 -> /usr/include/MicroSeed
#   静态库 -> /usr/lib/libMicroSeed.a
#   cmake  -> /usr/lib/cmake/MicroSeed
# FORCE 表示强制覆盖：即使命令行传了 -DCMAKE_INSTALL_PREFIX=xxx，也以这里 /usr 为准。


# -----------------------------------------------------------------------------
# 4. 收集源文件并创建库目标
# -----------------------------------------------------------------------------
file(GLOB_RECURSE BASE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cc)
# file(GLOB_RECURSE ...)：
#   作用是在文件系统里“按通配符收集文件”，把结果保存到第一个参数(BASE_FILES)里。
#   GLOB_RECURSE 表示“递归查找”，会进入子目录一层层往下找。
#   这里的意思是：递归收集 Source 目录下所有扩展名为 .cc 的源文件，存进 BASE_FILES 变量。
#
# ${CMAKE_CURRENT_SOURCE_DIR}：
#   指“当前正在处理的 CMakeLists.txt 所在的源码目录”，
#   也就是本项目根目录(/home/dev/workspace/CppMicroSeed)。
#   注意：本项目的源码目录是 Source(首字母大写)，所以这里写成 /Source/*.cc。

add_library(${PROJECT_NAME} STATIC ${BASE_FILES})
# add_library：创建一个“库目标”(target)。
#   ${PROJECT_NAME}   -> 目标名字，也就是 MicroSeed；
#   STATIC            -> 生成“静态库”(.a 文件)；
#   ${BASE_FILES}     -> 用上面收集到的那些 .cc 文件来编译。
# 这句话执行后，CMake 就知道：“我要把 Source 下所有 .cc 编译成一个名叫 MicroSeed 的静态库”。


# -----------------------------------------------------------------------------
# 4. 指定编译特性：使用 C++17
# -----------------------------------------------------------------------------
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# target_compile_features：给某个目标设置“编译特性”。
#   PUBLIC   表示“这个要求既用于我自己，也会传递给依赖我的目标”。
#   cxx_std_17 表示要求编译器启用 C++17 标准。
# 翻译成大白话：编译 MicroSeed 时用 C++17；以后谁链接了 MicroSeed，也自动按 C++17 来。


# -----------------------------------------------------------------------------
# 5. 设置库目标的属性(输出位置、版本信息)
# -----------------------------------------------------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
# set_target_properties：给某个目标设置属性(一个键值对)。
#   ARCHIVE_OUTPUT_DIRECTORY：静态库(“归档文件” .a)的输出目录。
#   ${CMAKE_CURRENT_BINARY_DIR}：指“当前执行 cmake 时对应的构建目录”，
#                               即编译产物最终生成到哪里。
# 翻译：把生成出来的 libMicroSeed.a 放到 构建目录/lib 下面。

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})
# 这里设置版本相关属性：
#   VERSION    -> 完整版本号(1.0)
#   SOVERSION  -> 主版本号(1)
# 说明：VERSION / SOVERSION 这两个属性主要对“动态库(.so)”有意义，
#       会给文件名加上版本后缀(如 libMicroSeed.so.1.0)。
#       对于静态库(.a)它们基本不起实际作用，这里沿用模板写法，无害保留。
#       写它们是为了将来如果改成动态库，版本信息已经备好。

# -----------------------------------------------------------------------------
# 6. 依赖检查：加载我们的依赖查找子模块
# -----------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Cmake)
# list(APPEND ...)：往某个 list 变量里追加内容。
#   CMAKE_MODULE_PATH 是 CMake 内置的“模块搜索路径”列表，
#   当用 include(xxx.cmake) 时，CMake 会到这个列表里的目录去找 xxx.cmake。
# 这里把本项目的 Cmake 目录(首字母大写)追加进去，
# 这样下面 include(FindDepends) 才能找到我们的 FindDepends.cmake。

include(FindDepends)
# include(...)：读取并执行一个 .cmake 文件(相当于 C++ 里的 #include)。
# 这里把我们自己写的 FindDepends.cmake(依赖查找子模块)加载进来，
# 加载后，该文件里定义的变量、宏(macro)、函数(function)就可以用了。

#    1. 下面这行是在“调用”子模块里定义好的函数：
find_my_depends()
# find_my_depends() 是我们在 FindDepends.cmake 里自定义的函数，
# 它的工作是：遍历第三方库列表 -> 逐个 find_library 查找 ->
# 找到后包装成 imported target -> 最后统一链接到 MicroSeed 上。
# 具体实现见 Cmake/FindDepends.cmake。


# -----------------------------------------------------------------------------
# 7. 安装头文件
# -----------------------------------------------------------------------------
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Include/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h")
# install(...)：定义一条“安装规则”。当执行 cmake --install 或 make install 时，
#               会把指定文件复制到安装目录(DESTINATION)。
#   DIRECTORY <目录>          : 要安装的目录(本项目头文件在 Include/ 目录)。
#   DESTINATION <目标路径>    : 装到哪里。这里装到 include/MicroSeed 下。
#   FILES_MATCHING PATTERN "*.h" : 只安装该目录下扩展名为 .h 的文件(过滤掉 .cc 等)。
# 翻译：把 Include 目录下所有 .h 头文件安装到 <安装前缀>/include/MicroSeed/。


# -----------------------------------------------------------------------------
# 8. 安装库文件，并声明导出信息
# -----------------------------------------------------------------------------
install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include/${PROJECT_NAME})
# install(TARGETS ...)：安装某个“目标”(这里就是 MicroSeed 这个库)。
#   EXPORT ${PROJECT_NAME}Targets : 把这个目标登记到名为 "MicroSeedTargets" 的导出集合里，
#                                   后面第 10 步会据此生成 MicroSeedTargets.cmake，
#                                   供别的项目 find_package 时导入使用。
#   ARCHIVE DESTINATION lib       : 静态库(.a)安装到 lib 目录。
#   INCLUDES DESTINATION include/MicroSeed : 记录头文件位置，作为该目标的接口包含目录。


# -----------------------------------------------------------------------------
# 9. 安装依赖查找子模块
# -----------------------------------------------------------------------------
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/
    DESTINATION lib/cmake/${PROJECT_NAME}/modules
    FILES_MATCHING PATTERN "Find*.cmake")
# 把我们的 Cmake 目录下、名字以 Find 开头且以 .cmake 结尾的文件
# (也就是 FindDepends.cmake)安装到 lib/cmake/MicroSeed/modules/ 下。
# 这样以后别的项目 find_package(MicroSeed) 时，Config 文件还能找到并 include 它。


# -----------------------------------------------------------------------------
# 10. 导出目标信息，生成 <项目名>Targets.cmake
# -----------------------------------------------------------------------------
install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::)
# install(EXPORT ...)：把第 8 步登记到 "MicroSeedTargets" 导出集合里的目标，
#                      生成一个 MicroSeedTargets.cmake 文件。
#   FILE 生成的文件名    -> MicroSeedTargets.cmake
#   DESTINATION 安装位置 -> lib/cmake/MicroSeed/
#   NAMESPACE 命名空间   -> 所有导出的目标名字前加 "MicroSeed::" 前缀，
#                           即别的项目使用时，目标叫 MicroSeed::MicroSeed。


# -----------------------------------------------------------------------------
# 11. 处理 Config.cmake.in 模板，生成最终配置文件
# -----------------------------------------------------------------------------
include(CMakePackageConfigHelpers)
# CMakePackageConfigHelpers 是 CMake 自带的一个模块，
# 提供两个非常常用的函数：
#   configure_package_config_file()   —— 处理 Config 模板(下面第 12 步用)
#   write_basic_package_version_file() —— 生成版本配置文件(第 13 步用)
# 它给“制作一个可以被 find_package 找到的包”提供了标准做法。

# 12. 处理 Config.cmake.in 模板
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})
# configure_package_config_file：把一个 .in 模板“渲染”成最终的 Config 文件。
#   第 1 个参数：模板文件的输入路径。
#               ${PROJECT_NAME}Config.cmake.in 展开后就是
#               Cmake/MicroSeedConfig.cmake.in。
#   第 2 个参数：生成结果写到哪个文件(这里写到构建目录下的 MicroSeedConfig.cmake)。
#   INSTALL_DESTINATION：告诉 CMake 这个 Config 最终会被安装到哪里，
#                        用来正确计算“包的前缀路径”。
# 它会把模板里的 @PACKAGE_INIT@ 等占位符替换成真正的代码。


# -----------------------------------------------------------------------------
# 13. 生成版本配置文件
# -----------------------------------------------------------------------------
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
# write_basic_package_version_file：自动生成 MicroSeedConfigVersion.cmake，
#   里面记录了本包的版本号，并定义了版本匹配规则。
#   COMPATIBILITY SameMajorVersion 表示：只要“主版本号相同”就算兼容。
#   例如我们版本是 1.0，那么请求 1.x 的 find_package 都会被接受。


# -----------------------------------------------------------------------------
# 14. 安装最终的配置文件
# -----------------------------------------------------------------------------
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})
# 把上面两步生成的两个文件：
#   MicroSeedConfig.cmake         （包的主配置）
#   MicroSeedConfigVersion.cmake  （版本配置）
# 一起安装到 lib/cmake/MicroSeed/ 下。
# 这样别的项目只要在 CMAKE_PREFIX_PATH 里指向我们的安装前缀，
# 就可以用 find_package(MicroSeed) 找到并使用我们了。


# -----------------------------------------------------------------------------
# 15. 卸载目标(uninstall)
# -----------------------------------------------------------------------------
# 把 cmake_uninstall.cmake.in 模板渲染成构建目录下的 cmake_uninstall.cmake。
# @ONLY 表示只替换 @VAR@ 形式的占位符，模板里的 ${...} 保持原样不展开。
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY)

# 注册一个 uninstall 目标。用户执行：
#   cmake --build build --target uninstall
# 或 make uninstall
# 就会运行上面生成的脚本，按 install_manifest.txt 逐条删除已安装的文件。
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
