// LinkerHandL10Can.cpp
#include "LinkerHandL10Can.h"
#include "CanBus.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

// 构造函数
LinkerHandL10Can::LinkerHandL10Can(uint32_t handId, const std::string &canChannel, int baudrate)
    : handId(handId), bus(canChannel, baudrate), running(true)
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
void LinkerHandL10Can::setJointPositions(const std::vector<u_int8_t> &jointAngles)
{
    if (jointAngles.size() == 10)
    { 
        int JointEffort = 100; // 力度 0～255
        int JointVelocity = 180; // 速度 0～255

        uint8_t send_data[8];

        send_data[0] = static_cast<uint8_t>(JointEffort);
        send_data[1] = static_cast<uint8_t>(JointEffort);
        send_data[2] = static_cast<uint8_t>(JointEffort);
        send_data[3] = static_cast<uint8_t>(JointEffort);
        send_data[4] = static_cast<uint8_t>(JointEffort);
        std::vector<uint8_t> pressure_data_vector(send_data, send_data + 5);
        setPressure(pressure_data_vector);

        send_data[0] = static_cast<uint8_t>(JointVelocity);
        send_data[1] = static_cast<uint8_t>(JointVelocity);
        send_data[2] = static_cast<uint8_t>(JointVelocity);
        send_data[3] = static_cast<uint8_t>(JointVelocity);
        send_data[4] = static_cast<uint8_t>(JointVelocity);
        std::vector<uint8_t> speed_data_vector(send_data, send_data + 5);
        setJointSpeed(speed_data_vector);

        
        send_data[0] = FRAME_PROPERTY::JOINT_POSITION_RCO;
        send_data[1] = jointAngles[0];
        send_data[2] = jointAngles[1];
        send_data[3] = jointAngles[2];
        send_data[4] = jointAngles[3];
        send_data[5] = jointAngles[4];
        send_data[6] = jointAngles[5];
        std::vector<uint8_t> position_data_vector(send_data, send_data + 7);
        bus.send(position_data_vector, handId);

        send_data[0] = FRAME_PROPERTY::JOINT_POSITION2_RCO;
        send_data[1] = jointAngles[6];
        send_data[2] = jointAngles[7];
        send_data[3] = jointAngles[8];
        send_data[4] = jointAngles[9];
        std::vector<uint8_t> position2_data_vector(send_data, send_data + 5);
        bus.send(position2_data_vector, handId);

    } else {
        std::cout << "Joint position size is not 10" << std::endl;
    }
}

std::vector<uint8_t> LinkerHandL10Can::getCurrentStatus()
{
    std::vector<uint8_t> result;

    if (joint_position.size() > 2 && joint_position2.size() > 2)
    {
        result.insert(result.end(), joint_position.begin() + 1, joint_position.end());
        result.insert(result.end(), joint_position2.begin() + 1, joint_position2.end());
    }

    return result;
}

// 获取版本号
std::string LinkerHandL10Can::getVersion()
{
    bus.send({FRAME_PROPERTY::LINKER_HAND_VERSION}, handId); // 发送获取版本号的命令

    std::unique_lock<std::mutex> lock(queueMutex);
    // queueCond.wait(lock, [this]{ return !responseQueue.empty(); });
    // if (!queueCond.wait_for(lock, std::chrono::seconds(5), [this] { return !responseQueue.empty(); }))
    if (!queueCond.wait_for(lock, std::chrono::milliseconds(100), [this] { return !responseQueue.empty(); }))
    {
        std::cout << "Timeout waiting for version response" << std::endl;
    }

    std::vector<uint8_t> response = responseQueue.front();
    
    std::stringstream ss;

    if (response.size() > 0) 
    {
        ss << "————————————————————————————————————" << std::endl;
        ss << "             版本信息" << std::endl;
        ss << "————————————————————————————————————" << std::endl;
        ss << "自由度       ：" << (int)response[1] << std::endl;
        ss << "机械手版本   ：" << (int)response[2] << std::endl;
        ss << "版本序号     ：" << (int)response[3] << std::endl;
        if (response[4] == 0x52) {
            ss << "手方向       ：右手" << std::endl;
        } else if (response[4] == 0x4C) {
            ss << "手方向       ：左手" << std::endl;
        }
        ss << "软件版本号   ：V" << ((int)(response[5] >> 4) + (int)(response[5] & 0x0F) / 10.0) << std::endl;
        ss << "硬件版本号   ：V" << ((int)(response[6] >> 4) + (int)(response[6] & 0x0F) / 10.0) << std::endl;
        ss << "————————————————————————————————————" << std::endl;
        responseQueue.pop();
    }

    return ss.str();
}

