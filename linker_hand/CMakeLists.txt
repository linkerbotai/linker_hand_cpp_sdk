cmake_minimum_required(VERSION 3.5)
project(LinkerHandCppSDK)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找库
find_library(RMAN_API_LIB NAMES api_cpp PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Robotic_Arm/lib")

if(NOT RMAN_API_LIB)
    message(FATAL_ERROR "api_cpp library not found")
else()
    message(STATUS "RMAN_API_LIB: ${RMAN_API_LIB}")
    
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/Robotic_Arm/include")

endif()

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# 自动查找所有源文件
file(GLOB LINKER_HAND_SOURCES "src/*.cpp")

# 静态库
add_library(linker_hand_lib STATIC ${LINKER_HAND_SOURCES})
target_link_libraries(linker_hand_lib ${RMAN_API_LIB})

# 动态库
# add_library(linker_hand_lib_shared SHARED ${LINKER_HAND_SOURCES})
# target_link_libraries(linker_hand_lib_shared ${RMAN_API_LIB})

# 添加可执行文件
add_executable(Demo_ModbusRTU examples/Demo_ModbusRTU.cpp)
# 链接库
target_link_libraries(Demo_ModbusRTU linker_hand_lib pthread yaml-cpp ${RMAN_API_LIB})


# 添加可执行文件
add_executable(linker_hand_example examples/Examples.cpp)
# 链接库
target_link_libraries(linker_hand_example linker_hand_lib pthread yaml-cpp)

# 添加可执行文件
add_executable(test examples/test.cpp)
# 链接库
target_link_libraries(test linker_hand_lib pthread yaml-cpp)

# 安装规则
install(TARGETS linker_hand_lib
    DESTINATION linker_hand_cpp_sdk/lib
)

# install(TARGETS linker_hand_lib_shared
#     DESTINATION linker_hand_cpp_sdk/lib
# )

install(DIRECTORY include/
    DESTINATION linker_hand_cpp_sdk/include
)

install(DIRECTORY third_party/
    DESTINATION linker_hand_cpp_sdk/third_party
)

install(FILES config/setting.yaml
    DESTINATION linker_hand_cpp_sdk/config
)

# 安装示例程序
install(TARGETS linker_hand_example
    DESTINATION linker_hand_cpp_sdk/bin
)

install(TARGETS Demo_ModbusRTU
    DESTINATION linker_hand_cpp_sdk/bin
)

set_target_properties(Demo_ModbusRTU PROPERTIES
    INSTALL_RPATH "../third_party/Robotic_Arm/lib"
)