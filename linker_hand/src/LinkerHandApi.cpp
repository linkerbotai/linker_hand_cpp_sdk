#include "LinkerHandApi.h"

LinkerHandApi::LinkerHandApi(const std::string &handType, const std::string &handJoint)
{
    if (handType == "left")
    {
        handId = 0x28;
    }
    else if (handType == "right")
    {
        handId = 0x27;
    }
    else
    {
        throw std::runtime_error("Please choose left or right hand !");
    }

    if (handJoint == "L10")
    {
        hand = HandFactory::createHand(HandType::L10, handId, "can0", 1000000);
    }
    else if (handJoint == "L20")
    {
        hand = HandFactory::createHand(HandType::L20, handId, "can0", 1000000);
    }
    else
    {
        throw std::invalid_argument("Unsupported hand joint type");
    }
}

LinkerHandApi::~LinkerHandApi()
{
}

// 获取法向压力
void LinkerHandApi::getNormalForce()
{
}

// 获取切向压力
void LinkerHandApi::getTangentialForce()
{
}

// 获取切向压力方向
void LinkerHandApi::getTangentialForceDir()
{
}

// 获取接近度
void LinkerHandApi::getApproachInc()
{
}

// 获取压力
std::vector<std::vector<double>> LinkerHandApi::getForce()
{
    return hand->getForce();
}

// 获取当前关节状态
std::vector<double> LinkerHandApi::getState()
{
    // return {1,2,3,4,5,6};\

    return hand->getVersion();
}

std::vector<double> LinkerHandApi::getVersion()
{
    // return {1,2,3,4,5,6};\

    return hand->getVersion();
}

// 获取速度
std::vector<double> LinkerHandApi::getSpeed()
{
}

// 设置速度
void LinkerHandApi::setSpeed(const std::vector<double> &speed)
{
}

// 设置关节位置
void LinkerHandApi::fingerMove(const std::vector<double> &pose)
{
    hand->setJointPositions(pose);
}
