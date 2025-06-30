cmake_minimum_required(VERSION 3.5)
project(linker_hand_cpp_ros2)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# ----------------------------------------------------------------------
# 查找 liblinker_hand_lib.a 库
find_library(LINKER_HAND_LIB
    NAMES linker_hand_lib
    PATHS /usr/local/linker_hand_cpp_sdk/lib
    NO_DEFAULT_PATH
)
# 包含目录
include_directories(
  /usr/local/linker_hand_cpp_sdk/include
  include
)
# ----------------------------------------------------------------------
# 查找 RMAN_API_LIB 库
find_library(RMAN_API_LIB
    NAMES api_cpp
    PATHS /usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/lib
    NO_DEFAULT_PATH
)
# 包含目录
include_directories(
  /usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/include
)
# ----------------------------------------------------------------------
add_executable(linker_hand_node src/linker_hand_node.cpp)
target_include_directories(linker_hand_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(linker_hand_node rclcpp std_msgs sensor_msgs)

# 链接库
target_link_libraries(linker_hand_node ${LINKER_HAND_LIB} ${RMAN_API_LIB} pthread)


install(TARGETS linker_hand_node
  DESTINATION lib/${PROJECT_NAME})

install (DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

# install(DIRECTORY config
#   DESTINATION share/${PROJECT_NAME}
# )

# install(FILES config/setting.yaml
#   DESTINATION config
# )

set_target_properties(linker_hand_node PROPERTIES
    INSTALL_RPATH "/usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/lib"
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
