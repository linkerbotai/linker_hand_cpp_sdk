
---

# Linker Hand C++ API Documentation

## API Overview

This document provides a detailed overview of the C++ API for the Linker Hand, including functions for controlling the hand's movements, retrieving sensor data, and setting operational parameters.

## Public API

### 设置速度
```cpp
void setSpeed(const std::vector<uint8_t> &speed);
```
**Description**:  
设置手部的运动速度。  
**Parameters**:  
- `speed`: 一个包含速度数据的 `std::vector<uint8_t>`，长度为5个元素对应每个关节的速度值。

---

### 设置五根手指的转矩限制 - 力度
```cpp
void setPressureData(const std::vector<uint8_t> &pressure);
```
**Description**:  
设置手指的转矩限制或力度，用于控制抓取力度。  
**Parameters**:  
- `pressure`: 一个包含力度数据的 `std::vector<uint8_t>`，长度为5个元素对应每个手指的力度值。

---

### 设置关节位置
```cpp
void fingerMove(const std::vector<uint8_t> &pose);
```
**Description**:  
设置关节的目标位置，用于控制手指的运动。  
**Parameters**:  
- `pose`: 一个包含目标位置数据的 `std::vector<uint8_t>`，长度为10个元素。

---

### 获取速度
```cpp
std::vector<uint8_t> getSpeed();
```
**Description**:  
获取当前设置的速度值。提示：需设置关节位置后才能获取到速度值。

**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含当前的速度设置值。

---

### 获取当前关节状态
```cpp
std::vector<uint8_t> getState();
```
**Description**:  
获取当前关节的状态信息。提示：需要设置关节位置后才能获取到状态信息。

**Returns**:  
- 返回一个 `std::vector<uint8_t>`，包含当前关节的状态数据。

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
- 参数值（如速度、力度等）的具体范围和含义请参考设备的技术手册。

---

## Contact
- 如果有任何问题或需要进一步支持，请联系 [support@linkerhand.com](mailto:support@linkerhand.com)。

---