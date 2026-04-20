cmake_minimum_required(VERSION 3.5)
project(go2_demo)

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


include_directories(
  include
  include/common
  include/nlohmann
  include/devel)
link_directories(src)

set (
  DEPENDENCY_LIST
  unitree_go
  unitree_api
  rclcpp
  std_msgs
  rosbag2_cpp
  geometry_msgs
  sensor_msgs 
)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(unitree_go REQUIRED)
find_package(unitree_api REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosbag2_cpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(imu_publisher src/devel/imu_publisher.cpp)
add_executable(cmdvel_control src/devel/cmdvel_control.cpp src/common/ros2_sport_client.cpp)
add_executable(ros2serial src/common/ros2serial.cpp)

ament_target_dependencies(imu_publisher ${DEPENDENCY_LIST})
ament_target_dependencies(cmdvel_control ${DEPENDENCY_LIST})
ament_target_dependencies(ros2serial ${DEPENDENCY_LIST})

install(TARGETS 
        imu_publisher
        cmdvel_control
        DESTINATION lib/${PROJECT_NAME})


ament_package()
