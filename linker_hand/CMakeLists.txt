cmake_minimum_required(VERSION 3.5)
project(LinkerHandCppSDK)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# 添加可执行文件
add_executable(linker_hand_example src/example.cpp)

# 添加库
add_library(linker_hand_lib STATIC
    src/CanBus.cpp
    src/LinkerHandApi.cpp
    src/LinkerHandL10Can.cpp
    src/LinkerHandL20Can.cpp
)

# 链接库
target_link_libraries(linker_hand_example linker_hand_lib pthread)

# 安装规则
install(TARGETS linker_hand_lib
    DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)

install(FILES config/setting.yaml
    DESTINATION config
)

# 安装示例程序
install(TARGETS linker_hand_example
    DESTINATION bin
)