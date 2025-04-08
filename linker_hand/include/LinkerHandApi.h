#ifndef LINKER_HAND_API_H
#define LINKER_HAND_API_H

#include <iostream>
#include <vector>
#include <memory>

#include "LinkerHandL10Can.h"
#include "HandFactory.h"
#include "YamlConfig.h"

class LinkerHandApi
{
public:
	LinkerHandApi(const std::string &handType, const std::string &handJoint);

	~LinkerHandApi();

	// 获取法向压力、切向压力、切向方向、接近感应
	std::vector<std::vector<double>> getForce();
	// 获取大拇指、食指、中指、无名指、小指的所有压力数据
	std::vector<std::vector<double>> getPressure();
	// 设置关节位置
	void fingerMove(const std::vector<double> &pose);
	// 获取版本号
	std::vector<double> getVersion();

private: // 开发中，暂不开放
	// 获取法向压力
	void getNormalForce();
	// 获取切向压力
	void getTangentialForce();
	// 获取切向压力方向
	void getTangentialForceDir();
	// 获取接近感应
	void getApproachInc();
	// 获取当前关节状态
	std::vector<double> getState();
	// 获取速度
	std::vector<double> getSpeed();
	// 设置速度
	void setSpeed(const std::vector<double> &speed);
	// 设置五根手指的转矩限制 - 力度
	void setPressureData(const std::vector<double> &pressure);

private:
	std::unique_ptr<IHand> hand;
	uint32_t handId;
};

#endif
