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

// 获取当前关节状态
std::vector<double> LinkerHandApi::getState()
{
}

// 获取速度
std::vector<double> LinkerHandApi::getSpeed()
{
}

// 设置速度
void LinkerHandApi::setSpeed(const std::vector<double> &speed)
{
}



//---------------------------------------------------------------

// 获取法向压力、切向压力、切向方向、接近感应
std::vector<std::vector<double>> LinkerHandApi::getForce()
{
    return hand->getForce();
}

// 获取大拇指、食指、中指、无名指、小指的所有压力数据
std::vector<std::vector<double>> LinkerHandApi::getPressure()
{
    return hand->getPressureData();
}

// 设置关节位置
void LinkerHandApi::fingerMove(const std::vector<double> &pose)
{
    hand->setJointPositions(pose);
}

// 获取版本号
std::vector<double> LinkerHandApi::getVersion()
{
    return hand->getVersion();
}