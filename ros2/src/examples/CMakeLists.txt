cmake_minimum_required(VERSION 3.5)
project(examples)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Enable additional compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# Define common include directories
set(COMMON_INCLUDE_DIRS
  /usr/local/linker_hand_cpp_sdk/include
  /usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/include
  include
)

# Include directories
include_directories(${COMMON_INCLUDE_DIRS})

# Find linker_hand_lib
find_library(LINKER_HAND_LIB
  NAMES linker_hand_lib
  PATHS /usr/local/linker_hand_cpp_sdk/lib
  NO_DEFAULT_PATH
)

# Find api_cpp
find_library(API_CPP_LIB
  NAMES api_cpp
  PATHS /usr/local/linker_hand_cpp_sdk/third_party/Robotic_Arm/lib
  NO_DEFAULT_PATH
)

# Define common link libraries
set(COMMON_LIBS ${LINKER_HAND_LIB} ${API_CPP_LIB} pthread)

# Define source files
set(SOURCE_FILES
  action_group_show_l7
  action_group_show_l10
  action_group_show_l20
  action_group_show_l25
  loop_l20
  loop_l25
  show_surround_index_finger_l20
  show_ok_l20
  show_wave_l20
)

# Create and name a shared library
foreach(SOURCE_FILE ${SOURCE_FILES})
  add_executable(${SOURCE_FILE} src/${SOURCE_FILE}.cpp)
  target_include_directories(${SOURCE_FILE} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  ament_target_dependencies(${SOURCE_FILE}
    rclcpp
    std_msgs
    sensor_msgs
  )
  target_link_libraries(${SOURCE_FILE} ${COMMON_LIBS})
  install(TARGETS ${SOURCE_FILE}
    DESTINATION lib/${PROJECT_NAME}
  )
endforeach()

# Install launch files
# install(DIRECTORY launch
#   DESTINATION share/${PROJECT_NAME}/
# )

# Install configuration files
# install(DIRECTORY config
#   DESTINATION share/${PROJECT_NAME}/
# )

# Install settings file
# install(FILES config/setting.yaml
#   DESTINATION config
# )

# If BUILD_TESTING is enabled, include test-related dependencies
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

# Package information
ament_package()