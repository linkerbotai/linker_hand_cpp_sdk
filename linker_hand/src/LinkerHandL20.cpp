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

}

// 设置关节位置
void LinkerHand::setJointPositions(const std::vector<u_int8_t> &jointAngles)
{

}

// 设置关节速度
void LinkerHand::setSpeed(const std::vector<uint8_t> &speed)
{

}

// 获取当前速度
std::vector<uint8_t> LinkerHand::getSpeed()
{

}

// 获取当前关节状态
std::vector<uint8_t> LinkerHand::getCurrentStatus()
{

}

//--------------------------------------------------------------------
// 获取所有压感数据
std::vector<std::vector<uint8_t>> LinkerHand::getForce(const int type)
{

}

// 获取大拇指压感数据
std::vector<uint8_t> LinkerHand::getThumbForce()
{

}

// 获取食指压感数据
std::vector<uint8_t> LinkerHand::getIndexForce()
{

}

// 获取中指压感数据
std::vector<uint8_t> LinkerHand::getMiddleForce()
{

}

// 获取无名指压感数据
std::vector<uint8_t> LinkerHand::getRingForce()
{

}

// 获取小拇指压感数据
std::vector<uint8_t> LinkerHand::getLittleForce()
{

}

//--------------------------------------------------------------------
// 获取电机故障码
std::vector<uint8_t> LinkerHand::getMotorFaultCode()
{

}

// 获取电机电流
std::vector<uint8_t> LinkerHand::getMotorCurrent()
{

}

// 清除电机故障码
void LinkerHand::clearMotorFaultCode(const std::vector<uint8_t> &torque)
{

}

// 设置电流
void LinkerHand::setMotorCurrent(const std::vector<uint8_t> &current)
{

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
                    
                    break;
                case FRAME_PROPERTY::JOINT_YAW_R: // 最大压力
                    
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

} // namespace LinkerHandL20
