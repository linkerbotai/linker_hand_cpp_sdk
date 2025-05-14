#include "LinkerHandL10.h"
#include "CanBus.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

namespace LinkerHandL10
{
// 构造函数
LinkerHand::LinkerHand(uint32_t handId, const std::string &canChannel, int baudrate)
    : handId(handId), bus(canChannel, baudrate), running(true)
{
    thumb_pressure = std::vector<uint8_t>(72, 0);
    index_finger_pressure = std::vector<uint8_t>(72, 0);
    middle_finger_pressure = std::vector<uint8_t>(72, 0);
    ring_finger_pressure = std::vector<uint8_t>(72, 0);
    little_finger_pressure = std::vector<uint8_t>(72, 0);

    // 启动接收线程
    receiveThread = std::thread(&LinkerHand::receiveResponse, this);
    
    bus.send({TOUCH_SENSOR_TYPE}, handId);
    bus.send({FRAME_PROPERTY::THUMB_TOUCH, 0xC6}, handId);
}

// 析构函数
LinkerHand::~LinkerHand()
{
    running = false;
    if (receiveThread.joinable())
    {
        receiveThread.join();
    }
}

// 设置关节位置
void LinkerHand::setJointPositions(const std::vector<u_int8_t> &jointAngles)
{
    if (jointAngles.size() == 10)
    { 
        int JointEffort = 100; // 力度 0～255
        int JointVelocity = 180; // 速度 0～255

        uint8_t send_data[8];

        #if 0
        send_data[0] = static_cast<uint8_t>(JointEffort);
        send_data[1] = static_cast<uint8_t>(JointEffort);
        send_data[2] = static_cast<uint8_t>(JointEffort);
        send_data[3] = static_cast<uint8_t>(JointEffort);
        send_data[4] = static_cast<uint8_t>(JointEffort);
        std::vector<uint8_t> pressure_data_vector(send_data, send_data + 5);
        setTorque(pressure_data_vector);

        send_data[0] = static_cast<uint8_t>(JointVelocity);
        send_data[1] = static_cast<uint8_t>(JointVelocity);
        send_data[2] = static_cast<uint8_t>(JointVelocity);
        send_data[3] = static_cast<uint8_t>(JointVelocity);
        send_data[4] = static_cast<uint8_t>(JointVelocity);
        std::vector<uint8_t> speed_data_vector(send_data, send_data + 5);
        setSpeed(speed_data_vector);
        #endif

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

void LinkerHand::setJointPositionArc(const std::vector<double> &jointAngles)
{
    if (jointAngles.size() == 10)
    {
        if (handId == HAND_TYPE::LEFT) {
            setJointPositions(arc_to_range(10, "left", jointAngles));
        } else if (handId == HAND_TYPE::RIGHT) {
            setJointPositions(arc_to_range(10, "right", jointAngles));
        }
    } else {
        std::cout << "Joint position size is not 10" << std::endl;
    }
}

std::vector<uint8_t> LinkerHand::getCurrentStatus()
{
    // std::vector<uint8_t> result;
    // result = std::vector<uint8_t>(6, 0);
    // result[0] = FRAME_PROPERTY::JOINT_SPEED;
    // bus.send(result, handId);

    // result = std::vector<uint8_t>(7, 0);
    // result[0] = FRAME_PROPERTY::JOINT_POSITION_RCO;
    // bus.send(result, handId);

    // result = std::vector<uint8_t>(5, 0);
    // result[0] = FRAME_PROPERTY::JOINT_POSITION2_RCO;
    // bus.send(result, handId);

    return IHand::getSubVector(joint_position, joint_position2);
}

std::vector<double> LinkerHand::getCurrentStatusArc()
{
    if (handId == HAND_TYPE::LEFT) {
        return range_to_arc(10, "left", getCurrentStatus());
    } else if (handId == HAND_TYPE::RIGHT) {
        return range_to_arc(10, "right", getCurrentStatus());
    }
    return {};
}


// 获取版本号
std::string LinkerHand::getVersion()
{
    bus.send({FRAME_PROPERTY::LINKER_HAND_VERSION}, handId); // 发送获取版本号的命令
    
    std::stringstream ss;

    if (version.size() > 0) 
    {
        // ss << "————————————————————————————————————" << std::endl;
        // ss << "             版本信息" << std::endl;
        // ss << "————————————————————————————————————" << std::endl;
        // ss << "自由度       ：" << (int)version[1] << std::endl;
        // ss << "机械手版本   ：" << (int)version[2] << std::endl;
        // ss << "版本序号     ：" << (int)version[3] << std::endl;
        // if (version[4] == 0x52) {
        //     ss << "手方向       ：右手" << std::endl;
        // } else if (version[4] == 0x4C) {
        //     ss << "手方向       ：左手" << std::endl;
        // }
        // ss << "软件版本号   ：V" << ((int)(version[5] >> 4) + (int)(version[5] & 0x0F) / 10.0) << std::endl;
        // ss << "硬件版本号   ：V" << ((int)(version[6] >> 4) + (int)(version[6] & 0x0F) / 10.0) << std::endl;
        // ss << "————————————————————————————————————" << std::endl;

        ss << "自由度：" << (int)version[1] << std::endl;
        ss << "机械手版本：" << (int)version[2] << std::endl;
        ss << "版本序号：" << (int)version[3] << std::endl;
        if (version[4] == 0x52) {
            ss << "手方向：右手" << std::endl;
        } else if (version[4] == 0x4C) {
            ss << "手方向：左手" << std::endl;
        }
        ss << "软件版本号：v" << ((int)(version[5] >> 4) + (int)(version[5] & 0x0F) / 10.0) << std::endl;
        ss << "硬件版本号：v" << ((int)(version[6] >> 4) + (int)(version[6] & 0x0F) / 10.0) << std::endl;
    }

    return ss.str();
}

void LinkerHand::setTorque(const std::vector<uint8_t> &torque)
{
    if (torque.size() == 5) {
        std::vector<uint8_t> result = {FRAME_PROPERTY::TORQUE_LIMIT};
        result.insert(result.end(), torque.begin(), torque.end());

        bus.send(result, handId);
    } else {
        std::cout << "Torque size is not 5" << std::endl;
    }
}

// 设置关节速度
void LinkerHand::setSpeed(const std::vector<uint8_t> &speed)
{
    if (speed.size() == 5)
    {
        std::vector<uint8_t> result = {FRAME_PROPERTY::JOINT_SPEED};
        result.insert(result.end(), speed.begin(), speed.end());

        joint_speed = result;
        bus.send(result, handId);
    } else {
        std::cout << "Joint speed size is not 5" << std::endl;
    }
}


// 获取关节速度
std::vector<uint8_t> LinkerHand::getSpeed()
{
    bus.send({FRAME_PROPERTY::JOINT_SPEED}, handId);
    return IHand::getSubVector(joint_speed);
}

// 获取所有压感数据
std::vector<std::vector<uint8_t>> LinkerHand::getForce(const int type)
{
    std::vector<std::vector<uint8_t>> result_vec;
    
    if (sensor_type == 0x02) { 
        bus.send({FRAME_PROPERTY::THUMB_TOUCH, 0xC6}, handId);
        bus.send({FRAME_PROPERTY::INDEX_TOUCH, 0xC6}, handId);
        bus.send({FRAME_PROPERTY::MIDDLE_TOUCH, 0xC6}, handId);
        bus.send({FRAME_PROPERTY::RING_TOUCH, 0xC6}, handId);
        bus.send({FRAME_PROPERTY::LITTLE_TOUCH, 0xC6}, handId);

        result_vec.push_back(thumb_pressure);
        result_vec.push_back(index_finger_pressure);
        result_vec.push_back(middle_finger_pressure);
        result_vec.push_back(ring_finger_pressure);
        result_vec.push_back(little_finger_pressure);
    } else {
        if (type == 0) {
            result_vec.push_back(IHand::getSubVector(getThumbForce()));
            result_vec.push_back(IHand::getSubVector(getIndexForce()));
            result_vec.push_back(IHand::getSubVector(getMiddleForce()));
            result_vec.push_back(IHand::getSubVector(getRingForce()));
            result_vec.push_back(IHand::getSubVector(getLittleForce()));
        } else {
            result_vec.push_back(IHand::getSubVector(getNormalForce()));
            result_vec.push_back(IHand::getSubVector(getTangentialForce()));
            result_vec.push_back(IHand::getSubVector(getTangentialForceDir()));
            result_vec.push_back(IHand::getSubVector(getApproachInc()));
        }
    }

    return result_vec;
}

// 获取五指法向压力
std::vector<uint8_t> LinkerHand::getNormalForce()
{
    bus.send({FRAME_PROPERTY::HAND_NORMAL_FORCE}, handId); // 发送请求法向力的命令
    return normal_force;
}

// 获取五指切向压力
std::vector<uint8_t> LinkerHand::getTangentialForce()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE}, handId); // 发送请求切向力的命令
    return tangential_force;
}

