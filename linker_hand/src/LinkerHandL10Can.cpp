// LinkerHandL10Can.cpp
#include "LinkerHandL10Can.h"
#include "CanBus.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

// 构造函数
LinkerHandL10Can::LinkerHandL10Can(uint32_t canId, const std::string &canChannel, int baudrate)
    : canId(canId), bus(canChannel, baudrate), running(true)
{
    // 启动接收线程
    receiveThread = std::thread(&LinkerHandL10Can::receiveResponse, this);
}

// 析构函数
LinkerHandL10Can::~LinkerHandL10Can()
{
    running = false;
    if (receiveThread.joinable())
    {
        receiveThread.join();
    }
}

// 设置关节位置
void LinkerHandL10Can::setJointPositions(const std::vector<double> &jointAngles)
{
    // std::vector<uint8_t> data(jointAngles.begin(), jointAngles.end());

    int JointVelocity = 100;

    uint8_t send_data[8];
    send_data[0] = 0x01;
    send_data[1] = static_cast<uint8_t>(jointAngles[0]);
    send_data[2] = static_cast<uint8_t>(jointAngles[1]);
    send_data[3] = static_cast<uint8_t>(jointAngles[2]);
    send_data[4] = static_cast<uint8_t>(jointAngles[3]);
    send_data[5] = static_cast<uint8_t>(jointAngles[4]);
    send_data[6] = static_cast<uint8_t>(jointAngles[5]);
    // CanSendDataDeal(ID, send_data, 7);
    bus.send(send_data, 7, canId);

    send_data[0] = 0x04;
    send_data[1] = static_cast<uint8_t>(jointAngles[6]);
    send_data[2] = static_cast<uint8_t>(jointAngles[7]);
    send_data[3] = static_cast<uint8_t>(jointAngles[8]);
    send_data[4] = static_cast<uint8_t>(jointAngles[9]);
    // CanSendDataDeal(ID, send_data, 5);
    bus.send(send_data, 5, canId);

    send_data[0] = 0x05;
    send_data[1] = static_cast<uint8_t>(JointVelocity);
    send_data[2] = static_cast<uint8_t>(JointVelocity);
    send_data[3] = static_cast<uint8_t>(JointVelocity);
    send_data[4] = static_cast<uint8_t>(JointVelocity);
    send_data[5] = static_cast<uint8_t>(JointVelocity);
    // CanSendDataDeal(ID, send_data, 6);
    bus.send(send_data, 6, canId);
}

std::vector<double> LinkerHandL10Can::getCurrentStatus()
{
}

// 获取当前状态
std::vector<double> LinkerHandL10Can::getVersion()
{
    bus.send({0x64}, canId); // 发送请求当前状态的命令

    std::unique_lock<std::mutex> lock(queueMutex);
    queueCond.wait(lock, [this]
                   { return !responseQueue.empty(); });

    std::vector<uint8_t> response = responseQueue.front();
    responseQueue.pop();

    return std::vector<double>(response.begin(), response.end());
}

// 设置关节速度
void LinkerHandL10Can::setJointSpeed(const std::vector<double> &speed)
{
    std::vector<uint8_t> data(speed.begin(), speed.end());
    bus.send(data, canId);
}

// 获取关节速度
std::vector<double> LinkerHandL10Can::getSpeed()
{
    bus.send({0x05}, canId); // 发送请求速度的命令
    auto response = bus.receive(canId);
    return std::vector<double>(response.begin(), response.end());
}

// 获取所有手指的压力数据
std::vector<std::vector<double>> LinkerHandL10Can::getPressureData()
{		
    bus.send({0x28}, canId); // 大拇指所有压力数据
    bus.send({0x29}, canId); // 食指所有压力数据
    bus.send({0x30}, canId); // 中指所有压力数据
    bus.send({0x31}, canId); // 无名指所有压力数据
    bus.send({0x32}, canId); // 小拇指所有压力数据
}


// 获取传感器数据
std::vector<std::vector<double>> LinkerHandL10Can::getForce()
{
    // 发送请求传感器数据的命令
    bus.send({0x20}, canId); // 获取法向压力
    bus.send({0x21}, canId); // 获取切向压力
    bus.send({0x22}, canId); // 获取切向方向
    bus.send({0x23}, canId); // 获取接近感应

    std::unique_lock<std::mutex> lock(forceQueueMutex);
    forceQueueCond.wait(lock, [this]
                        { return !forceQueue.empty(); });

    ForceData forceData = forceQueue.front();
    forceQueue.pop();

    return {
        std::vector<double>(forceData.normal_force.begin(), forceData.normal_force.end()),
        std::vector<double>(forceData.tangential_force.begin(), forceData.tangential_force.end()),
        std::vector<double>(forceData.tangential_force_dir.begin(), forceData.tangential_force_dir.end()),
        std::vector<double>(forceData.approach_inc.begin(), forceData.approach_inc.end())};
}

// 获取法向压力
void LinkerHandL10Can::getNormalForce()
{
    bus.send({0x20}, canId); // 发送请求法向力的命令
}

// 获取切向压力
void LinkerHandL10Can::getTangentialForce()
{
    bus.send({0x21}, canId); // 发送请求切向力的命令
}

// 获取切向方向
void LinkerHandL10Can::getTangentialForceDir()
{
    bus.send({0x22}, canId); // 发送请求切向力方向的命令
}

// 获取接近感应
void LinkerHandL10Can::getApproachInc()
{
    bus.send({0x23}, canId); // 发送请求接近度的命令
}

void LinkerHandL10Can::receiveResponse()
{
    while (running)
    {
        try
        {
            auto data = bus.receive(canId);

#if 0
            std::cout << "receive : ";
            for (auto &can : data)
            {
            	std::cout << std::hex << (int)can << " ";
            }
            std::cout << std::endl;
#endif

            uint8_t dataType = data[0];
            // std::cout << "dataType:" << std::hex << (int)dataType << std::endl;

            if (dataType == 0x64)
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                responseQueue.push(data);
                queueCond.notify_one();
            }

#if 1
            if (dataType >= 0x20 && dataType <= 0x23)
            {
                std::vector<uint8_t> payload(data.begin(), data.end());
                {
                    std::lock_guard<std::mutex> lock(forceQueueMutex);
                    switch (dataType)
                    {
                    case 0x20: // 法向压力
                        forceData.normal_force = payload;
                        break;
                    case 0x21: // 切向压力
                        forceData.tangential_force = payload;
                        break;
                    case 0x22: // 切向方向
                        forceData.tangential_force_dir = payload;
                        break;
                    case 0x23: // 接近感应
                        forceData.approach_inc = payload;
                        break;
                    default:
                        // 未知数据类型，忽略
                        continue;
                    }

                    if (dataType == 0x23)
                    {
                        forceQueue.push(forceData);
                        forceQueueCond.notify_one();
                    }
                }
            }
#endif
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Error receiving data: " << e.what() << std::endl;
        }
    }
}
