#ifndef LINKER_HAND_API_H
#define LINKER_HAND_API_H

#include <iostream>
#include <vector>
#include <memory>

#include "HandFactory.h"
#include "YamlConfig.h"

class LinkerHandApi
{
public:
	LinkerHandApi(const LINKER_HAND &handJoint, const HAND_TYPE &handType);
	~LinkerHandApi();

	// 设置关节位置
	void fingerMove(const std::vector<uint8_t> &pose);
	// 设置速度
	void setSpeed(const std::vector<uint8_t> &speed);
	// 设置扭矩
	void setTorque(const std::vector<uint8_t> &torque);
	// 获取法向压力、切向压力、切向方向、接近感应
	std::vector<std::vector<uint8_t>> getForce(const int dataType = 0);
	// 获取速度
	std::vector<uint8_t> getSpeed();
	// 获取当前关节状态
	std::vector<uint8_t> getState();
	// 获取版本号
	std::string getVersion();

	// ----------------------------------------------------------

	// 获取电机温度
	std::vector<uint8_t> getMotorTemperature();
	// 获取电机故障码
	std::vector<uint8_t> getMotorFaultCode();
	// 获取当前电流
	std::vector<uint8_t> getMotorCurrent();
	// 获取当前最大扭矩
	std::vector<uint8_t> getMotorTorque();
	// ------------------------- 待开发 --------------------------
	
	// 设置电流 目前仅支持L20 
	void setMotorCurrent(const std::vector<uint8_t> &current);
	// 设置电机使能 目前仅支持L25
	void setMotorEnable(const std::vector<uint8_t> &enable);
	// 设置电机使能 目前仅支持L25
	void setMotorDisable(const std::vector<uint8_t> &disable);
	// 清除电机故障码 目前仅支持L20
	void clearMotorFaultCode(const std::vector<uint8_t> &torque = std::vector<uint8_t>(5, 1));

private:
	// 获取法向压力
	void getNormalForce();
	// 获取切向压力
	void getTangentialForce();
	// 获取切向压力方向
	void getTangentialForceDir();
	// 获取接近感应
	void getApproachInc();

private:
	std::unique_ptr<IHand> hand;
	uint32_t handId;
public:
	LINKER_HAND handJoint_;
	HAND_TYPE handType_;
};

#endif
