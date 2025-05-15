#include "LinkerHandL25.h"

namespace LinkerHandL25
{

LinkerHand::LinkerHand(uint32_t handId, const std::string &canChannel, int baudrate, const int currentHandType) : handId(handId), bus(canChannel, baudrate), running(true), current_hand_type(currentHandType)
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
    if (jointAngles.size() == 25)
    {
        std::vector<uint8_t> joint_array;
        joint_array.push_back(jointAngles[10]);
        joint_array.push_back(jointAngles[5]);
        joint_array.push_back(jointAngles[0]);
        joint_array.push_back(jointAngles[15]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[20]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[6]);
        joint_array.push_back(jointAngles[1]);
        joint_array.push_back(jointAngles[16]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[21]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[7]); // 05-12
        joint_array.push_back(jointAngles[2]);
        joint_array.push_back(jointAngles[17]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[22]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[8]);
        joint_array.push_back(jointAngles[3]);
        joint_array.push_back(jointAngles[18]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[23]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[9]);
        joint_array.push_back(jointAngles[4]);
        joint_array.push_back(jointAngles[19]);
        joint_array.push_back(0);
        joint_array.push_back(jointAngles[24]);

        // std::cout << "---------------------------------------" << std::endl;

        int i = 0;
        // 使用列表推导式将列表每6个元素切成一个子数组
        for (auto it = joint_array.begin(); it != joint_array.end(); it += 6)
        {
            std::vector<uint8_t> joint_position(it, it + 6);
            switch(i / 6)
            {
                case 0:
                    joint_position.insert(joint_position.begin(), FRAME_PROPERTY::THUMB_POS);
                    break;
                case 1:
                    joint_position.insert(joint_position.begin(), FRAME_PROPERTY::INDEX_POS);
                    break;
                case 2:
                    joint_position.insert(joint_position.begin(), FRAME_PROPERTY::MIDDLE_POS);
                    break;
                case 3:
                    joint_position.insert(joint_position.begin(), FRAME_PROPERTY::RING_POS);
                    break;
                case 4:
                    joint_position.insert(joint_position.begin(), FRAME_PROPERTY::LITTLE_POS);
                    break;
            }
            bus.send(joint_position, handId);
            i += 6;

            // for (auto &item : joint_position)
            // {
            //     std::cout << std::hex << (int)item << " ";
            // }
        }
        // std::cout << std::endl;
    }  else {
        std::cout << "Joint position size is not 25" << std::endl;
    }
}

void LinkerHand::setJointPositionArc(const std::vector<double> &jointAngles)
{
    if (jointAngles.size() == 25) {
        static int joints_num;
        if (current_hand_type == 0) {// L25
            joints_num = 25;
        } else if (current_hand_type == 1) {// L21
            joints_num = 21;
        }
        if (handId == HAND_TYPE::LEFT) {
            setJointPositions(arc_to_range(joints_num, "left", jointAngles));
        } else if (handId == HAND_TYPE::RIGHT) {
            setJointPositions(arc_to_range(joints_num, "right", jointAngles));
        }
    } else {
        std::cout << "Joint position size is not 25" << std::endl;
    }
}

#if 0
    // 横滚关节位置
    void setRoll(const std::vector<uint8_t> &roll) override;
	// 航向关节位置
    void setYaw(const std::vector<uint8_t> &yaw) override;
    // 指根1关节位置
    void setRoot1(const std::vector<uint8_t> &root1) override;
    // 指根2关节位置
    void setRoot2(const std::vector<uint8_t> &root2) override;
    // 指根3关节位置
    void setRoot3(const std::vector<uint8_t> &root3) override;
    // 指尖关节位置
    void setTip(const std::vector<uint8_t> &tip) override;
#endif

