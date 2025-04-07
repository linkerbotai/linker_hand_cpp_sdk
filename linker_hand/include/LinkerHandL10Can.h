#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "IHand.h"
#include "CanBus.h"

typedef enum
{									  
	INVALID_FRAME_PROPERTY_ = 0x00, //无效的can帧属性	   |无返回
	JOINT_PITCH_R   = 0x01,         //短帧俯仰角-手指根部弯曲		 |返回本类型数据
	JOINT_YAW_R     = 0x02,         //短帧航向角-手指横摆，控制间隙	  |返回本类型数据
	JOINT_ROLL_R    = 0x03,	        //短帧横滚角-只有大拇指副用到了   |返回本类型数据
	JOINT_TIP_R	    = 0X04,	        //短帧指尖角度控制				|返回本类型数据
	JOINT_SPEED_R   = 0X05,	        //短帧速度 	电机运行速度控制	|返回本类型数据
	JOINT_CURRENT_R = 0X06,	        //短帧电流 	电机运行电流反馈	|返回本类型数据
	JOINT_FAULT_R   = 0X07,         //短帧故障 	电机运行故障反馈	|返回本类型数据
	
	REQUEST_DATA_RETURN_ = 0x9,	 //请求数据返回				    |返回所有数据
	JOINT_PITCH_NR	 = 0x11,	 //俯仰角-手指根部弯曲           |不返回本类型数据
	JOINT_YAW_NR	 = 0x12,     //航向角-手指横摆，控制间隙	  |不返回本类型数据
	JOINT_ROLL_NR	 = 0x13,	 //横滚角-只有大拇指副用到了	  |不返回本类型数据
	JOINT_TIP_NR	 = 0X14,	 //指尖角度控制			        |不返回本类型数据
	JOINT_SPEED_NR	 = 0X15,	 //速度 	电机运行速度控制	  |不返回本类型数据
	JOINT_CURRENT_NR = 0X16,     //电流 电机运行电流反馈	      |不返回本类型数据
	JOINT_FAULT_NR   = 0X17,	 //故障 	电机运行故障反馈	  |不返回本类型数据
}FRAME_PROPERTY;


struct ForceData
{
    std::vector<uint8_t> normal_force;
    std::vector<uint8_t> tangential_force;
    std::vector<uint8_t> tangential_force_dir;
    std::vector<uint8_t> approach_inc;
};

class LinkerHandL10Can : public IHand
{
public:
    LinkerHandL10Can(uint32_t canId, const std::string &canChannel, int baudrate);
    ~LinkerHandL10Can();

    void setJointPositions(const std::vector<double> &jointAngles) override;
    std::vector<double> getCurrentStatus() override;
    void setJointSpeed(const std::vector<double> &speed) override;
    std::vector<double> getSpeed() override;
    std::vector<std::vector<double>> getForce() override;
    void getNormalForce() override;
    void getTangentialForce() override;
    void getTangentialForceDir() override;
    void getApproachInc() override;
    std::vector<double> getVersion() override;

    std::vector<std::vector<double>> getPressureData() override;

private:
    uint32_t canId;
    AIMOcommunicate::CanBus bus;
    std::thread receiveThread;
    bool running;
    std::mutex responseMutex;

    void receiveResponse();

    ForceData forceData;

    // 队列和条件变量
    std::queue<std::vector<uint8_t>> responseQueue; // 通用响应队列
    std::queue<ForceData> forceQueue;               // 传感器数据队列
    std::mutex queueMutex;                          // 通用队列互斥锁
    std::mutex forceQueueMutex;                     // 传感器数据队列互斥锁
    std::condition_variable queueCond;              // 通用队列条件变量
    std::condition_variable forceQueueCond;         // 传感器数据队列条件变量
};
