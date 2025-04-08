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

    int JointVelocity = 180;

    uint8_t send_data[8];
    
    send_data[0] = FRAME_PROPERTY::JOINT_POSITION_RCO;
    send_data[1] = static_cast<uint8_t>(jointAngles[0]);
    send_data[2] = static_cast<uint8_t>(jointAngles[1]);
    send_data[3] = static_cast<uint8_t>(jointAngles[2]);
    send_data[4] = static_cast<uint8_t>(jointAngles[3]);
    send_data[5] = static_cast<uint8_t>(jointAngles[4]);
    send_data[6] = static_cast<uint8_t>(jointAngles[5]);
    // CanSendDataDeal(ID, send_data, 7);
    bus.send(send_data, 7, canId);

    send_data[0] = FRAME_PROPERTY::JOINT_POSITION2_RCO;
    send_data[1] = static_cast<uint8_t>(jointAngles[6]);
    send_data[2] = static_cast<uint8_t>(jointAngles[7]);
    send_data[3] = static_cast<uint8_t>(jointAngles[8]);
    send_data[4] = static_cast<uint8_t>(jointAngles[9]);
    // CanSendDataDeal(ID, send_data, 5);
    bus.send(send_data, 5, canId);

    send_data[0] = FRAME_PROPERTY::JOINT_SPEED;
    send_data[1] = static_cast<uint8_t>(JointVelocity);
    send_data[2] = static_cast<uint8_t>(JointVelocity);
    send_data[3] = static_cast<uint8_t>(JointVelocity);
    send_data[4] = static_cast<uint8_t>(JointVelocity);
    send_data[5] = static_cast<uint8_t>(JointVelocity);
    // CanSendDataDeal(ID, send_data, 6);
    bus.send(send_data, 6, canId);
    
    setJointSpeed({send_data[0], send_data[1], send_data[2], send_data[3], send_data[4], send_data[5]});
}

std::vector<double> LinkerHandL10Can::getCurrentStatus()
{
}

// 获取版本号
std::vector<double> LinkerHandL10Can::getVersion()
{
    bus.send({FRAME_PROPERTY::LINKER_HAND_VERSION}, canId); // 发送获取版本号的命令

    std::unique_lock<std::mutex> lock(queueMutex);
    // queueCond.wait(lock, [this]{ return !responseQueue.empty(); });
    // if (!queueCond.wait_for(lock, std::chrono::seconds(5), [this] { return !responseQueue.empty(); }))
    if (!queueCond.wait_for(lock, std::chrono::milliseconds(100), [this] { return !responseQueue.empty(); }))
    {
        std::cout << "Timeout waiting for version response" << std::endl;
        // throw std::runtime_error("Timeout waiting for version response");
    }

    std::vector<uint8_t> response = responseQueue.front();
    if (response.size() > 0) responseQueue.pop();

    return std::vector<double>(response.begin(), response.end());
}

// 设置关节速度
void LinkerHandL10Can::setJointSpeed(const std::vector<int> &speed)
{
    std::vector<uint8_t> data(speed.begin(), speed.end());
    bus.send(data, canId);
}

// 获取关节速度
std::vector<double> LinkerHandL10Can::getSpeed()
{
    bus.send({FRAME_PROPERTY::JOINT_SPEED}, canId); // 发送请求速度的命令
    auto response = bus.receive(canId);
    return std::vector<double>(response.begin(), response.end());
}

// 获取所有手指的压力数据
std::vector<std::vector<double>> LinkerHandL10Can::getPressureData()
{
    bus.send({FRAME_PROPERTY::PRESSURE_THUMB}, canId); // 大拇指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_INDEX_FINGER}, canId); // 食指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_MIDDLE_FINGER}, canId); // 中指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_RING_FINGER}, canId); // 无名指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_LITTLE_FINGER}, canId); // 小拇指所有压力数据

    std::unique_lock<std::mutex> lock(pressureQueueMutex);
    if (!pressureQueueCond.wait_for(lock, std::chrono::milliseconds(100), [this] { return !pressureQueue.empty(); }))
    {
        std::cout << "Timeout waiting for PressureData response" << std::endl;
        // throw std::runtime_error("Timeout waiting for PressureData response");
    }

    PressureData pressureData = pressureQueue.front();

    // 判断所有不为空
    if (pressureData.thumb_pressure.size() > 0 && 
        pressureData.index_finger_pressure.size() > 0 && 
        pressureData.middle_finger_pressure.size() > 0 && 
        pressureData.ring_finger_pressure.size() > 0 && 
        pressureData.little_finger_pressure.size() > 0)
    {
        pressureQueue.pop();
    }

    return {
        std::vector<double>(pressureData.thumb_pressure.begin(), pressureData.thumb_pressure.end()),
        std::vector<double>(pressureData.index_finger_pressure.begin(), pressureData.index_finger_pressure.end()),
        std::vector<double>(pressureData.middle_finger_pressure.begin(), pressureData.middle_finger_pressure.end()),
        std::vector<double>(pressureData.ring_finger_pressure.begin(), pressureData.ring_finger_pressure.end()),
        std::vector<double>(pressureData.little_finger_pressure.begin(), pressureData.little_finger_pressure.end())};
}


