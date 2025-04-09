#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <sstream>
#include <condition_variable>

#include "IHand.h"
#include "CanBus.h"

// typedef enum
// {									  
// 	INVALID_FRAME_PROPERTY_ = 0x00, //无效的can帧属性	   |无返回
// 	JOINT_PITCH_R   = 0x01,         //短帧俯仰角-手指根部弯曲		 |返回本类型数据
// 	JOINT_YAW_R     = 0x02,         //短帧航向角-手指横摆，控制间隙	  |返回本类型数据
// 	JOINT_ROLL_R    = 0x03,	        //短帧横滚角-只有大拇指副用到了   |返回本类型数据
// 	JOINT_TIP_R	    = 0X04,	        //短帧指尖角度控制				|返回本类型数据
// 	JOINT_SPEED_R   = 0X05,	        //短帧速度 	电机运行速度控制	|返回本类型数据
// 	JOINT_CURRENT_R = 0X06,	        //短帧电流 	电机运行电流反馈	|返回本类型数据
// 	JOINT_FAULT_R   = 0X07,         //短帧故障 	电机运行故障反馈	|返回本类型数据
	
// 	REQUEST_DATA_RETURN_ = 0x9,	 //请求数据返回				    |返回所有数据
// 	JOINT_PITCH_NR	 = 0x11,	 //俯仰角-手指根部弯曲           |不返回本类型数据
// 	JOINT_YAW_NR	 = 0x12,     //航向角-手指横摆，控制间隙	  |不返回本类型数据
// 	JOINT_ROLL_NR	 = 0x13,	 //横滚角-只有大拇指副用到了	  |不返回本类型数据
// 	JOINT_TIP_NR	 = 0X14,	 //指尖角度控制			        |不返回本类型数据
// 	JOINT_SPEED_NR	 = 0X15,	 //速度 	电机运行速度控制	  |不返回本类型数据
// 	JOINT_CURRENT_NR = 0X16,     //电流 电机运行电流反馈	      |不返回本类型数据
// 	JOINT_FAULT_NR   = 0X17,	 //故障 	电机运行故障反馈	  |不返回本类型数据
// }FRAME_PROPERTY;

// 指令码		指令功能			数据长度	CAN发送DLC	CAN接收DLC	数据范围
// 0x01	关节1-6的关节位置			6	7	7	0-0xFF
// 0x02	五根手指的转矩限制			5	6	6	0-0xFF
// 0x04	关节7-10的关节位置			4	5	5	0-0xFF
// 0x05	五根手指的速度				5	6	6	0-0xFF
// 0x20	五个手指的法向压力			5	1	6	0-0xFF
// 0x21	五个手指的切向压力			5	1	6	0-0xFF
// 0x22	五个手指的切向方向			5	1	6	0-0x7F 0xFF
// 0x23	五个手指指接近感应			5	1	6	0-0xFF
// 0x28	大拇指所有压力数据			4	1	5	0-0xFF
// 0x29	食指所有压力数据			4	1	5	0-0xFF
// 0x30	中指所有压力数据			4	1	5	0-0xFF
// 0x31	无名指所有压力数据			4	1	5	0-0xFF
// 0x32	小拇指所有压力数据			4	1	5	0-0xFF
// 0x64	版本号					  8	  1   8	  0-0xFF

typedef enum
{									  
	// INVALID_FRAME_PROPERTY = 0x00,	// 无效的can帧属性
    JOINT_POSITION_RCO = 0x01,			// 关节1-6的关节位置
    MAX_PRESS_RCO = 0x02,				// 五根手指的转矩限制		
    JOINT_POSITION2_RCO = 0x04,			// 关节7-10的关节位置
    JOINT_SPEED = 0x05,					// 五根手指的速度
    // REQUEST_DATA_RETURN = 0x09,
    // JOINT_POSITION_N = 0x11,
    // MAX_PRESS_N = 0x12,
    HAND_NORMAL_FORCE = 0X20,			// 五个手指的法向压力
    HAND_TANGENTIAL_FORCE = 0X21,		// 五个手指的切向压力
    HAND_TANGENTIAL_FORCE_DIR = 0X22,	// 五个手指的切向方向
    HAND_APPROACH_INC = 0X23,			// 五个手指指接近感应

	PRESSURE_THUMB = 0x28,				// 大拇指所有压力数据		
	PRESSURE_INDEX_FINGER = 0x29,		// 食指所有压力数据		
	PRESSURE_MIDDLE_FINGER = 0x30,		// 中指所有压力数据		
	PRESSURE_RING_FINGER = 0x31,		// 无名指所有压力数据		
	PRESSURE_LITTLE_FINGER = 0x32,		// 小拇指所有压力数据		

	LINKER_HAND_VERSION = 0X64			// 版本号
}FRAME_PROPERTY;


