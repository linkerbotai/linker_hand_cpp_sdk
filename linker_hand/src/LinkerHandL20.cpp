#include "LinkerHandL20.h"

namespace LinkerHandL20
{

LinkerHand::LinkerHand(uint32_t handId, const std::string &canChannel, int baudrate) : handId(handId), bus(canChannel, baudrate), running(true)
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
    if (jointAngles.size() == 20)
    {
        // 将数组拆成四个部分 R T P Y
        std::vector<uint8_t> joint_position1(jointAngles.begin(), jointAngles.begin() + 5);
        std::vector<uint8_t> joint_position2(jointAngles.begin() + 5, jointAngles.begin() + 10);
        std::vector<uint8_t> joint_position3(jointAngles.begin() + 10, jointAngles.begin() + 15);
        std::vector<uint8_t> joint_position4(jointAngles.begin() + 15, jointAngles.begin() + 20);

        joint_position3.insert(joint_position3.begin(), FRAME_PROPERTY::JOINT_ROLL_R);
        joint_position4.insert(joint_position4.begin(), FRAME_PROPERTY::JOINT_TIP_R);
        joint_position1.insert(joint_position1.begin(), FRAME_PROPERTY::JOINT_PITCH_R);
        joint_position2.insert(joint_position2.begin(), FRAME_PROPERTY::JOINT_YAW_R);

        bus.send(joint_position3, handId);
        bus.send(joint_position4, handId);
        bus.send(joint_position1, handId);
        bus.send(joint_position2, handId);
    } else {
        std::cout << "Joint position size is not 20" << std::endl;
    }
}

void LinkerHand::setJointPositionArc(const std::vector<double> &jointAngles)
{
    if (jointAngles.size() == 20) {
        if (handId == HAND_TYPE::LEFT) {
            setJointPositions(arc_to_range(20, "left", jointAngles));
        } else if (handId == HAND_TYPE::RIGHT) {
            setJointPositions(arc_to_range(20, "right", jointAngles));
        }
    } else {
        std::cout << "Joint position size is not 20" << std::endl;
    }
}

// 设置关节速度
void LinkerHand::setSpeed(const std::vector<uint8_t> &speed)
{
    if (speed.size() == 5) {
        std::vector<uint8_t> data;
        data.push_back(FRAME_PROPERTY::JOINT_SPEED_R);
        data.insert(data.end(), speed.begin(), speed.end());
        bus.send(data, handId); 
    } else {
        std::cout << "Speed size is not 5" << std::endl;
    }
}

// 获取当前速度
std::vector<uint8_t> LinkerHand::getSpeed()
{
    bus.send({FRAME_PROPERTY::JOINT_SPEED_R}, handId);
    return IHand::getSubVector(motor_speed);
}

// 获取当前关节状态
std::vector<uint8_t> LinkerHand::getCurrentStatus()
{
    // bus.send({FRAME_PROPERTY::REQUEST_DATA_RETURN}, handId); // 反馈了 1 2 3
    
    bus.send({FRAME_PROPERTY::JOINT_PITCH_R}, handId);
    bus.send({FRAME_PROPERTY::JOINT_YAW_R}, handId);
    bus.send({FRAME_PROPERTY::JOINT_ROLL_R}, handId);
    bus.send({FRAME_PROPERTY::JOINT_TIP_R}, handId);

    // 合成一个完整的关节位置数据
    std::vector<uint8_t> joint_position;

    std::vector<uint8_t> joint_position1_ = IHand::getSubVector(joint_position1);
    std::vector<uint8_t> joint_position2_ = IHand::getSubVector(joint_position2);
    std::vector<uint8_t> joint_position3_ = IHand::getSubVector(joint_position3);
    std::vector<uint8_t> joint_position4_ = IHand::getSubVector(joint_position4);

    joint_position.insert(joint_position.end(), joint_position1_.begin(), joint_position1_.end());
    joint_position.insert(joint_position.end(), joint_position2_.begin(), joint_position2_.end());
    joint_position.insert(joint_position.end(), joint_position3_.begin(), joint_position3_.end());
    joint_position.insert(joint_position.end(), joint_position4_.begin(), joint_position4_.end());

    return joint_position;
}