// 获取法向压力、切向压力、切向方向、接近感应
std::vector<std::vector<double>> LinkerHandL10Can::getForce()
{
    // bus.send({FRAME_PROPERTY::HAND_NORMAL_FORCE}, canId); // 获取法向压力
    // bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE}, canId); // 获取切向压力
    // bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR}, canId); // 获取切向方向
    // bus.send({FRAME_PROPERTY::HAND_APPROACH_INC}, canId); // 获取接近感应

    getNormalForce();
    getTangentialForce();
    getTangentialForceDir();
    getApproachInc();

    std::unique_lock<std::mutex> lock(forceQueueMutex);
    if (!forceQueueCond.wait_for(lock, std::chrono::milliseconds(100), [this] { return !forceQueue.empty(); }))
    {
        std::cout << "Timeout waiting for force response" << std::endl;
        // throw std::runtime_error("Timeout waiting for force response");
    }

    ForceData forceData = forceQueue.front();
    // 判断所有不为空
    if (forceData.normal_force.size() > 0 && 
        forceData.tangential_force.size() > 0 && 
        forceData.tangential_force_dir.size() > 0 && 
        forceData.approach_inc.size() > 0)
    {
        forceQueue.pop();
    }

    return {
        std::vector<double>(forceData.normal_force.begin(), forceData.normal_force.end()),
        std::vector<double>(forceData.tangential_force.begin(), forceData.tangential_force.end()),
        std::vector<double>(forceData.tangential_force_dir.begin(), forceData.tangential_force_dir.end()),
        std::vector<double>(forceData.approach_inc.begin(), forceData.approach_inc.end())};
}

// 获取法向压力
void LinkerHandL10Can::getNormalForce()
{
    bus.send({FRAME_PROPERTY::HAND_NORMAL_FORCE}, canId); // 发送请求法向力的命令
}

// 获取切向压力
void LinkerHandL10Can::getTangentialForce()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE}, canId); // 发送请求切向力的命令
}

// 获取切向方向
void LinkerHandL10Can::getTangentialForceDir()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR}, canId); // 发送请求切向力方向的命令
}

// 获取接近感应
void LinkerHandL10Can::getApproachInc()
{
    bus.send({FRAME_PROPERTY::HAND_APPROACH_INC}, canId); // 发送请求接近度的命令
}

void LinkerHandL10Can::receiveResponse()
{
    while (running)
    {
        try
        {
            auto data = bus.receive(canId);

#if 1
            std::cout << "Recv: ";
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

            if (dataType >= 0x28 && dataType <= 0x32)
            {
                std::vector<uint8_t> payload(data.begin(), data.end());
                {
                    std::lock_guard<std::mutex> lock(forceQueueMutex);
                    switch (dataType)
                    {
                    case 0x28: // 大拇指所有压力数据
                        pressureData.thumb_pressure = payload;
                        break;
                    case 0x29: // 食指所有压力数据
                        pressureData.index_finger_pressure = payload;
                        break;
                    case 0x30: // 中指所有压力数据
                        pressureData.middle_finger_pressure = payload;
                        break;
                    case 0x31: // 无名指所有压力数据
                        pressureData.ring_finger_pressure = payload;
                        break;
                    case 0x32: // 小拇指所有压力数据
                        pressureData.little_finger_pressure = payload;
                        break;
                    default:
                        // 未知数据类型，忽略
                        continue;
                    }

                    if (dataType == 0x32)
                    {
                        pressureQueue.push(pressureData);
                        pressureQueueCond.notify_one();
                    }
                }
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
