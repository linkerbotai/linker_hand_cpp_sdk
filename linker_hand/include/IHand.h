#ifndef I_HAND_H
#define I_HAND_H

#include <vector>
#include <string>

class IHand
{
public:
    virtual ~IHand() = default;
    // 设置关节位置
    virtual void setJointPositions(const std::vector<uint8_t> &jointAngles) = 0;
    // 获取速度数据
    virtual std::vector<uint8_t> getSpeed() = 0;
    // 设置关节速度
    virtual void setSpeed(const std::vector<uint8_t> &speed) = 0;
    // 获取当前关节位置
    virtual std::vector<uint8_t> getCurrentStatus() = 0;
    // 获取电机故障码
    virtual std::vector<uint8_t> getMotorFaultCode() = 0;
    // 获取电机电流
    virtual std::vector<uint8_t> getMotorCurrent()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // ------------------------------------------------------
    // 获取压感数据
    virtual std::vector<std::vector<uint8_t>> getForce(const int type) = 0;
    // 获取大拇指压感数据
    virtual std::vector<uint8_t> getThumbForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取食指压感数据
    virtual std::vector<uint8_t> getIndexForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取中指压感数据
    virtual std::vector<uint8_t> getMiddleForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取无名指压感数据
    virtual std::vector<uint8_t> getRingForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取小拇指压感数据
    virtual std::vector<uint8_t> getLittleForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // ------------------------------------------------------
    // 获取五指法向力
    virtual std::vector<uint8_t> getNormalForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取五指切向力
    virtual std::vector<uint8_t> getTangentialForce()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取五指法向力方向
    virtual std::vector<uint8_t> getTangentialForceDir()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取五指接近感应
    virtual std::vector<uint8_t> getApproachInc()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // ------------------------------------------------------
    // 设置扭矩 L20暂不支持
    virtual void setTorque(const std::vector<uint8_t> &torque)
    {
        std::cout << "Not currently supported!" << std::endl;
    }
    // 获取电机扭矩 L20暂不支持
    virtual std::vector<uint8_t> getTorque()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取电机温度 L20暂不支持
    virtual std::vector<uint8_t> getMotorTemperature()
    {
        std::cout << "Not currently supported!" << std::endl;
        return {0};
    }
    // 获取版本号   目前仅支持L10
    virtual std::string getVersion()
    {
        return "Not currently supported!";
    }
    // 获取设备ID L20协议
    virtual std::vector<uint8_t> getUID()
    {
        return {0};
    }
    // 手指堵转或过流判断计数阀值 L20协议
    virtual std::vector<uint8_t> getRotorLockCount()
    {
        return {0};
    }
    // 清除电机故障码 目前仅支持L20
	virtual void clearMotorFaultCode(const std::vector<uint8_t> &torque)
    {
        std::cout << "Not currently supported!" << std::endl;
    }
    // 设置电流 目前仅支持L20 
	virtual void setMotorCurrent(const std::vector<uint8_t> &current)
    {
        std::cout << "Not currently supported!" << std::endl;
    }
	// 设置电机使能 目前仅支持L25
	virtual void setMotorEnable(const std::vector<uint8_t> &enable)
    {
        std::cout << "Not currently supported!" << std::endl;
    }
	// 设置电机使能 目前仅支持L25
	virtual void setMotorDisable(const std::vector<uint8_t> &disable)
    {
        std::cout << "Not currently supported!" << std::endl;
    }

    // ---------------------------------------------------------------------

    virtual std::vector<uint8_t> getSubVector(const std::vector<uint8_t> &vec)
    {
        std::vector<uint8_t> result;
        if (vec.size() > 1) result.insert(result.end(), vec.begin() + 1, vec.end());
        return result;
    }

    virtual std::vector<uint8_t> getSubVector(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2)
    {
        std::vector<uint8_t> result;
        if (vec1.size() > 0) result.insert(result.end(), vec1.begin() + 1, vec1.end());
        if (vec2.size() > 0) result.insert(result.end(), vec2.begin() + 1, vec2.end());
        return result;
    }
};
#endif // I_HAND_H
