#include "LinkerHandApi.h"

LinkerHandApi::LinkerHandApi(const LINKER_HAND &handJoint, const HAND_TYPE &handType) : handJoint_(handJoint), handType_(handType)
{
    hand = HandFactory::createHand(handJoint, handType, "can0", 1000000);
}

LinkerHandApi::~LinkerHandApi()
{
}

void LinkerHandApi::setTorque(const std::vector<uint8_t> &torque)
{
    hand->setTorque(torque);
}

void LinkerHandApi::setSpeed(const std::vector<uint8_t> &speed)
{
    hand->setSpeed(speed);
}

void LinkerHandApi::fingerMove(const std::vector<uint8_t> &pose)
{
    hand->setJointPositions(pose);
}

void LinkerHandApi::fingerMoveArc(const std::vector<double> &pose)
{
    hand->setJointPositionArc(pose);
}

std::vector<uint8_t> LinkerHandApi::getSpeed()
{
    // 获取当前速度
    return hand->getSpeed();
}

std::vector<uint8_t> LinkerHandApi::getState()
{
    // 获取当前关节状态
    return hand->getCurrentStatus();
}

std::vector<double> LinkerHandApi::getStateArc()
{
    // 获取当前关节状态
    return hand->getCurrentStatusArc();
}

std::vector<std::vector<uint8_t>> LinkerHandApi::getForce(const int type)
{
    // 获取所有压感数据
    return hand->getForce(type);
}

std::string LinkerHandApi::getVersion()
{
    // 获取版本号
    return hand->getVersion();
}

std::vector<uint8_t> LinkerHandApi::getTorque()
{
    // 获取当前扭矩
    return hand->getTorque();
}

std::vector<uint8_t> LinkerHandApi::getTemperature()
{
    // 获取温度
    return hand->getTemperature();
}

std::vector<uint8_t> LinkerHandApi::getFaultCode()
{
    // 获取故障码
    return hand->getFaultCode();
}

std::vector<uint8_t> LinkerHandApi::getCurrent()
{
    // 获取当前电流
    return hand->getCurrent();
}

// 设置电流 目前仅支持L20
void LinkerHandApi::setCurrent(const std::vector<uint8_t> &current)
{
    if (handJoint_ == LINKER_HAND::L20) {
        hand->setCurrent(current);
    } else {
        std::cout << "目前仅支持L20!" << std::endl;
    }
}

// 设置电机使能 目前仅支持L25
void LinkerHandApi::setEnable(const std::vector<uint8_t> &enable)
{
    std::cout << "目前仅支持L25!" << std::endl;
    if (handJoint_ == LINKER_HAND::L25) {
        // hand->setMotorEnable(enable);
    }
}

// 设置电机使能 目前仅支持L25
void LinkerHandApi::setDisable(const std::vector<uint8_t> &disable)
{
    std::cout << "目前仅支持L25!" << std::endl;
    if (handJoint_ == LINKER_HAND::L25) {
        // hand->setMotorDisable(disable);
    }
}

// 清除电机故障码 目前仅支持L20
void LinkerHandApi::clearFaultCode(const std::vector<uint8_t> &torque)
{
    if (handJoint_ == LINKER_HAND::L20) {
        hand->clearFaultCode(torque);
    } else {
        std::cout << "目前仅支持L20!" << std::endl;
    }
}