// 将 L25 的状态值转换为 CMD 格式的状态值
std::vector<uint8_t> LinkerHand::state_to_cmd(const std::vector<uint8_t>& l25_state) {
    // L25 CAN 默认接收 30 个数据，初始化 pose 为 25 个 0.0
    std::vector<uint8_t> pose(25, 0.0);  // 原来控制 L25 的指令数据为 25 个

    // 映射关系，字典中存储 l25_state 索引和 pose 索引之间的映射关系
    std::map<int, int> mapping = {
        {0, 10},  {1, 5},   {2, 0},   {3, 15}, {5, 20}, {7, 6},
        {8, 1},   {9, 16},  {11, 21}, {13, 7}, {14, 2}, {15, 17}, {17, 22},
        {19, 8},  {20, 3},  {21, 18}, {23, 23}, {25, 9}, {26, 4},
        {27, 19}, {29, 24}
    };

    // 遍历映射字典，更新 pose 的值
    for (const auto& pair : mapping) {
        int l25_idx = pair.first;
        int pose_idx = pair.second;

        // 确保索引在有效范围内
        if (l25_idx < l25_state.size() && pose_idx < pose.size()) {
            pose[pose_idx] = l25_state[l25_idx];
        }
    }

    std::cout << "state_to_cmd pose.size() : " << pose.size() << std::endl;

    return pose;
}

// 获取当前关节状态
std::vector<uint8_t> LinkerHand::getCurrentStatus()
{
    bus.send({FRAME_PROPERTY::THUMB_POS}, handId);
    bus.send({FRAME_PROPERTY::INDEX_POS}, handId);
    bus.send({FRAME_PROPERTY::MIDDLE_POS}, handId);
    bus.send({FRAME_PROPERTY::RING_POS}, handId);
    bus.send({FRAME_PROPERTY::LITTLE_POS}, handId);

    std::vector<uint8_t> result_vec;

    // 合成一个完整的关节位置数据
    std::vector<uint8_t> joint_position;
    if (thumb_pos.size() > 0 && index_pos.size() > 0 && middle_pos.size() > 0 && ring_pos.size() > 0 && little_pos.size() > 0)
    {
        joint_position.insert(joint_position.end(), thumb_pos.begin() + 1, thumb_pos.end());
        joint_position.insert(joint_position.end(), index_pos.begin() + 1, index_pos.end());
        joint_position.insert(joint_position.end(), middle_pos.begin() + 1, middle_pos.end());
        joint_position.insert(joint_position.end(), ring_pos.begin() + 1, ring_pos.end());
        joint_position.insert(joint_position.end(), little_pos.begin() + 1, little_pos.end());
    }

    if (joint_position.size() == 30) {
        result_vec = state_to_cmd(joint_position);
    }

    return result_vec;
}

std::vector<double> LinkerHand::getCurrentStatusArc()
{
    static int joints_num;
    if (current_hand_type == 0) {// L25
        joints_num = 25;
    } else if (current_hand_type == 1) {// L21
        joints_num = 21;
    }

    std::cout << "joints_num : " << joints_num << std::endl;

    if (handId == HAND_TYPE::LEFT) {
        std::cout << "LEFT range_to_arc" << std::endl;
        return range_to_arc(joints_num, "left", getCurrentStatus());
    } else if (handId == HAND_TYPE::RIGHT) {
        std::cout << "RIGHT range_to_arc" << std::endl;
        return range_to_arc(joints_num, "right", getCurrentStatus());
    }
    return {};
}

#if 0
    // 大拇指指关节位置
    std::vector<uint8_t> getThumb() override;
    // 食指关节位置
    std::vector<uint8_t> getIndex() override;
    // 中指关节位置
    std::vector<uint8_t> getMiddle() override;
    // 无名指关节位置
    std::vector<uint8_t> getRing() override;
    // 小拇指关节位置
    std::vector<uint8_t> getLittle() override;
