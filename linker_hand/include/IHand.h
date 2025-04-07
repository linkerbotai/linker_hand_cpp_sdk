#pragma once

#include <vector>
#include <string>

class IHand
{
public:
    virtual ~IHand() = default;

    virtual void setJointPositions(const std::vector<double> &jointAngles) = 0;
    virtual std::vector<double> getCurrentStatus() = 0;
    virtual void setJointSpeed(const std::vector<double> &speed) = 0;
    virtual std::vector<double> getSpeed() = 0;
    virtual std::vector<std::vector<double>> getForce() = 0;
    virtual std::vector<std::vector<double>> getPressureData() = 0;
    virtual void getNormalForce() = 0;
    virtual void getTangentialForce() = 0;
    virtual void getTangentialForceDir() = 0;
    virtual void getApproachInc() = 0;

    virtual std::vector<double> getVersion() = 0;
};
