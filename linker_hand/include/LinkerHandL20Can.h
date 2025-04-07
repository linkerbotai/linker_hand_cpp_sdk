#pragma once

#include "IHand.h"
// #include "CanBus.h"

class LinkerHandL20Can : public IHand {
public:
    LinkerHandL20Can(uint32_t canId, const std::string& canChannel, int baudrate);
    ~LinkerHandL20Can();

    void setJointPositions(const std::vector<double>& jointAngles) override;
    std::vector<double> getCurrentStatus() override;
    void setJointSpeed(const std::vector<double>& speed) override;
    std::vector<double> getSpeed() override;
    std::vector<std::vector<double>> getForce() override;
    void getNormalForce() override;
    void getTangentialForce() override;
    void getTangentialForceDir() override;
    void getApproachInc() override;
    std::vector<std::vector<double>> getPressureData(); override;

private:
    uint32_t canId;
    // CanBus bus;
    std::thread receiveThread;
    bool running;
    std::mutex responseMutex;

    void receiveResponse();
};