#endif
// 设置关节速度-有合并指令待确认
void LinkerHand::setSpeed(const std::vector<uint8_t> &speed)
{
    if (speed.size() == 25) {
        // 将数组拆成五个部分
        std::vector<uint8_t> joint_speed1(speed.begin(), speed.begin() + 5);
        std::vector<uint8_t> joint_speed2(speed.begin() + 5, speed.begin() + 10);
        std::vector<uint8_t> joint_speed3(speed.begin() + 10, speed.begin() + 15);
        std::vector<uint8_t> joint_speed4(speed.begin() + 15, speed.begin() + 20);
        std::vector<uint8_t> joint_speed5(speed.begin() + 20, speed.begin() + 25);

        joint_speed1.insert(joint_speed1.begin(), FRAME_PROPERTY::THUMB_SPEED);
        joint_speed2.insert(joint_speed2.begin(), FRAME_PROPERTY::INDEX_SPEED);
        joint_speed3.insert(joint_speed3.begin(), FRAME_PROPERTY::MIDDLE_SPEED);
        joint_speed4.insert(joint_speed4.begin(), FRAME_PROPERTY::RING_SPEED);
        joint_speed5.insert(joint_speed5.begin(), FRAME_PROPERTY::LITTLE_SPEED);

        // 发送数据
        bus.send(joint_speed1, handId);
        bus.send(joint_speed2, handId);
        bus.send(joint_speed3, handId);
        bus.send(joint_speed4, handId);
        bus.send(joint_speed5, handId);
    } else {
        std::cout << "Joint speed size is not 25" << std::endl;
    }
}

#if 0
    // 横滚关节速度
    void setRollSpeed(const std::vector<uint8_t> &rollSpeed) override;
    // 航向关节速度
    void setYawSpeed(const std::vector<uint8_t> &yawSpeed) override;
    // 指根1关节速度
    void setRoot1Speed(const std::vector<uint8_t> &root1Speed) override;
    // 指根2关节速度
    void setRoot2Speed(const std::vector<uint8_t> &root2Speed) override;
    // 指根3关节速度
    void setRoot3Speed(const std::vector<uint8_t> &root3Speed) override;
    // 指尖关节速度
    void setTipSpeed(const std::vector<uint8_t> &tipSpeed) override;
#endif
// 获取当前速度-有合并指令待确认
std::vector<uint8_t> LinkerHand::getSpeed() 
{
    bus.send({FRAME_PROPERTY::THUMB_SPEED}, handId);
    bus.send({FRAME_PROPERTY::INDEX_SPEED}, handId);
    bus.send({FRAME_PROPERTY::MIDDLE_SPEED}, handId);
    bus.send({FRAME_PROPERTY::RING_SPEED}, handId);
    bus.send({FRAME_PROPERTY::LITTLE_SPEED}, handId);

    std::vector<uint8_t> result_vec;

    // 合成一个完整的关节位置数据
    std::vector<uint8_t> joint_speed;

    if (thumb_speed.size() > 0 && index_speed.size() > 0 && middle_speed.size() > 0 && ring_speed.size() > 0 && little_speed.size() > 0) {
        joint_speed.insert(joint_speed.end(), thumb_speed.begin() + 1, thumb_speed.end());
        joint_speed.insert(joint_speed.end(), index_speed.begin() + 1, index_speed.end());
        joint_speed.insert(joint_speed.end(), middle_speed.begin() + 1, middle_speed.end());
        joint_speed.insert(joint_speed.end(), ring_speed.begin() + 1, ring_speed.end());
        joint_speed.insert(joint_speed.end(), little_speed.begin() + 1, little_speed.end());
    }

    if (joint_speed.size() == 30) {
        result_vec = state_to_cmd(joint_speed);
    }

    return result_vec;
}
#if 0
    // 大拇指速度
    std::vector<uint8_t> getThumbSpeed() override;
    // 食指速度
    std::vector<uint8_t> getIndexSpeed() override;
    // 中指速度
    std::vector<uint8_t> getMiddleSpeed() override;
    // 无名指速度
    std::vector<uint8_t> getRingSpeed() override;
    // 小拇指速度
    std::vector<uint8_t> getLittleSpeed() override;