void LinkerHandL10Can::setPressure(const std::vector<uint8_t> &pressure)
{
    std::vector<uint8_t> result = {FRAME_PROPERTY::MAX_PRESS_RCO};
    result.insert(result.end(), pressure.begin(), pressure.end());

    bus.send(result, handId);
}

// 设置关节速度
void LinkerHandL10Can::setJointSpeed(const std::vector<uint8_t> &speed)
{
    std::vector<uint8_t> result = {FRAME_PROPERTY::JOINT_SPEED};
    result.insert(result.end(), speed.begin(), speed.end());

    joint_speed = result;
    bus.send(result, handId);
}


// 获取关节速度
std::vector<uint8_t> LinkerHandL10Can::getSpeed()
{
    std::vector<uint8_t> result;
    result.insert(result.end(), joint_speed.begin() + 1, joint_speed.end());
    return result;
}

// 获取所有手指的压力数据
std::vector<std::vector<uint8_t>> LinkerHandL10Can::getPressureData()
{
    bus.send({FRAME_PROPERTY::PRESSURE_THUMB}, handId); // 大拇指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_INDEX_FINGER}, handId); // 食指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_MIDDLE_FINGER}, handId); // 中指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_RING_FINGER}, handId); // 无名指所有压力数据
    bus.send({FRAME_PROPERTY::PRESSURE_LITTLE_FINGER}, handId); // 小拇指所有压力数据

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
        std::vector<uint8_t>(pressureData.thumb_pressure.begin(), pressureData.thumb_pressure.end()),
        std::vector<uint8_t>(pressureData.index_finger_pressure.begin(), pressureData.index_finger_pressure.end()),
        std::vector<uint8_t>(pressureData.middle_finger_pressure.begin(), pressureData.middle_finger_pressure.end()),
        std::vector<uint8_t>(pressureData.ring_finger_pressure.begin(), pressureData.ring_finger_pressure.end()),
        std::vector<uint8_t>(pressureData.little_finger_pressure.begin(), pressureData.little_finger_pressure.end())};
}


// 获取法向压力、切向压力、切向方向、接近感应
std::vector<std::vector<uint8_t>> LinkerHandL10Can::getForce()
{
    bus.send({FRAME_PROPERTY::HAND_NORMAL_FORCE}, handId); // 获取法向压力
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE}, handId); // 获取切向压力
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR}, handId); // 获取切向方向
    bus.send({FRAME_PROPERTY::HAND_APPROACH_INC}, handId); // 获取接近感应

    // getNormalForce();
    // getTangentialForce();
    // getTangentialForceDir();
    // getApproachInc();

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
        std::vector<uint8_t>(forceData.normal_force.begin(), forceData.normal_force.end()),
        std::vector<uint8_t>(forceData.tangential_force.begin(), forceData.tangential_force.end()),
        std::vector<uint8_t>(forceData.tangential_force_dir.begin(), forceData.tangential_force_dir.end()),
        std::vector<uint8_t>(forceData.approach_inc.begin(), forceData.approach_inc.end())};
}