std::vector<double> LinkerHand::getCurrentStatusArc()
{
    if (handId == HAND_TYPE::LEFT) {
        return range_to_arc(20, "left", getCurrentStatus());
    } else if (handId == HAND_TYPE::RIGHT) {
        return range_to_arc(20, "right", getCurrentStatus());
    }
    return {};
}

//--------------------------------------------------------------------
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
            result_vec.push_back(IHand::getSubVector(getNormalForce()));
            result_vec.push_back(IHand::getSubVector(getTangentialForce()));
            result_vec.push_back(IHand::getSubVector(getTangentialForceDir()));
            result_vec.push_back(IHand::getSubVector(getApproachInc()));
        } else {
            result_vec.push_back(IHand::getSubVector(getThumbForce()));
            result_vec.push_back(IHand::getSubVector(getIndexForce()));
            result_vec.push_back(IHand::getSubVector(getMiddleForce()));
            result_vec.push_back(IHand::getSubVector(getRingForce()));
            result_vec.push_back(IHand::getSubVector(getLittleForce()));
        }
    }
    return result_vec;
}

#if 0
// 获取大拇指压感数据
std::vector<uint8_t> LinkerHand::getThumbForce()
{
    bus.send({FRAME_PROPERTY::THUMB_ALL_DATA}, handId);
    return IHand::getSubVector(thumb_pressure);
}