// 获取五指切向方向
std::vector<uint8_t> LinkerHand::getTangentialForceDir()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR}, handId); // 发送请求切向力方向的命令
    return tangential_force_dir;
}

// 获取五指接近感应
std::vector<uint8_t> LinkerHand::getApproachInc()
{
    bus.send({FRAME_PROPERTY::HAND_APPROACH_INC}, handId); // 发送请求接近度的命令
    return approach_inc;
}

// -----------------------------------------------------------------------

// 获取当前扭矩
std::vector<uint8_t> LinkerHand::getTorque()
{
    bus.send({FRAME_PROPERTY::TORQUE_LIMIT}, handId);
    return IHand::getSubVector(max_torque);
}

// 获取电机温度
std::vector<uint8_t> LinkerHand::getTemperature()
{
    bus.send({FRAME_PROPERTY::MOTOR_TEMPERATURE_1}, handId);
    bus.send({FRAME_PROPERTY::MOTOR_TEMPERATURE_2}, handId);

    return IHand::getSubVector(motorTemperature_1, motorTemperature_2);
}

// 获取电机故障码
std::vector<uint8_t> LinkerHand::getFaultCode()
{
    bus.send({FRAME_PROPERTY::MOTOR_FAULT_CODE_1}, handId);
    bus.send({FRAME_PROPERTY::MOTOR_FAULT_CODE_2}, handId);
    
    return IHand::getSubVector(motorFaultCode_1, motorFaultCode_2);
}

