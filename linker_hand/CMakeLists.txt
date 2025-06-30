cmake_minimum_required(VERSION 3.10)
project(LinkerHandCppSDK)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
    message(STATUS "Detected Windows system")
    set(SYSTEM_ARCH "Windows")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Detected Unix/Linux system")
    set(SYSTEM_ARCH "Unix")
elseif(APPLE)
    message(STATUS "Detected macOS system")
    set(SYSTEM_ARCH "macOS")
else()
    message(WARNING "Unknown system architecture")
    set(SYSTEM_ARCH "Unknown")
endif()

if(WIN32)
find_library(PCAN_API_LIB NAMES PCANBasic PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/PCAN_Basic/x64/VC_LIB")
if(NOT PCAN_API_LIB)
    message(FATAL_ERROR "PCANBasic library not found")
else()
    message(STATUS "PCAN_API_LIB: ${PCAN_API_LIB}")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/PCAN_Basic/Include")
endif()
endif()

# 查找库
if(WIN32)
    find_library(RMAN_API_LIB NAMES api_cpp PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Robotic_Arm/windows/win_mingw64_c++_v1.1.0")
elseif(UNIX AND NOT APPLE)
    find_library(RMAN_API_LIB NAMES api_cpp PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Robotic_Arm/linux/lib")
endif()
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
if(WIN32)
target_link_libraries(linker_hand_lib ${RMAN_API_LIB} ${PCAN_API_LIB})
else()
    target_link_libraries(linker_hand_lib ${RMAN_API_LIB})
endif()

# 动态库
# add_library(linker_hand_lib_shared SHARED ${LINKER_HAND_SOURCES})
# target_link_libraries(linker_hand_lib_shared ${RMAN_API_LIB})


add_executable(Demo_ModbusRTU examples/Demo_ModbusRTU.cpp)
target_link_libraries(Demo_ModbusRTU linker_hand_lib pthread ${RMAN_API_LIB})

add_executable(linker_hand_example examples/Examples.cpp)
target_link_libraries(linker_hand_example linker_hand_lib pthread)

add_executable(test examples/test.cpp)
target_link_libraries(test linker_hand_lib pthread)

add_executable(action_group_show examples/L10/action_group_show.cpp)
target_link_libraries(action_group_show linker_hand_lib pthread)


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
