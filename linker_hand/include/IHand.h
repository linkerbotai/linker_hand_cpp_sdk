#ifndef I_HAND_H
#define I_HAND_H

#include <set>
#include <vector>
#include <string>
#include <iostream>
#include "RangeToArc.h"

#define RECV_DEBUG 0

typedef enum {
    L7,
	L10,
    L20,
    L21,
	L25
} LINKER_HAND;

typedef enum {
    RIGHT = 0x27,
    LEFT = 0x28
} HAND_TYPE;

class IHand
{
public:
    virtual ~IHand() = default;
    // 设置关节位置
    virtual void setJointPositions(const std::vector<uint8_t> &jointAngles) = 0;
    virtual void setJointPositionArc(const std::vector<double> &jointAngles)
    {
        (void) jointAngles;
        printUnsupportedFeature("setJointPositionArc");
    }
    // 获取速度数据
    virtual std::vector<uint8_t> getSpeed() = 0;
    // 设置关节速度
    virtual void setSpeed(const std::vector<uint8_t> &speed) = 0;
    // 获取当前关节位置
    virtual std::vector<uint8_t> getCurrentStatus() = 0;
    virtual std::vector<double> getCurrentStatusArc()
    {
        printUnsupportedFeature("getCurrentPositionArc");
        return {};
    }
    // 获取电机故障码
    virtual std::vector<uint8_t> getFaultCode() = 0;
    // 获取电机电流
    virtual std::vector<uint8_t> getCurrent()
    {
        printUnsupportedFeature("getCurrent");
        return {};
    }
    // ------------------------------------------------------
    // 获取压感数据
    virtual std::vector<std::vector<uint8_t>> getForce(const int type) = 0;
    // 获取大拇指压感数据
    virtual std::vector<uint8_t> getThumbForce()
    {
        printUnsupportedFeature("getThumbForce");
        return {};
    }
    // 获取食指压感数据
    virtual std::vector<uint8_t> getIndexForce()
    {
        printUnsupportedFeature("getIndexForce");
        return {};
    }
    // 获取中指压感数据
    virtual std::vector<uint8_t> getMiddleForce()
    {
        printUnsupportedFeature("getMiddleForce");
        return {};
    }
    // 获取无名指压感数据
    virtual std::vector<uint8_t> getRingForce()
    {
        printUnsupportedFeature("getRingForce");
        return {};
    }
    // 获取小拇指压感数据
    virtual std::vector<uint8_t> getLittleForce()
    {
        printUnsupportedFeature("getLittleForce");
        return {};
    }
    // ------------------------------------------------------
    // 获取五指法向力
    virtual std::vector<uint8_t> getNormalForce()
    {
        printUnsupportedFeature("getNormalForce");
        return {};
    }
    // 获取五指切向力
    virtual std::vector<uint8_t> getTangentialForce()
    {
        printUnsupportedFeature("getTangentialForce");
        return {};
    }
    // 获取五指法向力方向
    virtual std::vector<uint8_t> getTangentialForceDir()
    {
        printUnsupportedFeature("getTangentialForceDir");
        return {};
    }
    // 获取五指接近感应
    virtual std::vector<uint8_t> getApproachInc()
    {
        printUnsupportedFeature("getApproachInc");
        return {};
    }
    // ------------------------------------------------------
    // 设置扭矩 L20暂不支持
    virtual void setTorque(const std::vector<uint8_t> &torque)
    {
        (void)torque;
        printUnsupportedFeature("setTorque");
    }
    // 获取电机扭矩 L20暂不支持
    virtual std::vector<uint8_t> getTorque()
    {
        printUnsupportedFeature("getTorque");
        return {};
    }
    // 获取电机温度 L20暂不支持
    virtual std::vector<uint8_t> getTemperature()
    {
        printUnsupportedFeature("getTemperature");
        return {};
    }
    // 获取版本号   目前仅支持L10
    virtual std::string getVersion()
    {
        printUnsupportedFeature("getVersion");
        return "";
    }
    // 获取设备ID L20协议
    virtual std::vector<uint8_t> getUID()
    {
        printUnsupportedFeature("getUID");
        return {};
    }
    // 手指堵转或过流判断计数阀值 L20协议
    virtual std::vector<uint8_t> getRotorLockCount()
    {
        printUnsupportedFeature("getRotorLockCount");
        return {};
    }
    // 清除电机故障码 目前仅支持L20
	virtual void clearFaultCode(const std::vector<uint8_t> &code)
    {
        (void)code;
        printUnsupportedFeature("clearFaultCode");
    }
    // 设置电流 目前仅支持L20 
	virtual void setCurrent(const std::vector<uint8_t> &current)
    {
        (void)current;
        printUnsupportedFeature("setCurrent");
    }
	// 设置电机使能 目前仅支持L25
	virtual void setMotorEnable(const std::vector<uint8_t> &enable)
    {
        (void)enable;
        printUnsupportedFeature("setMotorEnable");
    }
	// 设置电机使能 目前仅支持L25
	virtual void setMotorDisable(const std::vector<uint8_t> &disable)
    {
        (void)disable;
        printUnsupportedFeature("setMotorDisable");
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
protected:
    void printUnsupportedFeature(const std::string &featureName) const
    {
        static std::set<std::string> printedFeatures;
        if (printedFeatures.find(featureName) == printedFeatures.end())
        {
            std::cout << featureName << " Not currently supported!" << std::endl;
            printedFeatures.insert(featureName);
        }
    }
};
#endif // I_HAND_H
