#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <sstream>
#include <condition_variable>

#include "IHand.h"
#include "CanBus.h"

typedef enum
{									  
    INVALID_FRAME_PROPERTY = 0x00,  // 无效的can帧属性 | 无返回
    JOINT_PITCH_R = 0x01,           // 短帧俯仰角-手指根部弯曲 | 返回本类型数据
    JOINT_YAW_R = 0x02,             // 短帧航向角-手指横摆，控制间隙 | 返回本类型数据
    JOINT_ROLL_R = 0x03,            // 短帧横滚角-只有大拇指副用到了 | 返回本类型数据
    JOINT_TIP_R = 0x04,             // 短帧指尖角度控制 | 返回本类型数据
    JOINT_SPEED_R = 0x05,           // 短帧速度 电机运行速度控制 | 返回本类型数据
    JOINT_CURRENT_R = 0x06,         // 短帧电流 电机运行电流反馈 | 返回本类型数据
    JOINT_FAULT_R = 0x07,           // 短帧故障 电机运行故障反馈 | 返回本类型数据
    REQUEST_DATA_RETURN = 0x09,      // 请求数据返回 | 返回所有数据
    JOINT_PITCH_NR = 0x11,           // 俯仰角-手指根部弯曲 | 不返回本类型数据
    JOINT_YAW_NR = 0x12,             // 航向角-手指横摆，控制间隙 | 不返回本类型数据
    JOINT_ROLL_NR = 0x13,            // 横滚角-只有大拇指副用到了 | 不返回本类型数据
    JOINT_TIP_NR = 0x14,             // 指尖角度控制 | 不返回本类型数据
    JOINT_SPEED_NR = 0x15,           // 速度 电机运行速度控制 | 不返回本类型数据
    JOINT_CURRENT_NR = 0x16,         // 电流 电机运行电流反馈 | 不返回本类型数据
    JOINT_FAULT_NR = 0x17,           // 故障 电机运行故障反馈 | 不返回本类型数据
    HAND_UID = 0xC0,                 // 设备唯一标识码 只读 --------
    HAND_HARDWARE_VERSION = 0xC1,    // 硬件版本 只读 --------
    HAND_SOFTWARE_VERSION = 0xC2,    // 软件版本 只读 --------
    HAND_COMM_ID = 0xC3,             // 设备ID 可读写 1字节
    HAND_SAVE_PARAMETER = 0xCF       // 保存参数 只写 --------
}FRAME_PROPERTY;

class LinkerHandL20Can : public IHand {
public:
    LinkerHandL20Can(uint32_t handId, const std::string& canChannel, int baudrate);
    ~LinkerHandL20Can();

    void setJointPositions(const std::vector<u_int8_t> &jointAngles) override;
    std::vector<uint8_t> getCurrentStatus() override;
    void setJointSpeed(const std::vector<uint8_t> &speed) override;
	void setPressure(const std::vector<uint8_t> &pressure) override;
    std::vector<uint8_t> getSpeed() override;
    std::vector<std::vector<uint8_t>> getForce() override;
    void getNormalForce() override;
    void getTangentialForce() override;
    void getTangentialForceDir() override;
    void getApproachInc() override;
    std::string getVersion() override;

    std::vector<std::vector<uint8_t>> getPressureData() override;

private:
    uint32_t handId;
    // CanBus bus;
    std::thread receiveThread;
    bool running;
    std::mutex responseMutex;

    void receiveResponse();
};
