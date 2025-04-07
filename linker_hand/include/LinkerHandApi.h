#ifndef LINKER_HAND_API_H
#define LINKER_HAND_API_H

#include <iostream>
#include <vector>
#include <memory>

#include "LinkerHandL10Can.h"
#include "HandFactory.h"

class LinkerHandApi
{
public:
	LinkerHandApi(const std::string &handType, const std::string &handJoint);

	~LinkerHandApi();

	// 获取法向压力
	void getNormalForce();
	// 获取切向压力
	void getTangentialForce();
	// 获取切向压力方向
	void getTangentialForceDir();
	// 获取接近度
	void getApproachInc();
	// 获取压力
	std::vector<std::vector<double>> getForce();
	// 获取当前关节状态
	std::vector<double> getState();
	// 获取速度
	std::vector<double> getSpeed();
	// 设置速度
	void setSpeed(const std::vector<double> &speed);
	// 设置关节位置
	void fingerMove(const std::vector<double> &pose);

	// 获取版本号
	std::vector<double> getVersion();

private:
	std::unique_ptr<IHand> hand;
	uint32_t handId;
};

#endif