// 获取法向压力
void LinkerHandL10Can::getNormalForce()
{
    bus.send({FRAME_PROPERTY::HAND_NORMAL_FORCE}, handId); // 发送请求法向力的命令
}

// 获取切向压力
void LinkerHandL10Can::getTangentialForce()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE}, handId); // 发送请求切向力的命令
}

// 获取切向方向
void LinkerHandL10Can::getTangentialForceDir()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR}, handId); // 发送请求切向力方向的命令
}

// 获取接近感应
void LinkerHandL10Can::getApproachInc()
{
    bus.send({FRAME_PROPERTY::HAND_APPROACH_INC}, handId); // 发送请求接近度的命令
}

void LinkerHandL10Can::receiveResponse()
{
    while (running)
    {
        try
        {
            auto data = bus.receive(handId);
            
            #if 0
            std::cout << "Recv: ";
            for (auto &can : data) std::cout << std::hex << (int)can << " ";
            std::cout << std::endl;
            #endif

            uint8_t frame_property = data[0];
            std::vector<uint8_t> payload(data.begin(), data.end());

            if (frame_property == FRAME_PROPERTY::LINKER_HAND_VERSION) std::lock_guard<std::mutex> lock(queueMutex);
            if (frame_property >= FRAME_PROPERTY::PRESSURE_THUMB && frame_property <= FRAME_PROPERTY::PRESSURE_LITTLE_FINGER) std::lock_guard<std::mutex> lock(pressureQueueMutex);
            if (frame_property >= FRAME_PROPERTY::HAND_NORMAL_FORCE && FRAME_PROPERTY::HAND_APPROACH_INC <= 0x23) std::lock_guard<std::mutex> lock(forceQueueMutex);

            switch(frame_property) {
                case FRAME_PROPERTY::JOINT_POSITION_RCO: // 关节位置
                    joint_position = payload;
                    break;
                case FRAME_PROPERTY::MAX_PRESS_RCO: // 最大压力
                    
                    break;
                case FRAME_PROPERTY::JOINT_POSITION2_RCO: // 关节位置2
                    joint_position2 = payload;
                    break;
                case FRAME_PROPERTY::JOINT_SPEED: // 关节速度
                    joint_speed = payload;
                    break;
                case FRAME_PROPERTY::PRESSURE_THUMB: // 大拇指所有压力数据
                    pressureData.thumb_pressure = payload;
                    break;
                case FRAME_PROPERTY::PRESSURE_INDEX_FINGER: // 食指所有压力数据
                    pressureData.index_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::PRESSURE_MIDDLE_FINGER: // 中指所有压力数据
                    pressureData.middle_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::PRESSURE_RING_FINGER: // 无名指所有压力数据
                    pressureData.ring_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::PRESSURE_LITTLE_FINGER: // 小拇指所有压力数据
                    pressureData.little_finger_pressure = payload;
                    pressureQueue.push(pressureData);
                    pressureQueueCond.notify_one();
                    break;
                case FRAME_PROPERTY::HAND_NORMAL_FORCE: // 法向压力
                    forceData.normal_force = payload;
                    break;
                case FRAME_PROPERTY::HAND_TANGENTIAL_FORCE: // 切向压力
                    forceData.tangential_force = payload;
                    break;
                case FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR: // 切向方向
                    forceData.tangential_force_dir = payload;
                    break;
                case FRAME_PROPERTY::HAND_APPROACH_INC: // 接近感应
                    forceData.approach_inc = payload;
                    forceQueue.push(forceData);
                    forceQueueCond.notify_one();
                    break;
                case FRAME_PROPERTY::LINKER_HAND_VERSION:
                    responseQueue.push(payload);
                    queueCond.notify_one();
                    break;
                default:
                    std::cout << "未知数据类型: " << std::hex << (int)frame_property << std::endl;
                    continue;
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << "Error receiving data: " << e.what() << std::endl;
        }
    }
}
