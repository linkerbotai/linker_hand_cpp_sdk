#include "LinkerHandApi.h"

LinkerHandApi::LinkerHandApi(const LINKER_HAND &handJoint, const HAND_TYPE &handType) : handJoint_(handJoint), handType_(handType)
{
    // try
    // {
    //     YamlConfig::getInstance().loadConfig("config/setting.yaml");
    
    //     std::cout << "SDK VERSION : " << YamlConfig::getInstance().getString("VERSION") << std::endl;
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

    if (handType != HAND_TYPE::LEFT && handType != HAND_TYPE::RIGHT)
    {
        throw std::invalid_argument("Unsupported hand type");
    }
    else
    {
        handId = handType;
    }

    if (handJoint == LINKER_HAND::L7)
    {
        hand = HandFactory::createHand(LINKER_HAND::L7, handId, "can0", 1000000);
    }
    else if (handJoint == LINKER_HAND::L10)
    {
        hand = HandFactory::createHand(LINKER_HAND::L10, handId, "can0", 1000000);
    }
    else if (handJoint == LINKER_HAND::L20)
    {
        hand = HandFactory::createHand(LINKER_HAND::L20, handId, "can0", 1000000);
    }
    else if (handJoint == LINKER_HAND::L25)
    {
        hand = HandFactory::createHand(LINKER_HAND::L25, handId, "can0", 1000000);
    }
    else
    {
        throw std::invalid_argument("Unsupported hand joint type");
    }
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
    if (handJoint_ == LINKER_HAND::L7 && speed.size() == 7)
    {
        hand->setSpeed(speed);
    }
    else if (handJoint_ == LINKER_HAND::L10 && speed.size() == 5)
    {
        hand->setSpeed(speed);
    }
    else
    {
        std::cout << "setSpeed提示：LinkerHand型号或动作序列长度不匹配！" << std::endl;
    }
}

void LinkerHandApi::fingerMove(const std::vector<uint8_t> &pose)
{
    // 设置手指位置
    if (handJoint_ == LINKER_HAND::L7 && pose.size() == 7)
    {
        hand->setJointPositions(pose);
    }
    else if (handJoint_ == LINKER_HAND::L10 && pose.size() == 10)
    {
        hand->setJointPositions(pose);
    }
    else if (handJoint_ == LINKER_HAND::L20 && pose.size() == 20)
    {
        hand->setJointPositions(pose);
    }
    else if (handJoint_ == LINKER_HAND::L25 && pose.size() == 25)
    {
        hand->setJointPositions(pose);
    }
    else
    {
        std::cout << "fingerMove提示：LinkerHand型号或动作序列长度不匹配！" << std::endl;
    }
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

std::vector<uint8_t> LinkerHandApi::getMotorTorque()
{
    // 获取当前扭矩
    return hand->getTorque();
}

std::vector<uint8_t> LinkerHandApi::getMotorTemperature()
{
    // 获取温度
    return hand->getMotorTemperature();
}

std::vector<uint8_t> LinkerHandApi::getMotorFaultCode()
{
    // 获取故障码
    return hand->getMotorFaultCode();
}

std::vector<uint8_t> LinkerHandApi::getMotorCurrent()
{
    // 获取当前电流
    return hand->getMotorCurrent();
}

// 设置电流 目前仅支持L20
void LinkerHandApi::setMotorCurrent(const std::vector<uint8_t> &current)
{
    if (handJoint_ == LINKER_HAND::L20)
    {
        hand->setMotorCurrent(current);
    }
    else
    {
        std::cout << "目前仅支持L20!" << std::endl;
    }
}

// 设置电机使能 目前仅支持L25
void LinkerHandApi::setMotorEnable(const std::vector<uint8_t> &enable)
{
    std::cout << "目前仅支持L25!" << std::endl;
    if (handJoint_ == LINKER_HAND::L25)
    {
        // hand->setMotorEnable(enable);
    }
}

// 设置电机使能 目前仅支持L25
void LinkerHandApi::setMotorDisable(const std::vector<uint8_t> &disable)
{
    std::cout << "目前仅支持L25!" << std::endl;
    if (handJoint_ == LINKER_HAND::L25)
    {
        // hand->setMotorDisable(disable);
    }
}

// 清除电机故障码 目前仅支持L20
void LinkerHandApi::clearMotorFaultCode(const std::vector<uint8_t> &torque)
{
    
    if (handJoint_ == LINKER_HAND::L20)
    {
        hand->clearMotorFaultCode(torque);
    }
    else
    {
        std::cout << "目前仅支持L20!" << std::endl;
    }
}