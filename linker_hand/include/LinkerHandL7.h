#ifndef LINKER_HAND_L7_H
#define LINKER_HAND_L7_H

#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <sstream>
#include <condition_variable>

#include "IHand.h"
#include "CanBus.h"


namespace LinkerHandL7
{

typedef enum
{									  
    // 指令码	指令功能		        	数据长度	        CAN发送DLC	CAN接收DLC	数据范围
    JOINT_POSITION = 0x01,	            // 关节1-7的关节位置		7	8	8	0-0xFF
    TORQUE_LIMIT = 0x02,	            // 关节1-7的转矩限制	    7	8	8	0-0xFF
    JOINT_SPEED = 0x05,	                // 关节1-7的速度		   7	8	8	0-0xFF
    HAND_NORMAL_FORCE = 0x20,	        // 五个手指的法向压力		5	1	6	0-0xFF
    HAND_TANGENTIAL_FORCE = 0x21,	    // 五个手指的切向压力		5	1	6	0-0xFF
    HAND_TANGENTIAL_FORCE_DIR = 0x22,	// 五个手指的切向方向		5	1	6	0-0x7F 0xFF
    HAND_APPROACH_INC = 0x23,	        // 五个手指指接近感应		5	1	6	0-0xFF
    THUMB_ALL_DATA = 0x28,	            // 大拇指所有压力数据		4	1	5	0-0xFF
    INDEX_FINGER_ALL_DATA = 0x29,	    // 食指所有压力数据			4	1	5	0-0xFF
    MIDDLE_FINGER_ALL_DATA = 0x30,	    // 中指所有压力数据		    4	1	5	0-0xFF
    RING_FINGER_ALL_DATA = 0x31,	    // 无名指所有压力数据		4	1	5	0-0xFF
    LITTLE_FINGER_ALL_DATA = 0x32,	    // 小拇指所有压力数据		4	1	5	0-0xFF
    MOTOR_TEMPERATURE = 0x33,	        // 关节1-7的温度信息		7	1	8	0-0xFF
    MOTOR_FAULT_CODE = 0x35,	        // 关节1-7的错误码			7	1	8	0-0xFF
    RESET_ZERO_COMMAND = 0x38,	        // 重新调零点命令		    1	1	2	
    LINKER_HAND_VERSION = 0x64	        // 版本号			       8   1   8    0-0xFF
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
    // 获取当前扭矩
	std::vector<uint8_t> getTorque() override;
	// 获取当前关节状态
    std::vector<uint8_t> getCurrentStatus() override;
	//--------------------------------------------------------------------
	// 获取所有压感数据
    std::vector<std::vector<uint8_t>> getForce(const int type = 0) override;
    #if 1
	// 获取五个手指的法向压力
    std::vector<uint8_t> getNormalForce() override;
	// 获取五个手指的切向压力
    std::vector<uint8_t> getTangentialForce() override;
	// 获取五个手指的切向方向
    std::vector<uint8_t> getTangentialForceDir() override;
	// 获取五个手指指接近感应
    std::vector<uint8_t> getApproachInc() override;
    #endif
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
	// 获取电机温度
    std::vector<uint8_t> getTemperature() override;
	// 获取电机故障码
    std::vector<uint8_t> getFaultCode() override;
    // 获取版本信息
    std::string getVersion() override;
	

private:
    uint32_t handId;
    Communication::CanBus bus;
    std::thread receiveThread;
    bool running;

    void receiveResponse();

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
} // namespace LinkerHandL7
#endif // LINKER_HAND_L7_H