#endif
// 设置扭矩-有合并指令待确认
void LinkerHand::setTorque(const std::vector<uint8_t> &torque) 
{
    if (torque.size() == 25) {
        // 将数组拆成五个部分
        std::vector<uint8_t> joint_torque1(torque.begin(), torque.begin() + 5);
        std::vector<uint8_t> joint_torque2(torque.begin() + 5, torque.begin() + 10);
        std::vector<uint8_t> joint_torque3(torque.begin() + 10, torque.begin() + 15);
        std::vector<uint8_t> joint_torque4(torque.begin() + 15, torque.begin() + 20);
        std::vector<uint8_t> joint_torque5(torque.begin() + 20, torque.begin() + 25);

        joint_torque1.insert(joint_torque1.begin(), FRAME_PROPERTY::THUMB_TORQUE);
        joint_torque2.insert(joint_torque2.begin(), FRAME_PROPERTY::INDEX_TORQUE);
        joint_torque3.insert(joint_torque3.begin(), FRAME_PROPERTY::MIDDLE_TORQUE);
        joint_torque4.insert(joint_torque4.begin(), FRAME_PROPERTY::RING_TORQUE);
        joint_torque5.insert(joint_torque5.begin(), FRAME_PROPERTY::LITTLE_TORQUE);

        // 发送数据
        bus.send(joint_torque1, handId);
        bus.send(joint_torque2, handId);
        bus.send(joint_torque3, handId);
        bus.send(joint_torque4, handId);
        bus.send(joint_torque5, handId);
    } else {
        std::cout << "Joint torque size is not 25" << std::endl;
    }
}
#if 0
    // 横滚关节扭矩
    void setRollTorque(const std::vector<uint8_t> &rollTorque) override;
    // 航向关节扭矩
    void setYawTorque(const std::vector<uint8_t> &yawTorque) override;
    // 指根1关节扭矩
    void setRoot1Torque(const std::vector<uint8_t> &root1Torque) override;
    // 指根2关节扭矩
    void setRoot2Torque(const std::vector<uint8_t> &root2Torque) override;
    // 指根3关节扭矩
    void setRoot3Torque(const std::vector<uint8_t> &root3Torque) override;
    // 指尖关节扭矩
    void setTipTorque(const std::vector<uint8_t> &tipTorque) override;
#endif
// 获取当前扭矩-有合并指令待确认
std::vector<uint8_t> LinkerHand::getTorque() 
{
    bus.send({FRAME_PROPERTY::THUMB_TORQUE}, handId);
    bus.send({FRAME_PROPERTY::INDEX_TORQUE}, handId);
    bus.send({FRAME_PROPERTY::MIDDLE_TORQUE}, handId);
    bus.send({FRAME_PROPERTY::RING_TORQUE}, handId);
    bus.send({FRAME_PROPERTY::LITTLE_TORQUE}, handId);

    std::vector<uint8_t> result_vec;

    // 合成一个完整的关节位置数据
    std::vector<uint8_t> joint_torque;

    if (thumb_torque.size() > 0 && index_torque.size() > 0 && middle_torque.size() > 0 && ring_torque.size() > 0 && little_torque.size() > 0)
    {
        joint_torque.insert(joint_torque.end(), thumb_torque.begin() + 1, thumb_torque.end());
        joint_torque.insert(joint_torque.end(), index_torque.begin() + 1, index_torque.end());
        joint_torque.insert(joint_torque.end(), middle_torque.begin() + 1, middle_torque.end());
        joint_torque.insert(joint_torque.end(), ring_torque.begin() + 1, ring_torque.end());
        joint_torque.insert(joint_torque.end(), little_torque.begin() + 1, little_torque.end());
    }

    if (joint_torque.size() == 30) {
        result_vec = state_to_cmd(joint_torque);
    }

    return result_vec;
}
#if 0
    // 大拇指扭矩
    std::vector<uint8_t> getThumbTorque() override;
    // 食指扭矩
    std::vector<uint8_t> getIndexTorque() override;
    // 中指扭矩
    std::vector<uint8_t> getMiddleTorque() override;
    // 无名指扭矩
    std::vector<uint8_t> getRingTorque() override;
    // 小拇指扭矩
    std::vector<uint8_t> getLittleTorque() override;
