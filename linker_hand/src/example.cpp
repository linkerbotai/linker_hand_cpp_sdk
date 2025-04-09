#include <iostream>
#include <signal.h>
#include <atomic>
#include <iomanip>

#include "LinkerHandApi.h"

// 用于标记程序是否应该退出
std::atomic<bool> running(true);

// 信号处理函数，捕获 Ctrl+C 信号
void handleSignal(int sig)
{
    if (sig == SIGINT)
    {
        running = false;
    }
}

std::string bytesToHex(const std::vector<uint8_t> &data)
{
    std::stringstream ss;
    for (const auto &val : data)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)val << " ";
    }
    return ss.str();
}

std::string bytesToHex(const std::vector<std::vector<uint8_t>> &data)
{
    std::stringstream ss;
    for (const auto &subVector : data)
    {
        ss << bytesToHex(subVector) << std::endl; // 调用一维向量版本的 bytesToHex
    }
    return ss.str();
}

int main()
{
    // 初始化API
    LinkerHandApi hand("right", "L10");

    // 获取版本号
    std::cout << hand.getVersion() << std::endl;
    
    // 获取法向压力、切向压力、切向方向、接近感应
    std::vector<std::vector<uint8_t>> force_result = hand.getForce();
    std::cout << "\n法向压力|切向压力|切向方向|接近感应：\n" << bytesToHex(force_result) << std::endl;

    // 获取所有指头的压力数据
    std::vector<std::vector<uint8_t>> pressure_result = hand.getPressure();
    std::cout << "所有指头的压力数据：\n" << bytesToHex(pressure_result) << std::endl;


#if 0
    // 执行动作 - 手指快速移动
    std::vector<uint8_t> pose = {};
    
    for (size_t i = 0; i < 10; i++)
    {
        if (i % 2 == 0) {
            pose = {255, 128, 255, 255, 255, 255, 128, 128, 128, 128};
        } else {
            pose = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // hand.fingerMove(pose);
    }


    std::vector<std::vector<uint8_t>> poses = {};

    // 执行动作 - 手指循环弯曲
    poses = {
        {35,140,255,255,255,255,255,255,255,30},    // 拇指弯曲
        {255,70,255,255,255,255,255,255,255,255},   // 手掌张开
        {255,70,0,255,255,255,255,255,255,255},     // 食指弯曲
        {255,70,255,255,255,255,255,255,255,255},   // 手掌张开
        {255,70,255,0,255,255,255,255,255,255},     // 中指弯曲
        {255,70,255,255,255,255,255,255,255,255},   // 手掌张开
        {255,70,255,255,0,255,255,255,255,255},     // 无名指弯曲
        {255,70,255,255,255,255,255,255,255,255},   // 手掌张开
        {255,70,255,255,255,0,255,255,255,255},     // 小拇指弯曲
        {255,70,255,255,255,255,255,255,255,255}    // 手掌张开
    };
    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }

    auto speed = hand.getSpeed();
    std::cout << "速度：" << bytesToHex(speed) << std::endl;

    auto state = hand.getState();
    std::cout << "状态：" << bytesToHex(state) << std::endl;

    // 执行动作 - 拇指与其他手指循环对指
    poses = {
        {255, 70, 255, 255, 255, 255, 255, 255, 255, 255},  // 手掌张开
        {145, 128, 146, 255, 255, 255, 255, 255, 255, 90},  // 拇指对食指
        {255, 70, 255, 255, 255, 255, 255, 255, 255, 255},  // 手掌张开
        {145, 88, 255, 148, 255, 255, 255, 255, 255, 75},   // 拇指对中指
        {255, 70, 255, 255, 255, 255, 255, 255, 255, 255},  // 手掌张开
        {145, 63, 255, 255, 150, 255, 255, 255, 255, 60},   // 拇指对无名指
        {255, 70, 255, 255, 255, 255, 255, 255, 255, 255},  // 手掌张开
        {147, 70, 255, 255, 255, 131, 255, 255, 120, 27},   // 拇指对小拇指
        {255, 70, 255, 255, 255, 255, 255, 255, 255, 255}   // 手掌张开
    };
    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }

    // 执行动作 - 手指侧摆
    poses = {
        {255, 255, 255, 255, 255, 255, 40, 88, 80, 63},     // 手掌张开
        {255, 0, 255, 255, 255, 255, 40, 88, 80, 63},       // 拇指侧摆
        {255, 70, 255, 255, 255, 255, 40, 88, 80, 0},       // 手掌张开
        {255, 255, 255, 255, 255, 255, 40, 88, 80, 255},    // 拇指旋转
        {255, 255, 255, 255, 255, 255, 40, 88, 80, 63},     // 手掌张开
        {255, 255, 255, 255, 255, 255, 255, 88, 80, 63},    // 食指侧摆
        {255, 255, 255, 255, 255, 255, 40, 88, 80, 63},     // 手掌张开
        {255, 255, 255, 255, 255, 255, 40, 255, 80, 63},    // 无名指侧摆
        {255, 255, 255, 255, 255, 255, 40, 88, 80, 63},     // 手掌张开
        {255, 255, 255, 255, 255, 255, 40, 88, 255, 63},    // 小拇指侧摆
        {255, 255, 255, 255, 255, 255, 40, 88, 80, 63}      // 手掌张开
    };
    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }


    // 张开
    std::vector<uint8_t> open_pose = {255, 104, 255, 255, 255, 255, 255, 255, 255, 71};
    // 握拳
    std::vector<uint8_t> fist_pose = {101, 60, 0, 0, 0, 0, 255, 255, 255, 51};

    // 执行动作 - 握拳
    hand.fingerMove(fist_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 执行动作 - 张开
    hand.fingerMove(open_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));

#endif

    while (running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}