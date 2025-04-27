# LinkerHand-CPP-ROS2

## 概述
LinkerHand-CPP-ROS2 是灵心巧手科技有限公司开发，基于 LinkerHand-CPP-SDK 的ROS2封装版本。

#### 环境
[LinkerHand-CPP-SDK](../README.md)

#### 编译
    cd linker_hand_cpp_sdk/ros2/
    colcon build

#### 运行
    source install/setup.bash
    ros2 launch linker_hand_cpp_ros2/run.xml

#### 话题

| Topic | Type | Description |
| :--- | :--- | :--- |
| /cb_left_hand_control_cmd | sensor_msgs/msg/JointState | 左手控制指令 |
| /cb_left_hand_force | std_msgs/msg/Float32MultiArray | 左手力传感器数据 |
| /cb_left_hand_info | std_msgs/msg/String | 左手信息 |
| /cb_left_hand_state | sensor_msgs/msg/JointState | 左手关节状态 |
| /cb_right_hand_control_cmd | sensor_msgs/msg/JointState | 右手控制指令 |
| /cb_right_hand_force | std_msgs/msg/Float32MultiArray | 右手力传感器数据 |
| /cb_right_hand_info | std_msgs/msg/String | 右手信息 |
| /cb_right_hand_state | sensor_msgs/msg/JointState | 右手关节状态 |



