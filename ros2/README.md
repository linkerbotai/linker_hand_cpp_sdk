# LinkerHand-CPP-ROS2

## 概述
LinkerHand-CPP-ROS2 是灵心巧手科技有限公司开发，基于 LinkerHand-CPP-SDK 的ROS2封装版本。

#### 环境安装
[LinkerHand-CPP-SDK](../README.md)

#### 工程编译
    cd linker_hand_cpp_sdk/ros2/
    colcon build

#### 配置XML文件

```bash
<?xml version="1.0" encoding="utf-8"?>
<launch>
  <!-- 参数声明 -->
  <arg name="VERSION" default="1.1.6" description="版本号"/>

  <!-- 左手配置 -->
  <arg name="LEFT_HAND_EXISTS" default="true" description="是否存在左手"/>
  <arg name="LEFT_TOUCH" default="true" description="是否有压力传感器"/>
  <arg name="LEFT_JOINTS" default="L10" description="左手型号 L7 \ L10 \ L20 \ L21 \ L25"/>
  <arg name="LEFT_CANBUS" default="can0" description="can0 \ can1"/>

  <!-- 右手配置 -->
  <arg name="RIGHT_HAND_EXISTS" default="false" description="是否存在右手"/>
  <arg name="RIGHT_TOUCH" default="false" description="是否有压力传感器"/>
  <arg name="RIGHT_JOINTS" default="L10" description="右手型号 L7 \ L10 \ L20 \ L21 \ L25"/>
  <arg name="RIGHT_CANBUS" default="can0" description="can0 \ can1"/>

  <!-- 通用配置 -->
  <arg name="HAND_SPEED" default="100" description="关节速度 0 ~ 255"/>
  <arg name="HAND_EFFORT" default="200" description="关节扭矩 0 ~ 255"/>

  ......
  
</launch>
```

#### 运行
    source install/setup.bash
    ros2 launch linker_hand_cpp_ros2 run.xml


#### 话题

| 话题名称 | I/O | 消息类型 | 描述 |
| :--- | :--- | :--- | :--- |
| /left_hand_control | Input | sensor_msgs/msg/JointState | 左手控制指令 |
| /left_hand_settings | Input | sensor_msgs/msg/String | 左手设置指令 |
| /left_hand_touch | Output | std_msgs/msg/Float32MultiArray | 左手触觉传感器数据 |
| /left_hand_state | Output | sensor_msgs/msg/JointState | 左手关节状态 |
| /left_hand_info | Output | std_msgs/msg/String | 左手基本信息 |
| /right_hand_control | Input | sensor_msgs/msg/JointState | 左手控制指令 |
| /right_hand_settings | Input | sensor_msgs/msg/String | 右手设置指令 |
| /right_hand_touch | Output | std_msgs/msg/Float32MultiArray | 左手触觉传感器数据 |
| /right_hand_state | Output | sensor_msgs/msg/JointState | 左手关节状态 |
| /right_hand_info | Output | std_msgs/msg/String | 左手基本信息 |


针对以上话题的具体字段及其详细描述如下表所示：


- 控制关节话题 /left_hand_control

```bash
  $ ros2 topic echo /left_hand_control

  header: 
    seq: 256
    stamp: 
      secs: 1744343699
      nsecs: 232647418
    frame_id: ''
  name: []
  position: [155.0, 162.0, 176.0, 125.0, 255.0, 255.0, 180.0, 179.0, 181.0, 68.0]
  velocity: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  effort: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```
- position与手指关节对照表

