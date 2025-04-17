#include "LinkerHandL20.h"

namespace LinkerHandL20
{

LinkerHand::LinkerHand(uint32_t handId, const std::string &canChannel, int baudrate) : handId(handId), bus(canChannel, baudrate), running(true)
{
    // 启动接收线程
    receiveThread = std::thread(&LinkerHand::receiveResponse, this);
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
}

// 设置关节速度
void LinkerHand::setSpeed(const std::vector<uint8_t> &speed)
{
    std::vector<uint8_t> data;
    data.push_back(FRAME_PROPERTY::JOINT_SPEED_R);
    data.insert(data.end(), speed.begin(), speed.end());
    bus.send(data, handId);
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

//--------------------------------------------------------------------
// 获取所有压感数据
std::vector<std::vector<uint8_t>> LinkerHand::getForce(const int type)
{
    std::vector<std::vector<uint8_t>> result_vec;
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
    return result_vec;
}

#if 0
// 获取大拇指压感数据
std::vector<uint8_t> LinkerHand::getThumbForce()
{
    bus.send({FRAME_PROPERTY::THUMB_ALL_DATA}, handId);
    return IHand::getSubVector(thumb_force_data);
}

// 获取食指压感数据
std::vector<uint8_t> LinkerHand::getIndexForce()
{
    bus.send({FRAME_PROPERTY::INDEX_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(index_force_data);
}

// 获取中指压感数据
std::vector<uint8_t> LinkerHand::getMiddleForce()
{
    bus.send({FRAME_PROPERTY::MIDDLE_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(middle_force_data);
}

// 获取无名指压感数据
std::vector<uint8_t> LinkerHand::getRingForce()
{
    bus.send({FRAME_PROPERTY::RING_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(ring_force_data);
}


// 获取小拇指压感数据
std::vector<uint8_t> LinkerHand::getLittleForce()
{
    bus.send({FRAME_PROPERTY::LITTLE_FINGER_ALL_DATA}, handId);
    return IHand::getSubVector(little_force_data);
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
std::vector<uint8_t> LinkerHand::getMotorFaultCode()
{
    bus.send({FRAME_PROPERTY::JOINT_FAULT_R}, handId);
    return IHand::getSubVector(motor_fault_code);
}

// 获取电机电流
std::vector<uint8_t> LinkerHand::getMotorCurrent()
{
    bus.send({FRAME_PROPERTY::JOINT_CURRENT_R}, handId);
    return IHand::getSubVector(motor_current);
}

// 清除电机故障码
void LinkerHand::clearMotorFaultCode(const std::vector<uint8_t> &torque)
{
    std::vector<uint8_t> data;
    data.push_back(FRAME_PROPERTY::JOINT_FAULT_R);
    data.insert(data.end(), torque.begin(), torque.end());
    bus.send(data, handId);
}

// 设置电流
void LinkerHand::setMotorCurrent(const std::vector<uint8_t> &current)
{
    std::vector<uint8_t> data;
    data.push_back(FRAME_PROPERTY::JOINT_CURRENT_R);
    data.insert(data.end(), current.begin(), current.end());
    bus.send(data, handId);
}

std::string LinkerHand::getVersion()
{
    bus.send({FRAME_PROPERTY::HAND_HARDWARE_VERSION}, handId);
    bus.send({FRAME_PROPERTY::HAND_SOFTWARE_VERSION}, handId);
    bus.send({FRAME_PROPERTY::HAND_COMM_ID}, handId);

    IHand::getSubVector(hand_software_version, hand_hardware_version);

    std::stringstream ss;

    if (hand_software_version.size() > 0)
    {
        ss << "————————————————————————————————————" << std::endl;
        ss << "             版本信息" << std::endl;
        ss << "————————————————————————————————————" << std::endl;
        if (hand_comm_id[1] == 0x27)
        {
            ss << "手方向       ：右手" << std::endl;
        }
        else if (hand_comm_id[1] == 0x28)
        {
            ss << "手方向       ：左手" << std::endl;
        }
        ss << "软件版本号   ：V" << ((int)(hand_software_version[0] >> 4) + (int)(hand_software_version[0] & 0x0F) / 10.0) << std::endl;
        ss << "硬件版本号   ：V" << ((int)(hand_hardware_version[0] >> 4) + (int)(hand_hardware_version[0] & 0x0F) / 10.0) << std::endl;
        ss << "————————————————————————————————————" << std::endl;
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
            
            #if 0
            std::cout << "Recv: ";
            for (auto &can : data) std::cout << std::hex << (int)can << " ";
            std::cout << std::endl;
            #endif

            if (data.size() <= 0) continue;  
            uint8_t frame_property = data[0];
            std::vector<uint8_t> payload(data.begin(), data.end());

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
                    thumb_force_data = payload;
                    break;
                case FRAME_PROPERTY::INDEX_FINGER_ALL_DATA: // 食指
                    index_force_data = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_FINGER_ALL_DATA: // 中指
                    middle_force_data = payload;
                    break;
                case FRAME_PROPERTY::RING_FINGER_ALL_DATA: // 无名指
                    ring_force_data = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_FINGER_ALL_DATA: // 小拇指
                    little_force_data = payload;
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
                    std::cout << "未知数据类型: " << std::hex << (int)frame_property << std::endl;
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