// 获取电机电流
std::vector<uint8_t> LinkerHand::getCurrent()
{
    return {0,0,0,0,0,0,0,0,0,0};
}

// 获取所有关节位置和压力
std::vector<uint8_t> LinkerHand::requestAllStatus()
{
    bus.send({FRAME_PROPERTY::REQUEST_DATA_RETURN}, handId);
    return {0,0,0,0,0,0,0,0,0,0};
}

// 获取大拇指压感数据
std::vector<uint8_t> LinkerHand::getThumbForce()
{
    bus.send({FRAME_PROPERTY::THUMB_ALL_DATA}, handId); // 获取大拇指指尖传感器的所有数据
        
    return thumb_pressure;
}

// 获取食指压感数据
std::vector<uint8_t> LinkerHand::getIndexForce()
{
    bus.send({FRAME_PROPERTY::INDEX_FINGER_ALL_DATA}, handId); // 获取食指指尖传感器的所有数据
    return index_finger_pressure;
}

// 获取中指压感数据
std::vector<uint8_t> LinkerHand::getMiddleForce()
{
    bus.send({FRAME_PROPERTY::MIDDLE_FINGER_ALL_DATA}, handId); // 获取中指指尖传感器的所有数据
    return middle_finger_pressure;
}

// 获取无名指压感数据
std::vector<uint8_t> LinkerHand::getRingForce()
{
    bus.send({FRAME_PROPERTY::RING_FINGER_ALL_DATA}, handId); // 获取无名指指尖传感器的所有数据
    return ring_finger_pressure;
}

// 获取小拇指压感数据
std::vector<uint8_t> LinkerHand::getLittleForce()
{
    bus.send({FRAME_PROPERTY::LITTLE_FINGER_ALL_DATA}, handId); // 获取小拇指指尖传感器的所有数据
    return little_finger_pressure;
}