#endif
// 获取故障码-有合并指令待确认
std::vector<uint8_t> LinkerHand::getFaultCode() 
{
    bus.send({FRAME_PROPERTY::THUMB_FAULT}, handId);
    bus.send({FRAME_PROPERTY::INDEX_FAULT}, handId);
    bus.send({FRAME_PROPERTY::MIDDLE_FAULT}, handId);
    bus.send({FRAME_PROPERTY::RING_FAULT}, handId);
    bus.send({FRAME_PROPERTY::LITTLE_FAULT}, handId);

    // 合成一个完整的关节位置数据
    std::vector<uint8_t> fault_code;
    if (thumb_fault.size() > 0 && index_fault.size() > 0 && middle_fault.size() > 0 && ring_fault.size() > 0 && little_fault.size() > 0)
    {
        fault_code.insert(fault_code.end(), thumb_fault.begin() + 1, thumb_fault.end());
        fault_code.insert(fault_code.end(), index_fault.begin() + 1, index_fault.end());
        fault_code.insert(fault_code.end(), middle_fault.begin() + 1, middle_fault.end());
        fault_code.insert(fault_code.end(), ring_fault.begin() + 1, ring_fault.end());
        fault_code.insert(fault_code.end(), little_fault.begin() + 1, little_fault.end());
    }
    return fault_code;
}

#if 0
// 清除电机故障码
void LinkerHand::clearFaultCode(const std::vector<uint8_t> &torque) 
{
    
}

    // 大拇指故障码
    std::vector<uint8_t> getThumbFaultCode() override;
    // 食指故障码
    std::vector<uint8_t> getIndexFaultCode() override;
    // 中指故障码
    std::vector<uint8_t> getMiddleFaultCode() override;
    // 无名指故障码
    std::vector<uint8_t> getRingFaultCode() override;
    // 小拇指故障码
    std::vector<uint8_t> getLittleFaultCode() override;
    // ------------------------------------------------以下函数待确认
    // 横滚关节故障码
    std::vector<uint8_t> getRollFaultCode() override;
    // 航向关节故障码
    std::vector<uint8_t> getYawFaultCode() override;
    // 指根1关节故障码
    std::vector<uint8_t> getRoot1FaultCode() override;
    // 指根2关节故障码
    std::vector<uint8_t> getRoot2FaultCode() override;
    // 指根3关节故障码
    std::vector<uint8_t> getRoot3FaultCode() override;
    // 指尖关节故障码
    std::vector<uint8_t> getTipFaultCode() override;
