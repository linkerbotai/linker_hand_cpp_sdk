#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <sstream>
#include <condition_variable>

#include "IHand.h"
#include "CanBus.h"

namespace LinkerHandL10
{

typedef enum
{									  
	// INVALID_FRAME_PROPERTY = 0x00,	// 无效的can帧属性
    JOINT_POSITION_RCO = 0x01,			// 关节1-6的关节位置
    MAX_PRESS_RCO = 0x02,				// 五根手指的转矩限制		
    JOINT_POSITION2_RCO = 0x04,			// 关节7-10的关节位置
    JOINT_SPEED = 0x05,					// 五根手指的速度
    REQUEST_DATA_RETURN = 0x09,			// 获取所有关节位置和压力
    // JOINT_POSITION_N = 0x11,
    // MAX_PRESS_N = 0x12,
    HAND_NORMAL_FORCE = 0X20,			// 五个手指的法向压力
    HAND_TANGENTIAL_FORCE = 0X21,		// 五个手指的切向压力
    HAND_TANGENTIAL_FORCE_DIR = 0X22,	// 五个手指的切向方向
    HAND_APPROACH_INC = 0X23,			// 五个手指指接近感应
	THUMB_ALL_DATA = 0x28,              // 大拇指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    INDEX_FINGER_ALL_DATA = 0x29,       // 食指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    MIDDLE_FINGER_ALL_DATA = 0x30,      // 中指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    RING_FINGER_ALL_DATA = 0x31,        // 无名指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
    LITTLE_FINGER_ALL_DATA = 0x32,      // 小拇指所有数据 | 返回 1法向压力 2切向压力 3切向方向 4接近感应
	MOTOR_TEMPERATURE_1 = 0x33,			// 电机温度1
    MOTOR_TEMPERATURE_2 = 0x34,			// 电机温度2
	MOTOR_FAULT_CODE_1 = 0x35,			// 电机故障码1
	MOTOR_FAULT_CODE_2 = 0x36,			// 电机故障码2
	LINKER_HAND_VERSION = 0X64			// 版本号
}FRAME_PROPERTY;

class LinkerHand : public IHand
{
public:
    LinkerHand(uint32_t handId, const std::string &canChannel, int baudrate);
    ~LinkerHand();

	// 设置关节位置
    void setJointPositions(const std::vector<u_int8_t> &jointAngles) override;
	// 设置最大扭矩
	void setTorque(const std::vector<uint8_t> &torque) override;
	// 设置关节速度
    void setSpeed(const std::vector<uint8_t> &speed) override;
	// 获取当前速度
    std::vector<uint8_t> getSpeed() override;
	// 获取当前关节状态
    std::vector<uint8_t> getCurrentStatus() override;
	//--------------------------------------------------------------------
	// 获取所有压感数据
    std::vector<std::vector<uint8_t>> getForce(const int type = 0) override;
	// 获取五个手指的法向压力
    std::vector<uint8_t> getNormalForce() override;
	// 获取五个手指的切向压力
    std::vector<uint8_t> getTangentialForce() override;
	// 获取五个手指的切向方向
    std::vector<uint8_t> getTangentialForceDir() override;
	// 获取五个手指指接近感应
    std::vector<uint8_t> getApproachInc() override;
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
	//--------------------------------------------------------------------
	// 获取版本信息
    std::string getVersion() override;
	// 获取电机温度
    std::vector<uint8_t> getMotorTemperature() override;
	// 获取电机故障码
    std::vector<uint8_t> getMotorFaultCode() override;
	// 获取电机电流
    std::vector<uint8_t> getMotorCurrent() override;	// 暂时无用
	// 获取所有关节位置和压力
    std::vector<uint8_t> requestAllStatus(); // 暂时无用
	// 获取当前扭矩
	std::vector<uint8_t> getTorque() override;

private:
    uint32_t handId;
    AIMOcommunicate::CanBus bus;
    std::thread receiveThread;
    bool running;
    std::mutex responseMutex;

    void receiveResponse();
	std::vector<uint8_t> getSubVector(const std::vector<uint8_t> &vec);
	std::vector<uint8_t> getSubVector(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2);

    std::queue<std::vector<uint8_t>> responseQueue; // 通用响应队列
    std::condition_variable queueCond;              // 通用队列条件变量
    std::mutex queueMutex;                          // 通用队列互斥锁

    // 队列和条件变量
	std::vector<uint8_t> joint_position;
	std::vector<uint8_t> joint_position2;
	std::vector<uint8_t> joint_speed;

	std::vector<uint8_t> normal_force;
    std::vector<uint8_t> tangential_force;
    std::vector<uint8_t> tangential_force_dir;
    std::vector<uint8_t> approach_inc;

	std::vector<uint8_t> thumb_pressure;
    std::vector<uint8_t> index_finger_pressure;
    std::vector<uint8_t> middle_finger_pressure;
    std::vector<uint8_t> ring_finger_pressure;
    std::vector<uint8_t> little_finger_pressure;

	// 最大扭矩
    std::vector<uint8_t> max_torque;

	// 电机温度
    std::vector<uint8_t> motorTemperature_1;
	std::vector<uint8_t> motorTemperature_2;
	
	// 电机故障码
    std::vector<uint8_t> motorFaultCode_1;
	std::vector<uint8_t> motorFaultCode_2;

	// 版本信息
	std::vector<uint8_t> version;
};
} // namespace LinkerHandL10