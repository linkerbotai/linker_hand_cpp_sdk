# CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(MyProject)

# 包含目录
include_directories(/usr/local/linker_hand_cpp_sdk/include)
include_directories(/usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/include)

# 查找 LINKER_HAND_LIB 库
find_library(LINKER_HAND_LIB
    NAMES linker_hand_lib
    PATHS /usr/local/linker_hand_cpp_sdk/lib
    NO_DEFAULT_PATH
)

# 检查是否找到库
if(NOT LINKER_HAND_LIB)
    message(FATAL_ERROR "liblinker_hand_lib.a not found!")
else()
    message(STATUS "liblinker_hand_lib.a found at ${LINKER_HAND_LIB}")
endif()




# 查找 RMAN_API_LIB 库
find_library(RMAN_API_LIB
    NAMES api_cpp
    PATHS /usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/lib
    NO_DEFAULT_PATH
)

# 检查是否找到库
if(NOT RMAN_API_LIB)
    message(FATAL_ERROR "libapi_cpp.so not found!")
else()
    message(STATUS "liblinker_hand_lib.a found at ${RMAN_API_LIB}")
endif()


# 添加可执行文件
add_executable(ModbusRTU main.cpp)

# 链接库
target_link_libraries(ModbusRTU ${LINKER_HAND_LIB} ${RMAN_API_LIB} pthread)