#endif
//--------------------------------------------------------------------
// 获取温度
std::vector<uint8_t> LinkerHand::getTemperature()
{
    bus.send({FRAME_PROPERTY::THUMB_TEMPERATURE}, handId);
    bus.send({FRAME_PROPERTY::INDEX_TEMPERATURE}, handId);
    bus.send({FRAME_PROPERTY::MIDDLE_TEMPERATURE}, handId);
    bus.send({FRAME_PROPERTY::RING_TEMPERATURE}, handId);
    bus.send({FRAME_PROPERTY::LITTLE_TEMPERATURE}, handId);

    // 合成一个完整的关节位置数据
    std::vector<uint8_t> temperature;
    if (thumb_temperature.size() > 0 && index_temperature.size() > 0 && middle_temperature.size() > 0 && ring_temperature.size() > 0 && little_temperature.size() > 0)
    {
        temperature.insert(temperature.end(), thumb_temperature.begin() + 1, thumb_temperature.end());
        temperature.insert(temperature.end(), index_temperature.begin() + 1, index_temperature.end());
        temperature.insert(temperature.end(), middle_temperature.begin() + 1, middle_temperature.end());
        temperature.insert(temperature.end(), ring_temperature.begin() + 1, ring_temperature.end());
        temperature.insert(temperature.end(), little_temperature.begin() + 1, little_temperature.end());
    }
    return temperature;
}
#if 0
    // 大拇指温度
    std::vector<uint8_t> getThumbTemperature() override;
    // 食指温度
    std::vector<uint8_t> getIndexTemperature() override;
    // 中指温度
    std::vector<uint8_t> getMiddleTemperature() override;
    // 无名指温度
    std::vector<uint8_t> getRingTemperature() override;
    // 小拇指温度
    std::vector<uint8_t> getLittleTemperature() override;
    //----------------------------------------------------以下函数待确认
    // 获取横滚关节温度
    std::vector<uint8_t> getRollTemperature() override;
    // 获取航向关节温度
    std::vector<uint8_t> getYawTemperature() override;
    // 获取指根1关节温度
    std::vector<uint8_t> getRoot1Temperature() override;
    // 获取指根2关节温度
    std::vector<uint8_t> getRoot2Temperature() override;
    // 获取指根3关节温度
    std::vector<uint8_t> getRoot3Temperature() override;
    // 获取指尖关节温度
    std::vector<uint8_t> getTipTemperature() override;
#endif
//--------------------------------------------------------------------
// 获取所有压感数据
std::vector<std::vector<uint8_t>> LinkerHand::getForce(const int type) 
{
    std::vector<std::vector<uint8_t>> result_vec;

    if (current_hand_type == 0) // L25
    {
        result_vec.push_back(IHand::getSubVector(getNormalForce()));
        result_vec.push_back(IHand::getSubVector(getTangentialForce()));
        result_vec.push_back(IHand::getSubVector(getTangentialForceDir()));
        result_vec.push_back(IHand::getSubVector(getApproachInc()));
    }

    if (current_hand_type == 1) // L21
    {
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
    }
    
    return result_vec;
}

