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

// 定义一个函数，用于在单独的线程中不断调用 getSpeed
void thread_function (LinkerHandApi& hand) {
    while (running) {
        std::cout << "获取状态数据：" << bytesToHex(hand.getState()) << std::endl;

        // 暂停一段时间，避免过快输出
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    // 调用API接口
    LinkerHandApi hand(LINKER_HAND::L7, HAND_TYPE::LEFT);

    // 线程
    // std::thread speedThread(thread_function, std::ref(hand));





    // 获取版本号
    std::cout << hand.getVersion() << std::endl;

    std::cout << "获取电机温度：" << bytesToHex(hand.getMotorTemperature()) << std::endl;
    
    std::cout << "获取故障码：" << bytesToHex(hand.getMotorFaultCode()) << std::endl;

    std::cout << "获取五指的法向压力、切向压力、切向方向、接近感应数据：\n" << bytesToHex(hand.getForce(1)) << std::endl;

    std::cout << "获取每个手指的压感数据：\n" << bytesToHex(hand.getForce()) << std::endl;

    std::cout << "获取速度数据：" << bytesToHex(hand.getSpeed()) << std::endl;


    // L7 --------------------------------------------------------
    // 设置速度
    hand.setSpeed({200,200,200,200,200,200,200});

    // 手掌握拳
    std::vector<std::vector<uint8_t>> poses = {
        {255, 255, 255, 255, 255, 255, 255},
        {255, 255, 0, 255, 255, 255, 255},
        {255, 255, 0, 0, 255, 255, 255},
        {255, 255, 0, 0, 0, 255, 255},
        {255, 255, 0, 0, 0, 0, 255},
        {72, 90, 0, 0, 0, 0, 55}
    };

    for (const auto& pose : poses) {
        hand.fingerMove(pose);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
    }
    
    std::vector<uint8_t> open = {255, 255, 255, 255, 255, 255, 255};

    std::cout << "L7-执行动作 - 张开" << std::endl;
    hand.fingerMove(open);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // --------------------------------------------------------

    #if 0
    
    std::cout << "获取电机电流：" << bytesToHex(hand.getMotorCurrent()) << std::endl;
    // 握拳
    std::vector<uint8_t> fist_pose = {101, 60, 0, 0, 0, 0, 255, 255, 255, 51, 101, 60, 0, 0, 0, 0, 255, 255, 255, 51};

    std::cout << "执行动作 - 握拳" << std::endl;
    hand.fingerMove(fist_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));


    
    // std::cout << "执行动作 - 手指快速移动" << std::endl;
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

    std::cout << "执行动作 - 手指循环弯曲" << std::endl;
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

    std::cout << "获取速度数据：" << bytesToHex(hand.getSpeed()) << std::endl;

    std::cout << "获取状态数据：" << bytesToHex(hand.getState()) << std::endl;

    std::cout << "执行动作 - 拇指与其他手指循环对指" << std::endl;
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

    std::cout << "执行动作 - 手指侧摆" << std::endl;
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

    std::cout << "执行动作 - 握拳" << std::endl;
    hand.fingerMove(fist_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "执行动作 - 张开" << std::endl;
    hand.fingerMove(open_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));

#endif

    while (running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // speedThread.join();

    return 0;
}