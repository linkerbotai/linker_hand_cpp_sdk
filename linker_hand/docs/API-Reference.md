
---

# Linker Hand C++ API Documentation

## API Overview

This document provides a detailed overview of the C++ API for the Linker Hand, including functions for controlling the hand's movements, retrieving sensor data, and setting operational parameters.

## Public API

### 设置关节位置
```cpp
void fingerMove(const std::vector<uint8_t> &pose);
```
**Description**:  
设置关节的目标位置，用于控制手指的运动。  
**Parameters**:  
- `pose`: 一个包含目标位置数据的 `std::vector<uint8_t>`，长度为自由度型号（L10=10,L20=20）。

---

### 设置速度
```cpp
void setSpeed(const std::vector<uint8_t> &speed);
```
**Description**:  
设置手部的运动速度。  
**Parameters**:  
- `speed`: 一个包含速度数据的 `std::vector<uint8_t>`，长度为5个元素对应每手指的速度值。

---

### 设置扭矩
```cpp
void setTorque(const std::vector<uint8_t> &torque);
```
**Description**:  
设置手部的扭矩。  
**Parameters**:  
- `torque`: 一个包含扭矩数据的 `std::vector<uint8_t>`，长度为5个元素对应手指的扭矩值。

---

### 获取法向压力、切向压力、切向方向、接近感应
```cpp
std::vector<std::vector<uint8_t>> getForce();
```
**Description**:  
获取手部传感器的综合数据，包括法向压力、切向压力、切向方向和接近感应。  
**Returns**:  
- 返回一个二维向量，其中每个子向量包含不同类型的压力数据。

---

### 获取速度
```cpp
std::vector<uint8_t> getSpeed();
```
**Description**:  
获取当前设置的速度值。  
**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含当前的速度设置值。

---

### 获取当前关节状态
```cpp
std::vector<uint8_t> getState();
```
**Description**:  
获取当前关节的状态信息。  
**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含当前关节的状态数据。

---

### 获取大拇指、食指、中指、无名指、小指的所有压力数据
```cpp
std::vector<std::vector<uint8_t>> getPressure();
```
**Description**:  
获取五根手指的所有压力数据，按手指分类返回。  
**Returns**:  
- 返回一个二维向量，其中每个子向量对应一根手指的压力数据。

---

### 获取版本号
```cpp
std::string getVersion();
```
**Description**:  
获取当前软件或硬件的版本号。  
**Returns**:  
- 返回一个字符串，表示当前的版本号。

---

### 获取电机温度
```cpp
std::vector<uint8_t> getMotorTemperature();
```
**Description**:  
获取电机的温度数据。  
**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含电机的温度数据。

---

### 获取电机故障码
```cpp
std::vector<uint8_t> getMotorFaultCode();
```
**Description**:  
获取电机的故障码。  
**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含电机的故障码。

---

### 获取当前电流
```cpp
std::vector<uint8_t> getMotorCurrent();
```
**Description**:  
获取电机的当前电流值。  
**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含电机的当前电流值。

---

### 获取当前最大扭矩
```cpp
std::vector<uint8_t> getMotorTorque();
```
**Description**:  
获取电机的当前最大扭矩值。  
**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含电机的当前最大扭矩值。

---

### 设置电流（目前仅支持 L20）
```cpp
void setMotorCurrent(const std::vector<uint8_t> &current);
```
**Description**:  
设置电机的电流值。  
**Parameters**:  
- `current`: 一个包含电流数据的 `std::vector<uint8_t>`，每个元素对应一个电机的电流值。

---

### 设置电机使能（目前仅支持 L25）
```cpp
void setMotorEnable(const std::vector<uint8_t> &enable);
```
**Description**:  
使能电机。  
**Parameters**:  
- `enable`: 一个包含使能数据的 `std::vector<uint8_t>`，每个元素对应一个电机的使能状态。

---

### 设置电机禁用（目前仅支持 L25）
```cpp
void setMotorDisable(const std::vector<uint8_t> &disable);
```
**Description**:  
禁用电机。  
**Parameters**:  
- `disable`: 一个包含禁用数据的 `std::vector<uint8_t>`，每个元素对应一个电机的禁用状态。

---

### 清除电机故障码（目前仅支持 L20）
```cpp
void clearMotorFaultCode();
```
**Description**:  
清除电机的故障码。

---

## Example Usage

以下是一个完整的示例代码，展示如何使用上述 API：

```cpp
#include "LinkerHandApi.h"

int main() {
    
    // 调用API接口
    LinkerHandApi hand(LINKER_HAND::L10, HAND_TYPE::RIGHT);

    // 获取版本信息
    std::cout << hand.getVersion() << std::endl;

    // 握拳
    std::vector<uint8_t> fist_pose = {101, 60, 0, 0, 0, 0, 255, 255, 255, 51};
    hand.fingerMove(fist_pose);
	std::this_thread::sleep_for(std::chrono::seconds(1));

    // 松手
    std::vector<uint8_t> open_pose = {255, 104, 255, 255, 255, 255, 255, 255, 255, 71};
    hand.fingerMove(open_pose);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
    return 0;
}
```

---

## Notes
- 在使用 API 之前，请确保手部设备已正确连接并初始化。
- 参数值（如速度、扭矩等）的具体范围和含义请参考设备的技术手册。

---

## Contact
- 如果有任何问题或需要进一步支持，请联系 [https://linkerbot.cn/aboutUs](https://linkerbot.cn/aboutUs)。

---