#if 1
// 获取大拇指压感数据
std::vector<uint8_t> LinkerHand::getThumbForce()
{
    bus.send({FRAME_PROPERTY::THUMB_ALL_DATA}, handId); // 获取大拇指指尖传感器的所有数据
    return thumb_pressure;
}
// 获取食指压感数据
std::vector<uint8_t> LinkerHand::getIndexForce()
{
    bus.send({FRAME_PROPERTY::INDEX_ALL_DATA}, handId); // 获取食指指尖传感器的所有数据
    return index_finger_pressure;
}
// 获取中指压感数据
std::vector<uint8_t> LinkerHand::getMiddleForce()
{
    bus.send({FRAME_PROPERTY::MIDDLE_ALL_DATA}, handId); // 获取中指指尖传感器的所有数据
    return middle_finger_pressure;
}
// 获取无名指压感数据
std::vector<uint8_t> LinkerHand::getRingForce()
{
    bus.send({FRAME_PROPERTY::RING_ALL_DATA}, handId); // 获取无名指指尖传感器的所有数据
    return ring_finger_pressure;
}
// 获取小拇指压感数据
std::vector<uint8_t> LinkerHand::getLittleForce()
{
    bus.send({FRAME_PROPERTY::LITTLE_ALL_DATA}, handId); // 获取小拇指指尖传感器的所有数据
    return little_finger_pressure;
}
#endif
//-----------------------------------------------
// 获取五个手指的法向压力
std::vector<uint8_t> LinkerHand::getNormalForce()
{
    bus.send({FRAME_PROPERTY::HAND_NORMAL_FORCE}, handId);
    return normal_force;
}
// 获取五个手指的切向压力
std::vector<uint8_t> LinkerHand::getTangentialForce()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE}, handId);
    return tangential_force;
}
// 获取五个手指的切向方向
std::vector<uint8_t> LinkerHand::getTangentialForceDir()
{
    bus.send({FRAME_PROPERTY::HAND_TANGENTIAL_FORCE_DIR}, handId);
    return tangential_force_dir;
}
// 获取五个手指指接近感应
std::vector<uint8_t> LinkerHand::getApproachInc()
{
    bus.send({FRAME_PROPERTY::HAND_APPROACH_INC}, handId);
    return approach_inc;
}
//--------------------------------------------------------------------
// 获取版本号
std::string LinkerHand::getVersion()
{
    bus.send({FRAME_PROPERTY::HAND_HARDWARE_VERSION}, handId);
    bus.send({FRAME_PROPERTY::HAND_SOFTWARE_VERSION}, handId);

    // getUID();
    getCommID();

    // std::cout << "handware_version size : " << hand_hardware_version.size() << std::endl;
    // std::cout << "hand_software_version size : " << hand_software_version.size() << std::endl;

    std::stringstream ss;

    if (hand_comm_id.size() >= 2)
    {
        if (hand_comm_id[1] == 0x28) {
            ss << "手方向：左手" << std::endl;
        } else if (hand_comm_id[1] == 0x27) {
            ss << "手方向：右手" << std::endl;
        } else {
            ss << "手方向：未知" << std::endl;
        }
    }
    if (hand_hardware_version.size() == 5)
    {
        ss << "软件版本号：v" << std::hex << (int)hand_hardware_version[1] << "." << (int)hand_hardware_version[2] << "." << (int)hand_hardware_version[3] << "." << (int)hand_hardware_version[4] << std::endl;
    }
    if (hand_software_version.size() == 5)
    {
        ss << "硬件版本号：v" << std::hex << (int)hand_software_version[1] << "." << (int)hand_software_version[2] << "." << (int)hand_software_version[3] << "." << (int)hand_software_version[4] << std::endl;
    }
    return ss.str();
}

void LinkerHand::setMotorEnable(const std::vector<uint8_t> &enable)
{
    std::vector<uint8_t> result = {FRAME_PROPERTY::MOTOR_ENABLE};
    result.insert(result.end(), enable.begin(), enable.end());
    bus.send(result, handId);
}

void LinkerHand::setMotorDisable(const std::vector<uint8_t> &disable)
{
    std::vector<uint8_t> result = {FRAME_PROPERTY::MOTOR_ENABLE};
    result.insert(result.end(), disable.begin(), disable.end());
    bus.send(result, handId);
}

// 设备唯一标识码
std::vector<uint8_t> LinkerHand::getUID()
{
    bus.send({FRAME_PROPERTY::HAND_UID}, handId);
    return hand_uid;
}

std::vector<uint8_t> LinkerHand::getCommID()
{
    bus.send({FRAME_PROPERTY::HAND_COMM_ID}, handId);
    return hand_comm_id;
}

// 恢复出厂设置
void LinkerHand::factoryReset()
{
    bus.send({FRAME_PROPERTY::HAND_FACTORY_RESET}, handId);
}
// 保存参数
void LinkerHand::saveParameter()
{
    bus.send({FRAME_PROPERTY::HAND_SAVE_PARAMETER}, handId);
}

#if 0
    // 动作，预设动作指令
    void playAction(const std::vector<uint8_t> &action) override;
    //--------------------------------------------------------------------
    // 整帧传输
    void setWholeFrame(bool wholeFrame) override;