// 获取食指压感数据
std::vector<uint8_t> LinkerHand::getIndexForce()
{
    bus.send({FRAME_PROPERTY::INDEX_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(index_finger_pressure);
}

// 获取中指压感数据
std::vector<uint8_t> LinkerHand::getMiddleForce()
{
    bus.send({FRAME_PROPERTY::MIDDLE_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(middle_finger_pressure);
}

// 获取无名指压感数据
std::vector<uint8_t> LinkerHand::getRingForce()
{
    bus.send({FRAME_PROPERTY::RING_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(ring_finger_pressure);
}


// 获取小拇指压感数据
std::vector<uint8_t> LinkerHand::getLittleForce()
{
    bus.send({FRAME_PROPERTY::LITTLE_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(little_finger_pressure);
}
#endif

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
//--------------------------------------------------------------------
// 获取电机故障码
std::vector<uint8_t> LinkerHand::getFaultCode()
{
    bus.send({FRAME_PROPERTY::JOINT_FAULT_R}, handId);
    return IHand::getSubVector(motor_fault_code);
}

// 获取电机电流
std::vector<uint8_t> LinkerHand::getCurrent()
{
    bus.send({FRAME_PROPERTY::JOINT_CURRENT_R}, handId);
    return IHand::getSubVector(motor_current);
}

// 清除电机故障码
void LinkerHand::clearFaultCode(const std::vector<uint8_t> &code)
{
    if (code.size() == 5) {
        std::vector<uint8_t> data;
        data.push_back(FRAME_PROPERTY::JOINT_FAULT_R);
        data.insert(data.end(), code.begin(), code.end());
        bus.send(data, handId);
    } else {
        std::cout << "clearFaultCode size is not 5" << std::endl;
    }
}

// 设置电流
void LinkerHand::setCurrent(const std::vector<uint8_t> &current)
{
    if (current.size() == 5) {
        std::vector<uint8_t> data;
        data.push_back(FRAME_PROPERTY::JOINT_CURRENT_R);
        data.insert(data.end(), current.begin(), current.end());
        bus.send(data, handId);
    } else {
        std::cout << "Current size is not 5" << std::endl;
    }
}

std::string LinkerHand::getVersion()
{
    bus.send({FRAME_PROPERTY::HAND_HARDWARE_VERSION}, handId);
    bus.send({FRAME_PROPERTY::HAND_SOFTWARE_VERSION}, handId);
    bus.send({FRAME_PROPERTY::HAND_COMM_ID}, handId);

    // IHand::getSubVector(hand_software_version, hand_hardware_version);

    std::stringstream ss;

    if (hand_software_version.size() > 0)
    {
        // ss << "————————————————————————————————————" << std::endl;
        // ss << "             版本信息" << std::endl;
        // ss << "————————————————————————————————————" << std::endl;
        // if (hand_comm_id[1] == 0x27)
        // {
        //     ss << "手方向       ：右手" << std::endl;
        // }
        // else if (hand_comm_id[1] == 0x28)
        // {
        //     ss << "手方向       ：左手" << std::endl;
        // }
        // ss << "软件版本号   ：V" << ((int)(hand_software_version[0] >> 4) + (int)(hand_software_version[0] & 0x0F) / 10.0) << std::endl;
        // ss << "硬件版本号   ：V" << ((int)(hand_hardware_version[0] >> 4) + (int)(hand_hardware_version[0] & 0x0F) / 10.0) << std::endl;
        // ss << "————————————————————————————————————" << std::endl;


        if (hand_comm_id[1] == 0x27) {
            ss << "手方向：右手" << std::endl;
        } else if (hand_comm_id[1] == 0x28) {
            ss << "手方向：左手" << std::endl;
        }
        ss << "软件版本号：v" << ((int)(hand_software_version[0] >> 4) + (int)(hand_software_version[0] & 0x0F) / 10.0) << std::endl;
        ss << "硬件版本号：v" << ((int)(hand_hardware_version[0] >> 4) + (int)(hand_hardware_version[0] & 0x0F) / 10.0) << std::endl;
    }
    return ss.str();
}

std::vector<uint8_t> LinkerHand::getUID()
{
    bus.send({FRAME_PROPERTY::HAND_UID}, handId);
    return IHand::getSubVector(hand_uid);
}

std::vector<uint8_t> LinkerHand::getRotorLockCount()
{
    bus.send({FRAME_PROPERTY::ROTOR_LOCK_COUNT}, handId);
    return IHand::getSubVector(rotor_lock_count);
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
                std::cout << "L20-Recv: ";
                for (auto &can : data) std::cout << std::hex << (int)can << std::dec << " ";
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
                case FRAME_PROPERTY::JOINT_PITCH_R: // 关节位置
                    joint_position1 = payload;
                    break;
                case FRAME_PROPERTY::JOINT_YAW_R: // 关节位置
                    joint_position2 = payload;
                    break;
                case FRAME_PROPERTY::JOINT_ROLL_R: // 关节位置
                    joint_position3 = payload;
                    break;
                case FRAME_PROPERTY::JOINT_TIP_R: // 关节位置
                    joint_position4 = payload;
                    break;
                case FRAME_PROPERTY::JOINT_FAULT_R: // 故障码
                    motor_fault_code = payload;
                    break;
                case FRAME_PROPERTY::JOINT_CURRENT_R: // 电流
                    motor_current = payload;
                    break;
                case FRAME_PROPERTY::JOINT_SPEED_R: // 速度
                    motor_speed = payload;
                    break;
                case FRAME_PROPERTY::THUMB_ALL_DATA: // 大拇指
                    thumb_pressure = payload;
                    break;
                case FRAME_PROPERTY::INDEX_FINGER_ALL_DATA: // 食指
                    index_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_FINGER_ALL_DATA: // 中指
                    middle_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::RING_FINGER_ALL_DATA: // 无名指
                    ring_finger_pressure = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_FINGER_ALL_DATA: // 小拇指
                    little_finger_pressure = payload;
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
                case FRAME_PROPERTY::HAND_HARDWARE_VERSION: // 硬件版本
                    hand_hardware_version = payload;
                    break;
                case FRAME_PROPERTY::HAND_SOFTWARE_VERSION: // 软件版本
                    hand_software_version = payload;
                    break;
                case FRAME_PROPERTY::HAND_COMM_ID: // 通信ID
                    hand_comm_id = payload;
                    break;
                case FRAME_PROPERTY::HAND_UID: // 软件版本
                    hand_uid = payload;
                    break;
                case FRAME_PROPERTY::ROTOR_LOCK_COUNT: // 电机锁止次数
                    rotor_lock_count = payload;
                    break;
                default:
                    std::cout << "L20 未知数据类型: " << std::hex << (int)frame_property << std::endl;
                    continue;
            }
        }
        catch (const std::runtime_error &e)
        {
            // std::cerr << "Error receiving data: " << e.what() << std::endl;
        }
    }
}

} // namespace LinkerHandL20
