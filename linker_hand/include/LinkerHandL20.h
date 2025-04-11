#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <sstream>
#include <condition_variable>

#include "IHand.h"
#include "CanBus.h"

namespace LinkerHandL20
{

typedef enum
{									  
    INVALID_FRAME_PROPERTY = 0x00,  // 无效的can帧属性 | 无返回
    JOINT_PITCH_R = 0x01,           // 短帧俯仰角-手指根部弯曲程度 | 返回本类型数据
    JOINT_YAW_R = 0x02,             // 短帧航向角-手指横摆，控制间隙 | 返回本类型数据
    JOINT_ROLL_R = 0x03,            // 短帧横滚角-只有大拇指副用到了 | 返回本类型数据
    JOINT_TIP_R = 0x04,             // 短帧指尖角度弯曲程度 | 返回本类型数据
    JOINT_SPEED_R = 0x05,           // 短帧速度 电机运行速度控制 | 返回本类型数据
    JOINT_CURRENT_R = 0x06,         // 短帧电流 电机运行电流 | 返回本类型数据
    JOINT_FAULT_R = 0x07,           // 短帧故障 电机运行故障 1清除 0查询 | 返回本类型数据
    ROTOR_LOCK_COUNT = 0x08,        // 手指堵转或过流判断计数阀值 | 返回本类型数据
    REQUEST_DATA_RETURN = 0x09,      // 请求数据返回 | 返回所有手指控制数据，返回数据会分若干帧发出。
    JOINT_PITCH_NR = 0x11,           // 俯仰角-手指根部弯曲 | 不返回本类型数据
    JOINT_YAW_NR = 0x12,             // 航向角-手指横摆，控制间隙 | 不返回本类型数据
    JOINT_ROLL_NR = 0x13,            // 横滚角-只有大拇指副用到了 | 不返回本类型数据
    JOINT_TIP_NR = 0x14,             // 指尖角度控制 | 不返回本类型数据
    JOINT_SPEED_NR = 0x15,           // 速度 电机运行速度控制 | 不返回本类型数据
    JOINT_CURRENT_NR = 0x16,         // 电流 电机运行电流反馈 | 不返回本类型数据
    JOINT_FAULT_NR = 0x17,           // 故障 电机运行故障反馈 | 不返回本类型数据
    HAND_NORMAL_FORCE = 0X20,			// 返回五个手指的法向压力
    HAND_TANGENTIAL_FORCE = 0X21,		// 返回五个手指的切向压力
    HAND_TANGENTIAL_FORCE_DIR = 0X22,	// 返回五个手指的切向方向 数值0-127对应实际切向力角度0-359,无切向力保持255
    HAND_APPROACH_INC = 0X23,			// 返回五个手指指接近感应
    THUMB_ALL_DATA = 0x30,              // 大拇指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    INDEX_FINGER_ALL_DATA = 0x31,       // 食指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    MIDDLE_FINGER_ALL_DATA = 0x32,      // 中指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    RING_FINGER_ALL_DATA = 0x33,        // 无名指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    LITTLE_FINGER_ALL_DATA = 0x34,      // 小拇指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    HAND_UID = 0xC0,                 // 设备唯一标识码 只读 --------
    HAND_HARDWARE_VERSION = 0xC1,    // 硬件版本 只读 --------
    HAND_SOFTWARE_VERSION = 0xC2,    // 软件版本 只读 --------
    HAND_COMM_ID = 0xC3,             // 设备ID 可读写 1字节 
    HAND_SAVE_PARAMETER = 0xCF       // 保存参数 只写 --------
}FRAME_PROPERTY;


class LinkerHand : public IHand {
public:
    LinkerHand(uint32_t handId, const std::string& canChannel, int baudrate);
    ~LinkerHand();

    // 设置关节位置
    void setJointPositions(const std::vector<u_int8_t> &jointAngles) override;
	// 设置关节速度
    void setSpeed(const std::vector<uint8_t> &speed) override;
	// 获取当前速度
    std::vector<uint8_t> getSpeed() override;
	// 获取当前关节状态
    std::vector<uint8_t> getCurrentStatus() override;
	//--------------------------------------------------------------------
	// 获取所有压感数据
    std::vector<std::vector<uint8_t>> getForce(const int type = 0) override;
	// 获取大拇指压感数据
    std::vector<uint8_t> getThumbForce() override;
    // 获取食指压感数据
    std::vector<uint8_t> getIndexForce() override;
    // 获取中指压感数据
    std::vector<uint8_t> getMiddleForce() override;
    // 获取无名指压感数据
    std::vector<uint8_t> getRingForce() override;
    // 获取小拇指压感数据
    std::vector<uint8_t> getLittleForce() override;
    #if 0
	// 获取五个手指的法向压力
    std::vector<uint8_t> getNormalForce() override;
	// 获取五个手指的切向压力
    std::vector<uint8_t> getTangentialForce() override;
	// 获取五个手指的切向方向
    std::vector<uint8_t> getTangentialForceDir() override;
	// 获取五个手指指接近感应
    std::vector<uint8_t> getApproachInc() override;
    #endif
    //--------------------------------------------------------------------
    // 获取电机故障码
    std::vector<uint8_t> getMotorFaultCode() override;
    // 获取电机电流
    std::vector<uint8_t> getMotorCurrent() override;
    // 清除电机故障码
    void clearMotorFaultCode(const std::vector<uint8_t> &torque) override;
    // 设置电流
    void setMotorCurrent(const std::vector<uint8_t> &current) override;

private:
    uint32_t handId;
    AIMOcommunicate::CanBus bus;
    std::thread receiveThread;
    bool running;
    std::mutex responseMutex;

    void receiveResponse();
};
}