#endif

    void LinkerHand::receiveResponse()
    {
        while (running)
        {
            try
            {
                auto data = bus.receive(handId);
                if (data.size() <= 0) continue;

				if (RECV_DEBUG) {
                    (current_hand_type == 0) ? std::cout << "L25-Recv: " : std::cout << "L21-Recv: ";
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

                switch (frame_property)
                {
                case FRAME_PROPERTY::THUMB_POS: // 大拇指位置
                    thumb_pos = payload;
                    break;
                case FRAME_PROPERTY::INDEX_POS: // 食指位置
                    index_pos = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_POS: // 中指位置
                    middle_pos = payload;
                    break;
                case FRAME_PROPERTY::RING_POS: // 无名指位置
                    ring_pos = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_POS: // 小拇指位置
                    little_pos = payload;
                    break;
                case FRAME_PROPERTY::THUMB_SPEED: // 大拇指速度
                    thumb_speed = payload;
                    break;
                case FRAME_PROPERTY::INDEX_SPEED: // 食指速度
                    index_speed = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_SPEED: // 中指速度
                    middle_speed = payload;
                    break;
                case FRAME_PROPERTY::RING_SPEED: // 无名指速度
                    ring_speed = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_SPEED: // 小拇指速度
                    little_speed = payload;
                    break;
                case FRAME_PROPERTY::THUMB_TORQUE: // 大拇指扭矩
                    thumb_torque = payload;
                    break;
                case FRAME_PROPERTY::INDEX_TORQUE: // 食指扭矩
                    index_torque = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_TORQUE: // 中指扭矩
                    middle_torque = payload;
                    break;
                case FRAME_PROPERTY::RING_TORQUE: // 无名指扭矩
                    ring_torque = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_TORQUE: // 小拇指扭矩
                    little_torque = payload;
                    break;
                case FRAME_PROPERTY::THUMB_TEMPERATURE: // 大拇指温度
                    thumb_temperature = payload;
                    break;
                case FRAME_PROPERTY::INDEX_TEMPERATURE: // 食指温度
                    index_temperature = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_TEMPERATURE: // 中指温度
                    middle_temperature = payload;
                    break;
                case FRAME_PROPERTY::RING_TEMPERATURE: // 无名指温度
                    ring_temperature = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_TEMPERATURE: // 小拇指温度
                    little_temperature = payload;
                    break;
                case FRAME_PROPERTY::THUMB_FAULT: // 大拇指故障码
                    thumb_fault = payload;
                    break;
                case FRAME_PROPERTY::INDEX_FAULT: // 食指故障码
                    index_fault = payload;
                    break;
                case FRAME_PROPERTY::MIDDLE_FAULT: // 中指故障码
                    middle_fault = payload;
                    break;
                case FRAME_PROPERTY::RING_FAULT: // 无名指故障码
                    ring_fault = payload;
                    break;
                case FRAME_PROPERTY::LITTLE_FAULT: // 小拇指故障码
                    little_fault = payload;
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
                case FRAME_PROPERTY::TOUCH_SENSOR_TYPE: // 传感器类型
                    if (payload.size() >= 2) {
                        if (payload[1] <= 0x03 && payload[1] >= 0x01) {
                            sensor_type = payload[1];
                        }
                    }
                    break;
                case FRAME_PROPERTY::PALM_TOUCH: // 手掌触摸
                    palm_force_data = payload;
                    break;
                case FRAME_PROPERTY::HAND_HARDWARE_VERSION: // 硬件版本
                    hand_hardware_version = payload;
                    break;
                case FRAME_PROPERTY::HAND_SOFTWARE_VERSION: // 软件版本
                    hand_software_version = payload;
                    break;
                case FRAME_PROPERTY::HAND_UID: // 软件版本
                    hand_uid = payload;
                    break;
                case FRAME_PROPERTY::HAND_COMM_ID: // 设备id
                    hand_comm_id = payload;
                    break;
                default:
                    std::cout << "L25 未知数据类型: " << std::hex << (int)frame_property << std::dec << std::endl;
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
