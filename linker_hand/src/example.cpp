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

std::string bytesToHex(const std::vector<double> &data)
{
    std::stringstream ss;
    for (const auto &val : data)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)val << " ";
    }
    return ss.str();
}

std::string bytesToHex(const std::vector<std::vector<double>> &data)
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
    auto version = hand.getVersion();
    std::cout << "版本号：" << bytesToHex(version) << std::endl;

#if 1

    // 获取法向压力、切向压力、切向方向、接近感应
    std::vector<std::vector<double>> force_result = hand.getForce();
    std::cout << "\n法向压力|切向压力|切向方向|接近感应：\n" << bytesToHex(force_result) << std::endl;

    std::vector<std::vector<double>> pressure_result = hand.getPressure();
    std::cout << "所有指头的压力数据：\n" << bytesToHex(pressure_result) << std::endl;



    // 执行动作 - 手指快速移动
    std::vector<double> pose = {};
    
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



    std::vector<std::vector<double>> poses = {};

    // 执行动作 - 手指循环弯曲
    poses = {
        {35.0,140.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0,30.0},    // 拇指弯曲
        {255.0,70.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0},   // 手掌张开
        {255.0,70.0,0.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0},     // 食指弯曲
        {255.0,70.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0},   // 手掌张开
        {255.0,70.0,255.0,0.0,255.0,255.0,255.0,255.0,255.0,255.0},     // 中指弯曲
        {255.0,70.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0},   // 手掌张开
        {255.0,70.0,255.0,255.0,0.0,255.0,255.0,255.0,255.0,255.0},     // 无名指弯曲
        {255.0,70.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0},   // 手掌张开
        {255.0,70.0,255.0,255.0,255.0,0.0,255.0,255.0,255.0,255.0},     // 小拇指弯曲
        {255.0,70.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0,255.0}    // 手掌张开
    };
    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }


    // 执行动作 - 拇指与其他手指循环对指
    poses = {
        {255.0, 70.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0},  // 手掌张开
        {145.0, 128.0, 146.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 90.0},  // 拇指对食指
        {255.0, 70.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0},  // 手掌张开
        {145.0, 88.0, 255.0, 148.0, 255.0, 255.0, 255.0, 255.0, 255.0, 75.0},   // 拇指对中指
        {255.0, 70.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0},  // 手掌张开
        {145.0, 63.0, 255.0, 255.0, 150.0, 255.0, 255.0, 255.0, 255.0, 60.0},   // 拇指对无名指
        {255.0, 70.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0},  // 手掌张开
        {147.0, 70.0, 255.0, 255.0, 255.0, 131.0, 255.0, 255.0, 120.0, 27.0},   // 拇指对小拇指
        {255.0, 70.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0}   // 手掌张开
    };
    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }

    // 执行动作 - 手指侧摆
    poses = {
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 63.0},     // 手掌张开
        {255.0, 0.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 63.0},       // 拇指侧摆
        {255.0, 70.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 0.0},       // 手掌张开
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 255.0},    // 拇指旋转
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 63.0},     // 手掌张开
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 88.0, 80.0, 63.0},    // 食指侧摆
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 63.0},     // 手掌张开
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 255.0, 80.0, 63.0},    // 无名指侧摆
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 63.0},     // 手掌张开
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 255.0, 63.0},    // 小拇指侧摆
        {255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 40.0, 88.0, 80.0, 63.0}      // 手掌张开
    };
    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }


    // 张开
    std::vector<double> open_pose = {255, 104, 255, 255, 255, 255, 255, 255, 255, 71};
    // 握拳
    std::vector<double> close_pose = {101.0, 60.0, 0.0, 0.0, 0.0, 0.0, 255.0, 255.0, 255.0, 51.0};

    // 执行动作 - 握拳
    hand.fingerMove(close_pose);
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