struct ForceData
{
    std::vector<uint8_t> normal_force;
    std::vector<uint8_t> tangential_force;
    std::vector<uint8_t> tangential_force_dir;
    std::vector<uint8_t> approach_inc;
};

struct PressureData
{
    std::vector<uint8_t> thumb_pressure;
    std::vector<uint8_t> index_finger_pressure;
    std::vector<uint8_t> middle_finger_pressure;
    std::vector<uint8_t> ring_finger_pressure;
    std::vector<uint8_t> little_finger_pressure;
};


struct CanHand_SendData 
{ 
	uint8_t joint_angle_1;	 //每个手指的关节位置设定
	uint8_t joint_angle_2;
	uint8_t joint_angle_3;
	uint8_t joint_angle_4;
	uint8_t joint_angle_5;
	uint8_t joint_angle_6;
	uint8_t joint_angle_7;
	uint8_t joint_angle_8;
	uint8_t joint_angle_9; 

	uint8_t joint1_rotation; // 拇指旋转 —— 10自由度多余部分

	uint8_t joint1_tip;  // 拇指 末端弯曲度 —— 16自由度多余部分
	uint8_t joint3_raw;  // 中指 旋转
	uint8_t joint2_tip;  // 食指 末端弯曲度
	uint8_t joint3_tip;  // 中指 末端弯曲度
	uint8_t joint4_tip;  // 无名 指末端弯曲度
	uint8_t joint5_tip;  // 小拇 指末端弯曲度
 
	uint8_t speed_1;	 //速度 设定
	uint8_t speed_2;
	uint8_t speed_3;
	uint8_t speed_4;
	uint8_t speed_5; 
 
	uint8_t pressure_1;	 //每个指尖的压力设定
	uint8_t pressure_2;
	uint8_t pressure_3;
	uint8_t pressure_4;
	uint8_t pressure_5; 
 
	bool press_calibration;  //压力传感器校准，暂时不启用
	uint8_t clear_fault;	 //对应位高电平有效，高电平清除故障,低六位对应六个电机的清除电流故障
};

struct CanHand_RecvData
{ 
	uint8_t joint_angle_1;	 //每个手指的关节位置设定
	uint8_t joint_angle_2;
	uint8_t joint_angle_3;
	uint8_t joint_angle_4;
	uint8_t joint_angle_5;
	uint8_t joint_angle_6;
	uint8_t joint_angle_7;
	uint8_t joint_angle_8;
	uint8_t joint_angle_9; 

	uint8_t joint1_rotation; // 拇指旋转 —— 10自由度多余部分

	uint8_t joint1_tip;  // 拇指 末端弯曲度 —— 16自由度多余部分
	uint8_t joint3_raw;  // 中指 旋转
	uint8_t joint2_tip;  // 食指 末端弯曲度
	uint8_t joint3_tip;  // 中指 末端弯曲度
	uint8_t joint4_tip;  // 无名 指末端弯曲度
	uint8_t joint5_tip;  // 小拇 指末端弯曲度
 
	uint8_t pressure_1;	 // 每个指尖的压力设定
	uint8_t pressure_2;
	uint8_t pressure_3;
	uint8_t pressure_4;
	uint8_t pressure_5; 

	uint8_t is_force_calibration; // 压力传感器是否校准标志位
    uint8_t fault_code;			  // 故障码低六位分别是六个电机的过扭矩标志
}; 

class LinkerHandL10Can : public IHand
{
public:
    LinkerHandL10Can(uint32_t handId, const std::string &canChannel, int baudrate);
    ~LinkerHandL10Can();

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
    AIMOcommunicate::CanBus bus;
    std::thread receiveThread;
    bool running;
    std::mutex responseMutex;

    void receiveResponse();

    std::queue<std::vector<uint8_t>> responseQueue; // 通用响应队列
    std::condition_variable queueCond;              // 通用队列条件变量
    std::mutex queueMutex;                          // 通用队列互斥锁

    // 队列和条件变量
    ForceData forceData;
    std::queue<ForceData> forceQueue;               // 传感器数据队列
    std::mutex forceQueueMutex;                     // 传感器数据队列互斥锁
    std::condition_variable forceQueueCond;         // 传感器数据队列条件变量

    PressureData pressureData;
    std::queue<PressureData> pressureQueue;
    std::mutex pressureQueueMutex;
    std::condition_variable pressureQueueCond;

	std::vector<uint8_t> joint_position;
	std::vector<uint8_t> joint_position2;
	std::vector<uint8_t> joint_speed;
	
};