void LinkerHand::receiveResponse()
{
    while (running)
    {
        try
        {
            auto data = bus.receive(handId);
            if (data.size() <= 0) continue;
            
            if (RECV_DEBUG)
            {
                std::cout << "L10-Recv: ";
                for (auto &can : data) std::cout << std::hex << (int)can << " ";
                std::cout << std::endl;
            }

            uint8_t frame_property = data[0];
            std::vector<uint8_t> payload(data.begin(), data.end());

            // 新压感数据
            if (frame_property >= THUMB_TOUCH && frame_property <= LITTLE_TOUCH) 
            {
                if (data.size() == 8) {
                    if (sensor_type != 0x02) {
                        sensor_type = 0x02;
                        continue;
                    }
                }
                if (sensor_type == 0x02) {
                    if (data.size() == 8) {
                        uint8_t index = ((data[1] >> 4) + 1) * 6;
                        if (index <= 0x48) {
                            std::vector<uint8_t> payload(data.begin() + 2, data.end());
                            for (uint8_t i = index - 6, p = 0; i < index; ++i, ++p) {
                                if (data[0] == FRAME_PROPERTY::THUMB_TOUCH) thumb_pressure[i] = payload[p];
                                if (data[0] == FRAME_PROPERTY::INDEX_TOUCH) index_finger_pressure[i] = payload[p];
                                if (data[0] == FRAME_PROPERTY::MIDDLE_TOUCH) middle_finger_pressure[i] = payload[p];
                                if (data[0] == FRAME_PROPERTY::RING_TOUCH) ring_finger_pressure[i] = payload[p];
                                if (data[0] == FRAME_PROPERTY::LITTLE_TOUCH) little_finger_pressure[i] = payload[p];
                            }
                        }
                    }
                } else {
                    // std::cout << "sensor type error !" << std::endl;
                }
                continue;
            }

            switch(frame_property) {
                case FRAME_PROPERTY::JOINT_POSITION_RCO: // 关节位置
                    joint_position = payload;
                    break;
                case FRAME_PROPERTY::TORQUE_LIMIT: // 扭矩限制
                    max_torque = payload;
                    break;
                case FRAME_PROPERTY::JOINT_POSITION2_RCO: // 关节位置2
                    joint_position2 = payload;
                    break;
                case FRAME_PROPERTY::JOINT_SPEED: // 关节速度
                    joint_speed = payload;
                    break;
                case FRAME_PROPERTY::THUMB_ALL_DATA: // 大拇指所有压力数据
                    thumb_pressure = payload;
                    break;
                case FRAME_PROPERTY::INDEX_FINGER_ALL_DATA: // 食指所有压力数据
                    index_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_FINGER_ALL_DATA: // 中指所有压力数据
                    middle_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::RING_FINGER_ALL_DATA: // 无名指所有压力数据
                    ring_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_FINGER_ALL_DATA: // 小拇指所有压力数据
                    little_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::TOUCH_SENSOR_TYPE: // 传感器类型
                    if (payload.size() >= 2) {
                        if (payload[1] <= 0x03 && payload[1] >= 0x01) {
                            sensor_type = payload[1];
                        }
                    }
                    break;
                case FRAME_PROPERTY::HAND_NORMAL_FORCE: // 法向压力
                    normal_force = payload;
                    break;
                case FRAME_PROPERTY::HAND_TANGENTIAL_FORCE: // 切向压力
                    tangential_force = payload;
                    break;
                case FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR: // 切向方向
                    tangential_force_dir = payload;
                    break;
                case FRAME_PROPERTY::HAND_APPROACH_INC: // 接近感应
                    approach_inc = payload;
                    break;
                case FRAME_PROPERTY::LINKER_HAND_VERSION: // 版本号
                    version = payload;
                    break;
                case FRAME_PROPERTY::MOTOR_TEMPERATURE_1: // 电机温度
                    motorTemperature_1 = payload;
                    break;
                case FRAME_PROPERTY::MOTOR_TEMPERATURE_2:
                    motorTemperature_2 = payload;
                    break;
                case FRAME_PROPERTY::MOTOR_FAULT_CODE_1: // 电机故障码
                    motorFaultCode_1 = payload;
                    break;
                case FRAME_PROPERTY::MOTOR_FAULT_CODE_2:
                    motorFaultCode_2 = payload;
                    break;
                case FRAME_PROPERTY::REQUEST_DATA_RETURN:
                    break;
                case 0x45:
                    /*
                        can0  027   [1]  05
                        can0  027   [6]  05 00 00 00 00 00
                        can0  028   [1]  05
                        can0  028   [5]  45 52 52 4F 52
                    */
                    break;
                default:
                    std::cout << "L10 未知数据类型: " << std::hex << (int)frame_property << std::endl;
                    continue;
            }
        }
        catch (const std::runtime_error &e)
        {
            // std::cerr << "Error receiving data: " << e.what() << std::endl;
        }
    }
}
}