```bash
L7:  ["大拇指弯曲", "大拇指横摆","食指弯曲", "中指弯曲", "无名指弯曲","小拇指弯曲","拇指旋转"]

L10: ["拇指根部", "拇指侧摆","食指根部", "中指根部", "无名指根部","小指根部","食指侧摆","无名指侧摆","小指侧摆","拇指旋转"]

L20: ["拇指根部", "食指根部", "中指根部", "无名指根部","小指根部","拇指侧摆","食指侧摆","中指侧摆","无名指侧摆","小指侧摆","拇指横摆","预留","预留","预留","预留","拇指尖部","食指末端","中指末端","无名指末端","小指末端"]

L21: ["大拇指根部", "食指根部", "中指根部","无名指根部","小拇指根部","大拇指侧摆","食指侧摆","中指侧摆","无名指侧摆","小拇指侧摆","大拇指横滚","预留","预留","预留","预留","大拇指中部","预留","预留","预留","预留","大拇指指尖","食指指尖","中指指尖","无名指指尖","小拇指指尖"]

L25: ["大拇指根部", "食指根部", "中指根部","无名指根部","小拇指根部","大拇指侧摆","食指侧摆","中指侧摆","无名指侧摆","小拇指侧摆","大拇指横滚","预留","预留","预留","预留","大拇指中部","食指中部","中指中部","无名指中部","小拇指中部","大拇指指尖","食指指尖","中指指尖","无名指指尖","小拇指指尖"]
```


---


- 设置指令话题 /left_hand_settings

```bash
  # 清除故障码
  $ ros2 topic pub /left_hand_setting std_msgs/msg/String "data: '{\"setting_cmd\": \"clear_faults\"}'"

  # 设置电流
  $ ros2 topic pub /left_hand_setting std_msgs/msg/String "data: '{\"setting_cmd\": \"set_electric_current\", \"params\": {\"electric_current\": 50}}'"

  # 使能
  $ ros2 topic pub /left_hand_setting std_msgs/msg/String "data: '{\"setting_cmd\": \"enable\"}'"

  # 失能
  $ ros2 topic pub /left_hand_setting std_msgs/msg/String "data: '{\"setting_cmd\": \"disable\"}'"

```


---
- 关节反馈话题 /left_hand_state

```bash
 $ ros2 topic echo /left_hand_state

  header: 
    seq: 256
    stamp: 
      secs: 1744343699
      nsecs: 232647418
    frame_id: ''
  name: []
  position: [155.0, 162.0, 176.0, 125.0, 255.0, 255.0, 180.0, 179.0, 181.0, 68.0]
  velocity: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
  effort: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
```

---

- 左手基本信息 /left_hand_info

```bash
$ ros2 topic echo /left_hand_info

data: 'Hand direction: Left hand

  Software Version: 7.0.0.0

  Hardware Version: 2.0.19.0

  Temperature: 47 49 40 41 0 39 0 46 42 0 0 39 0 49...'
```
- 压感数据 /left_hand_touch

  注意：反馈数据为一个一维数组，长度为360，由每个手指的压感数据组成。单个指头压感数据长度是72（6*12矩阵），每个手指压感数据需要单独拆分。（仅适用点阵式传感器）

```bash
$ ros2 topic echo /left_hand_touch 
layout:
  dim: []
  data_offset: 0
data:
- 0.0
- 0.0
- 0.0
- 0.0
- 0.0
- '...'
```

## 示例

| 序号 | 文件名称  | 描述                                           |
| :--- | :-------- | :--------------------------------------------- |
| 1    | examples/src/action_group_show_l7  | L7型号手指舞示例 |
| 2    | examples/src/action_group_show_l10  | L10型号手指舞示例 |
| 3    | examples/src/action_group_show_l20  | L20型号手指舞示例 |
| 4    | examples/src/loop_l20  | L20型号手势张开握拳示例 |
| 5    | examples/src/show_ok_l20  | L20型号手势ok示例 |
| 6    | examples/src/show_surround_index_finger_l20  | L20型号单指手势示例 |
| 7    | examples/src/show_wave_l20  | L20型号wave手势示例 |
| 8    | examples/src/action_group_show_l25  | L25型号手指舞示例 |
| 9    | examples/src/loop_l25  | L25型号手势张开握拳示例 |
