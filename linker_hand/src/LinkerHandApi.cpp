#include "LinkerHandApi.h"

LinkerHandApi::LinkerHandApi(const LINKER_HAND &handJoint, const HAND_TYPE &handType)
{
    if (handType != HAND_TYPE::LEFT && handType != HAND_TYPE::RIGHT)
    {
        throw std::invalid_argument("Unsupported hand type");
    }
    else
    {
        handId = handType;
    }

    if (handJoint == LINKER_HAND::L10)
    {
        hand = HandFactory::createHand(HandType::L10, handId, "can0", 1000000);
    }
    else if (handJoint == LINKER_HAND::L20)
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

void LinkerHandApi::setPressureData(const std::vector<uint8_t> &pressure)
{
    hand->setPressure(pressure);
}

void LinkerHandApi::setSpeed(const std::vector<uint8_t> &speed)
{
    hand->setJointSpeed(speed);
}

void LinkerHandApi::fingerMove(const std::vector<uint8_t> &pose)
{
    hand->setJointPositions(pose);
}

std::vector<uint8_t> LinkerHandApi::getSpeed()
{
    return hand->getSpeed();
}

std::vector<uint8_t> LinkerHandApi::getState()
{
    return hand->getCurrentStatus();
}

std::vector<std::vector<uint8_t>> LinkerHandApi::getForce()
{
    return hand->getForce();
}

std::vector<std::vector<uint8_t>> LinkerHandApi::getPressure()
{
    return hand->getPressureData();
}

std::string LinkerHandApi::getVersion()
{
    return hand->getVersion();
}