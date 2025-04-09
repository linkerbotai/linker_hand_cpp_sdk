#pragma once

#include <vector>
#include <string>

class IHand
{
public:
    virtual ~IHand() = default;

    virtual void setJointPositions(const std::vector<uint8_t> &jointAngles) = 0;
    virtual std::vector<uint8_t> getCurrentStatus() = 0;
    virtual void setJointSpeed(const std::vector<uint8_t> &speed) = 0;
    virtual void setPressure(const std::vector<uint8_t> &pressure) = 0;
    virtual std::vector<uint8_t> getSpeed() = 0;
    virtual std::vector<std::vector<uint8_t>> getForce() = 0;
    virtual std::vector<std::vector<uint8_t>> getPressureData() = 0;
    virtual void getNormalForce() = 0;
    virtual void getTangentialForce() = 0;
    virtual void getTangentialForceDir() = 0;
    virtual void getApproachInc() = 0;

    virtual std::string getVersion() = 0;
};
