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

| 话题名称 | I/O | 消息类型 | 描述 |
| :--- | :--- | :--- | :--- |
| /cb_left_hand_control_cmd | Input | sensor_msgs/msg/JointState | 左手控制指令 |
| /cb_left_hand_force | Output | std_msgs/msg/Float32MultiArray | 左手力传感器数据 |
| /cb_left_hand_info | Output | std_msgs/msg/String | 左手信息 |
| /cb_left_hand_state | Output | sensor_msgs/msg/JointState | 左手关节状态 |
| /cb_right_hand_control_cmd | Input | sensor_msgs/msg/JointState | 右手控制指令 |
| /cb_right_hand_force | Output | std_msgs/msg/Float32MultiArray | 右手力传感器数据 |
| /cb_right_hand_info | Output | std_msgs/msg/String | 右手信息 |
| /cb_right_hand_state | Output | sensor_msgs/msg/JointState | 右手关节状态 |


针对以上话题的具体字段及其详细描述如下表所示：


- 控制关节话题：ros2 topic echo /cb_left_hand_control_cmd

```bash
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

```
  L7:  ["大拇指弯曲", "大拇指横摆","食指弯曲", "中指弯曲", "无名指弯曲","小拇指弯曲","拇指旋转"]

  L10: ["拇指根部", "拇指侧摆","食指根部", "中指根部", "无名指根部","小指根部","食指侧摆","无名指侧摆","小指侧摆","拇指旋转"]

  L20: ["拇指根部", "食指根部", "中指根部", "无名指根部","小指根部","拇指侧摆","食指侧摆","中指侧摆","无名指侧摆","小指侧摆","拇指横摆","预留","预留","预留","预留","拇指尖部","食指末端","中指末端","无名指末端","小指末端"]
  
  L21: ["大拇指根部", "食指根部", "中指根部","无名指根部","小拇指根部","大拇指侧摆","食指侧摆","中指侧摆","无名指侧摆","小拇指侧摆","大拇指横滚","预留","预留","预留","预留","大拇指中部","预留","预留","预留","预留","大拇指指尖","食指指尖","中指指尖","无名指指尖","小拇指指尖"]

  L25: ["大拇指根部", "食指根部", "中指根部","无名指根部","小拇指根部","大拇指侧摆","食指侧摆","中指侧摆","无名指侧摆","小拇指侧摆","大拇指横滚","预留","预留","预留","预留","大拇指中部","食指中部","中指中部","无名指中部","小拇指中部","大拇指指尖","食指指尖","中指指尖","无名指指尖","小拇指指尖"]
```


---


- 灵巧手信息话题：ros2 topic echo /cb_left_hand_info

```bash
data: 'Hand direction: Left hand

  Software Version: 7.0.0.0

  Hardware Version: 2.0.19.0

  Temperature: 47 49 40 41 0 39 0 46 42 0 0 39 0 49...'
```

- 关节反馈话题 ros2 topic echo /cb_left_hand_state

```bash
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

<!-- 
<table>
<tbody>
    <tr>
        <th>话题名称</th><th>字段</th><th>描述</th>
    <tr>
        <td rowspan="4">/cb_left_hand_control_cmd</td>
        <td>header</td>
        <td>标准消息头</td>
    </tr>
    <tr>
        <td>position</td>
        <td>[Joint1_position, Joint2_position, Joint3_position, Joint4_position, Joint5_position, Joint6_position, Joint7_position . . . . . .]</td>
    </tr>
    <tr>
        <td>velocity</td>
        <td>[Joint1_velocity, Joint2_velocity, Joint3_velocity, Joint4_velocity, Joint5_velocity, Joint6_velocity, Joint7_velocity . . . . . .]</td>
    </tr>
    <tr>
        <td>effort</td>
        <td>[Joint1_effort, Joint2_effort, Joint3_effort, Joint4_effort, Joint5_effort, Joint6_effort, Joint7_effort . . . . . .]</td>
    </tr>
    <tr>
        <td rowspan="2">/cb_left_hand_force</td>
        <td>header</td>
        <td>标准消息头</td>
    </tr>
    <tr>
        <td>data</td>
        <td>[Joint1_force, Joint2_force, Joint3_force, Joint4_force, Joint5_force . . . . . .]</td>
    </tr>
    <tr>
        <td rowspan="2">/cb_left_hand_info</td>
        <td>header</td>
        <td>标准消息头</td>
    </tr>
    <tr>
        <td>data</td>
        <td>[Joint1_force, Joint2_force, Joint3_force, Joint4_force, Joint5_force . . . . . .]</td>
    </tr>
    <tr>
        <td>行1列1</td>
        <td>行1列2</td>
        <td>行1列3</td>
    </tr>
    <tr>
        <td rowspan="2">合并两列</td>
        <td colspan="2">合并两行</td>
    </tr>
    <tr>
        <td>行3列2</td>
        <td>行3列3</td>
    </tr>
</table> -->