#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <atomic>
#include "LinkerHandApi.h"

#include <iostream>

// 定义ANSI转义序列常量
const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

const std::string BG_BLACK = "\033[40m";
const std::string BG_RED = "\033[41m";
const std::string BG_GREEN = "\033[42m";
const std::string BG_YELLOW = "\033[43m";
const std::string BG_BLUE = "\033[44m";
const std::string BG_MAGENTA = "\033[45m";
const std::string BG_CYAN = "\033[46m";
const std::string BG_WHITE = "\033[47m";

const std::string BOLD = "\033[1m";
const std::string UNDERLINE = "\033[4m";
const std::string REVERSE = "\033[7m";

// 用于标记程序是否应该退出
std::atomic<bool> running(true);

void exit()
{
    std::cout << "Exit the program ..." << std::endl;
    running = false;
    system("sudo /usr/sbin/ip link set can0 down");
    exit(0);
}

// 辅助函数：将字节向量转换为十六进制字符串
std::string bytesToHex(const std::vector<uint8_t> &bytes)
{
    std::string hexStr;
    for (uint8_t byte : bytes)
    {
        hexStr += std::to_string(byte) + " ";
    }
    return hexStr;
}

// 辅助函数：将二维字节向量转换为十六进制字符串
std::string bytesToHex(const std::vector<std::vector<uint8_t>> &bytes)
{
    std::string hexStr;
    for (const auto &vec : bytes)
    {
        for (uint8_t byte : vec)
        {
            hexStr += std::to_string(byte) + " ";
        }
        hexStr += "\n";
    }
    return hexStr;
}

// 全局变量
int show_count = 0;
int show_count_obj = 0;
int show_step = 0;
std::map<std::string, int> hand = {
    {"joint1", 250}, {"joint2", 250}, {"joint3", 250}, {"joint4", 250}, {"joint5", 250},
    {"joint6", 250}, {"joint7", 250}, {"joint8", 250}, {"joint9", 250}, {"joint10", 250},
    {"joint11", 250}, {"joint12", 0}, {"joint13", 0}, {"joint14", 0}, {"joint15", 0},
    {"joint16", 250}, {"joint17", 250}, {"joint18", 250}, {"joint19", 250}, {"joint20", 250},
    {"joint21", 250}, {"joint22", 250}, {"joint23", 250}, {"joint24", 250}, {"joint25", 250}
};

// 更新关节状态
std::vector<uint8_t> show_left() {

    std::cout << "show_step: " << show_step << std::endl;
    std::cout << "show_count: " << show_count << std::endl;
    std::cout << "show_count_obj: " << show_count_obj << std::endl;

    show_count++;
    if (show_count >= show_count_obj) {
        show_count = 0;
        switch (show_step) {
            case 0: // 张开手掌
                show_step++;
                show_count_obj = 50;
                hand["joint1"] = 75;
                hand["joint2"] = 255;
                hand["joint3"] = 255;
                hand["joint4"] = 255;
                hand["joint5"] = 255;
                hand["joint6"] = 176;
                hand["joint7"] = 51;
                hand["joint8"] = 51;
                hand["joint9"] = 125;
                hand["joint10"] = 202;
                hand["joint11"] = 202;
                hand["joint12"] = 255;
                hand["joint13"] = 255;
                hand["joint14"] = 255;
                hand["joint15"] = 255;
                hand["joint16"] = 255;
                hand["joint17"] = 255;
                hand["joint18"] = 255;
                hand["joint19"] = 255;
                hand["joint20"] = 255;
                hand["joint21"] = 255;
                hand["joint22"] = 255;
                hand["joint23"] = 255;
                hand["joint24"] = 255;
                hand["joint25"] = 255;
                break;
            case 1: // 收小指与无名指
                show_step++;
                show_count_obj = 10;
                hand["joint4"] = 0;
                hand["joint5"] = 0;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                hand["joint9"] = 128;
                hand["joint10"] = 128;
                hand["joint11"] = 250;
                hand["joint16"] = 250;
                hand["joint17"] = 250;
                hand["joint18"] = 250;
                hand["joint19"] = 0;
                hand["joint20"] = 0;
                hand["joint21"] = 250;
                hand["joint22"] = 250;
                hand["joint23"] = 250;
                hand["joint24"] = 0;
                hand["joint25"] = 0;
                break;
            case 2: // 将拇指搭到小指与无名指上面
                show_step++;
                show_count_obj = 30;
                hand["joint1"] = 100;
                hand["joint6"] = 180;
                hand["joint16"] = 0;
                hand["joint21"] = 0;
                break;
            case 3: // 食指和中指向一侧倾斜
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 200;
                hand["joint8"] = 200;
                hand["joint11"] = 200;
                break;
            case 4: // 另一侧
                show_step++;
                show_count_obj = 13;
                hand["joint7"] = 50;
                hand["joint8"] = 50;
                break;
            case 5: // 两支回中
                show_step++;
                show_count_obj = 13;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                break;
            case 6: // 食指和中指做 Y
                show_step++;
                show_count_obj = 2;
                hand["joint7"] = 50;
                hand["joint8"] = 200;
                break;
            case 7: // 收 Y
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                break;
            case 8: // 食指和中指做 Y
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 50;
                hand["joint8"] = 200;
                break;
            case 9: // 收 Y
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                break;
            case 10: // 中指和食指弯曲伸直交替两遍
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 100;
                hand["joint3"] = 100;
                hand["joint17"] = 100;
                hand["joint18"] = 100;
                hand["joint22"] = 100;
                hand["joint23"] = 100;
                break;
            case 11: // 中指和食指弯曲伸直交替两遍
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 250;
                hand["joint3"] = 250;
                hand["joint17"] = 250;
                hand["joint18"] = 250;
                hand["joint22"] = 250;
                hand["joint23"] = 250;
                break;
            case 12: // 中指和食指弯曲伸直交替两遍
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 100;
                hand["joint3"] = 100;
                hand["joint17"] = 100;
                hand["joint18"] = 100;
                hand["joint22"] = 100;
                hand["joint23"] = 100;
                break;
            case 13: // 中指和食指弯曲伸直交替两遍
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 250;
                hand["joint3"] = 250;
                hand["joint17"] = 250;
                hand["joint18"] = 250;
                hand["joint22"] = 250;
                hand["joint23"] = 250;
                break;
            case 14: // 蜷曲拇指
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 250;
                hand["joint6"] = 150;
                hand["joint11"] = 250;
                hand["joint23"] = 250;
                break;
            case 15: // 拇指收于掌内
                show_step++;
                show_count_obj = 10;
                hand["joint6"] = 5;
                break;
            case 16: // 收4指
                show_step++;
                show_count_obj = 30;
                hand["joint2"] = 100;
                hand["joint3"] = 100;
                hand["joint4"] = 100;
                hand["joint5"] = 100;
                hand["joint17"] = 100;
                hand["joint18"] = 100;
                hand["joint19"] = 100;
                hand["joint20"] = 100;
                hand["joint22"] = 100;
                hand["joint23"] = 100;
                hand["joint24"] = 100;
                hand["joint25"] = 100;
                break;
            case 17: // 依次放开4指和拇指
                show_step++;
                show_count_obj = 15;
                hand["joint5"] = 250;
                hand["joint20"] = 250;
                hand["joint25"] = 250;
                break;
            case 18: // 1
                show_step++;
                show_count_obj = 15;
                hand["joint4"] = 250;
                hand["joint19"] = 250;
                hand["joint24"] = 250;
                break;
            case 19: // 2
                show_step++;
                show_count_obj = 15;
                hand["joint3"] = 250;
                hand["joint18"] = 250;
                hand["joint23"] = 250;
                break;
            case 20: // 3
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 250;
                hand["joint17"] = 250;
                hand["joint22"] = 250;
                break;
            case 21: // 40
                show_step++;
                show_count_obj = 10;
                hand["joint6"] = 250;
                hand["joint16"] = 250;
                hand["joint21"] = 250;
                break;
            case 22: // 并拢拇指
                show_step++;
                show_count_obj = 20;
                hand["joint11"] = 10;
                break;
            case 23: // 反转拇指指掌心
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 0;
                break;
            case 24: // 分两步回到初始位置
                show_step++;
                show_count_obj = 30;
                hand["joint11"] = 250;
                break;
            case 25: // 1
                show_step++;
                show_count_obj = 50;
                hand["joint11"] = 250;
                break;
            case 26: // 2
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 200;
                hand["joint8"] = 200;
                hand["joint9"] = 200;
                hand["joint10"] = 200;
                break;
            case 27: // 3
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 80;
                hand["joint8"] = 80;
                hand["joint9"] = 80;
                hand["joint10"] = 80;
                break;
            case 28: // 4
                show_step++;
                show_count_obj = 20;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                hand["joint9"] = 128;
                hand["joint10"] = 128;
                break;
            case 29: // 依次蜷曲4小指
                show_step++;
                show_count_obj = 15;
                hand["joint17"] = 0;
                hand["joint22"] = 0;
                break;
            case 30: // 蜷曲4指
                show_step++;
                show_count_obj = 15;
                hand["joint18"] = 0;
                hand["joint23"] = 0;
                break;
            case 31: // 4
                show_step++;
                show_count_obj = 15;
                hand["joint19"] = 0;
                hand["joint24"] = 0;
                break;
            case 32: // 4
                show_step++;
                show_count_obj = 15;
                hand["joint20"] = 0;
                hand["joint25"] = 0;
                break;
            case 33: // 依次蜷曲4小指
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 0;
                break;
            case 34: // 依次蜷曲4小指
                show_step++;
                show_count_obj = 15;
                hand["joint3"] = 0;
                break;
            case 35: // 依次蜷曲4小指
                show_step++;
                show_count_obj = 15;
                hand["joint4"] = 0;
                break;
            case 36: // 依次蜷曲4小指
                show_step++;
                show_count_obj = 15;
                hand["joint5"] = 0;
                break;
            case 37: // 蜷曲拇指
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 0;
                hand["joint16"] = 200;
                break;
            case 38: // 打开食指和小指
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 250;
                hand["joint16"] = 250;
                break;
            case 39: // 打开食指和小指
                show_step++;
                show_count_obj = 30;
                hand["joint2"] = 250;
                hand["joint5"] = 250;
                hand["joint17"] = 250;
                hand["joint20"] = 250;
                hand["joint22"] = 250;
                hand["joint25"] = 250;
                break;
            case 40: // 将拇指搭上666
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 100;
                hand["joint6"] = 200;
                hand["joint11"] = 100;
                hand["joint16"] = 100;
                break;
            case 41: // 左右动手指
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 80;
                hand["joint10"] = 200;
                break;
            case 42: // 左右动手指
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 200;
                hand["joint10"] = 80;
                break;
            case 43: // 左右动手指
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 80;
                hand["joint10"] = 200;
                break;
            case 44: // 左右动手指
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 200;
                hand["joint10"] = 80;
                break;
            case 45: // 左右动手指
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 128;
                hand["joint10"] = 128;
                break;
            case 46: // 展开
                show_step++;
                show_count_obj = 50;
                hand["joint1"] = 250;
                hand["joint3"] = 250;
                hand["joint4"] = 250;
                hand["joint6"] = 250;
                hand["joint11"] = 250;
                hand["joint16"] = 250;
                hand["joint18"] = 250;
                hand["joint19"] = 250;
                hand["joint21"] = 250;
                hand["joint23"] = 250;
                hand["joint24"] = 250;
                break;
            case 47: // 拇指和食指捏
                show_step++;
                show_count_obj = 50;
                hand["joint1"] = 40;
                hand["joint2"] = 0;
                hand["joint6"] = 100;
                hand["joint11"] = 70;
                hand["joint17"] = 240;
                hand["joint22"] = 240;
                break;
            case 48: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint2"] = 250;
                hand["joint6"] = 220;
                hand["joint11"] = 100;
                hand["joint17"] = 250;
                hand["joint22"] = 250;
                break;
            case 49: // 拇指和中指捏
                show_step++;
                show_count_obj = 35;
                hand["joint3"] = 0;
                hand["joint6"] = 70;
                hand["joint11"] = 60;
                hand["joint18"] = 220;
                hand["joint23"] = 220;
                break;
            case 50: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint3"] = 250;
                hand["joint6"] = 100;
                hand["joint11"] = 100;
                hand["joint18"] = 250;
                hand["joint23"] = 250;
                break;
            case 51: // 拇指和无名指捏
                show_step++;
                show_count_obj = 35;
                hand["joint4"] = 0;
                hand["joint6"] = 30;
                hand["joint11"] = 50;
                hand["joint19"] = 220;
                hand["joint24"] = 220;
                break;
            case 52: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint4"] = 250;
                hand["joint6"] = 100;
                hand["joint11"] = 100;
                hand["joint19"] = 250;
                hand["joint24"] = 250;
                break;
            case 53: // 拇指和小指捏
                show_step++;
                show_count_obj = 40;
                hand["joint5"] = 0;
                hand["joint6"] = 0;
                hand["joint11"] = 40;
                hand["joint20"] = 230;
                hand["joint25"] = 230;
                break;
            case 54: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint5"] = 20;
                hand["joint6"] = 0;
                hand["joint11"] = 100;
                hand["joint20"] = 250;
                hand["joint25"] = 250;
                break;
            case 55: // 拇指和小指掐
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 175;
                hand["joint5"] = 175;
                hand["joint6"] = 0;
                hand["joint11"] = 0;
                hand["joint16"] = 130;
                hand["joint20"] = 100;
                hand["joint21"] = 130;
                hand["joint25"] = 80;
                break;
            case 56: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint1"] = 250;
                hand["joint5"] = 250;
                hand["joint6"] = 0;
                hand["joint11"] = 0;
                hand["joint16"] = 250;
                hand["joint20"] = 250;
                hand["joint21"] = 250;
                hand["joint25"] = 250;
                break;
            case 57: // 拇指和无名指掐
                show_step++;
                show_count_obj = 35;
                hand["joint1"] = 170;
                hand["joint4"] = 170;
                hand["joint6"] = 30;
                hand["joint11"] = 50;
                hand["joint16"] = 130;
                hand["joint19"] = 80;
                hand["joint21"] = 130;
                hand["joint24"] = 80;
                break;
            case 58: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint1"] = 250;
                hand["joint4"] = 250;
                hand["joint6"] = 30;
                hand["joint11"] = 50;
                hand["joint16"] = 250;
                hand["joint19"] = 250;
                hand["joint21"] = 250;
                hand["joint24"] = 250;
                break;
            case 59: // 拇指和中指掐
                show_step++;
                show_count_obj = 35;
                hand["joint1"] = 155;
                hand["joint3"] = 155;
                hand["joint6"] = 70;
                hand["joint11"] = 60;
                hand["joint16"] = 130;
                hand["joint19"] = 90;
                hand["joint21"] = 130;
                hand["joint24"] = 80;
                break;
            case 60: // 1
                show_step++;
                show_count_obj = 20;
                hand["joint1"] = 250;
                hand["joint3"] = 250;
                hand["joint6"] = 100;
                hand["joint11"] = 100;
                hand["joint16"] = 250;
                hand["joint19"] = 250;
                hand["joint21"] = 250;
                hand["joint24"] = 250;
                break;
            case 61: // 拇指和食指掐
                show_step++;
                show_count_obj = 35;
                hand["joint1"] = 165;
                hand["joint2"] = 165;
                hand["joint6"] = 100;
                hand["joint11"] = 70;
                hand["joint16"] = 130;
                hand["joint19"] = 80;
                hand["joint21"] = 130;
                hand["joint24"] = 80;
                break;
            default:
                std::cout << "Error: Unknown command" << std::endl;
                // show_step = 0;
        }
    }

    std::vector<uint8_t> positions;
    for (const auto& kv : hand) {
        positions.push_back(static_cast<double>(kv.second));
    }
    return positions;
}

// L7 - 执行动作 - 手掌握拳
std::vector<std::vector<uint8_t>> L7_POSE_1 = {
    {255, 255, 255, 255, 255, 255, 255},
    {255, 255, 0, 255, 255, 255, 255},
    {255, 255, 0, 0, 255, 255, 255},
    {255, 255, 0, 0, 0, 255, 255},
    {255, 255, 0, 0, 0, 0, 255},
    {72, 90, 0, 0, 0, 0, 55}};
// L7 - 执行动作 - 张开
std::vector<uint8_t> L7_POSE_OPEN = {255, 255, 255, 255, 255, 255, 255};


// L10 - 执行动作 - 手指循环弯曲
std::vector<std::vector<uint8_t>> L10_POSE_1 = {
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

// L10 - 执行动作 - 拇指与其他手指循环对指
std::vector<std::vector<uint8_t>> L10_POSE_2 = {
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

// L10 - 执行动作 - 手指侧摆
std::vector<std::vector<uint8_t>> L10_POSE_3 = {
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

// L10 - 执行动作 - 张开
std::vector<uint8_t> L10_POSE_OPEN = {255, 104, 255, 255, 255, 255, 255, 255, 255, 71};
// L10 - 执行动作 - 握拳
std::vector<uint8_t> L10_POSE_CLOSE = {101, 60, 0, 0, 0, 0, 255, 255, 255, 51};

void interactiveMode(LinkerHandApi &hand)
{
    int choice;
    while (running)
    {
        // std::cout << YELLOW << "——————————————————————————————————————————————\n" << RESET;
        std::cout << GREEN << "Run Choose Task:\n" << RESET;
        std::cout << YELLOW << "——————————————————————————————————————————————\n" << RESET;
        std::cout << BLUE << "1. Obtain version information\n" << RESET;
        std::cout << BLUE << "2. Obtain temperature\n" << RESET;
        std::cout << BLUE << "3. Obtain fault codes\n" << RESET;
        std::cout << BLUE << "4. Obtain current\n" << RESET;
        std::cout << BLUE << "5. Obtain pressure sensing information 1\n" << RESET;
        std::cout << BLUE << "6. Obtain pressure sensing information 2\n" << RESET;
        std::cout << BLUE << "7. Clear fault codes\n" << RESET;
        std::cout << BLUE << "8. Loop to obtain the current finger joint status 10 times\n" << RESET;
        std::cout << BLUE << "9. Execute preset actions\n" << RESET;
        std::cout << RED << "0. Exit\n" << RESET;
        std::cout << YELLOW << "——————————————————————————————————————————————\n" << RESET;
        std::cout << GREEN << "Please enter options: " << RESET;
        std::cin >> choice;

        if (choice != 1)
            std::cout << "——————————————————————————————————————————————" << std::endl;

        switch (choice)
        {
        case 1:
            std::cout << hand.getVersion() << std::endl;
            break;
        case 2:
            std::cout << "Obtain temperature: " << bytesToHex(hand.getTemperature()) << std::endl;
            break;
        case 3:
            std::cout << "Obtain fault codes: " << bytesToHex(hand.getFaultCode()) << std::endl;
            break;
        case 4:
            std::cout << "Obtain current: " << bytesToHex(hand.getCurrent()) << std::endl;
            break;
        case 5:
            std::cout << "Obtain pressure sensing information 1:\n"
                      << bytesToHex(hand.getForce(1)) << std::endl;
            break;
        case 6:
            std::cout << "Obtain pressure sensing information 2:\n"
                      << bytesToHex(hand.getForce()) << std::endl;
            break;
        case 7:
            std::cout << "Clear fault codes\n" << std::endl;
            hand.clearFaultCode();
            break;
        case 8:
            for (size_t i = 0; i < 10; i++) 
            {
                std::cout << bytesToHex(hand.getState()) << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            break;
        case 9:
            if (hand.handJoint_ == LINKER_HAND::L7)
            {
                // 设置速度
                hand.setSpeed({200,200,200,200,200,200,200}); // L7协议速度需要7个
                //---------------------------------------------------------
                std::cout << "L7 - 执行动作 - 手掌握拳" << std::endl;
                for (const auto &pose : L7_POSE_1)
                {
                    hand.fingerMove(pose);
                    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待1秒
                }
                //---------------------------------------------------------
                std::cout << "L7 - 执行动作 - 张开" << std::endl;
                hand.fingerMove(L7_POSE_OPEN);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                //---------------------------------------------------------
            }
            else if (hand.handJoint_ == LINKER_HAND::L10)
            {
                // 设置速度
                hand.setSpeed({200,200,200,200,200}); // L10协议速度需要5个
                //---------------------------------------------------------
                std::cout << "L10 - 执行动作 - 手指循环弯曲" << std::endl;
                for (const auto &pose : L10_POSE_1)
                {
                    hand.fingerMove(pose);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                //---------------------------------------------------------
                std::cout << "L10 - 执行动作 - 拇指与其他手指循环对指" << std::endl;
                for (const auto &pose : L10_POSE_2)
                {
                    hand.fingerMove(pose);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                //---------------------------------------------------------
                std::cout << "L10 - 执行动作 - 手指侧摆" << std::endl;
                for (const auto &pose : L10_POSE_3)
                {
                    hand.fingerMove(pose);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                //---------------------------------------------------------
                std::cout << "L10 - 执行动作 - 握拳" << std::endl;
                hand.fingerMove(L10_POSE_CLOSE);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                //---------------------------------------------------------
                std::cout << "L10 - 执行动作 - 张开" << std::endl;
                hand.fingerMove(L10_POSE_OPEN);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                //---------------------------------------------------------
            }
            else if (hand.handJoint_ == LINKER_HAND::L20)
            {
                //---------------------------------------------------------
                std::cout << "L20 - 执行动作 - 握拳" << std::endl;
                std::vector<uint8_t> L20_POSE_CLOSE = {69, 0, 0, 0, 0, 32, 10, 100, 180, 240, 145, 255, 255, 255, 255, 0, 0, 0, 0, 0};
                hand.fingerMove(L20_POSE_CLOSE);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                //---------------------------------------------------------
                std::cout << "L20 - 执行动作 - 张开" << std::endl;
                std::vector<uint8_t> L20_POSE_OPEN = {255, 255, 230, 255, 255, 255, 10, 100, 180, 240, 245, 255, 255, 255, 255, 255, 255, 255, 255, 255};
                hand.fingerMove(L20_POSE_OPEN);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            else if (hand.handJoint_ == LINKER_HAND::L25)
            {
                std::cout << "LINKER_HAND::L25 ... " << std::endl;

                while (show_step <= 61)
                {
                    std::cout << bytesToHex(show_left()) << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                show_step = 0;
            }
            break;
        case 0:
            exit();
            break;
        default:
            std::cout << "Invalid option, please re-enter!\n";
        }
        if (choice != 1)
            std::cout << "——————————————————————————————————————————————" << std::endl;
    }
}

int main()
{
    std::cout << YELLOW << "=========================\n" << RESET;
    std::cout << YELLOW << "        Example\n" << RESET;
    std::cout << YELLOW << "=========================\n" << RESET;

    int choice;
    LINKER_HAND linkerhand;
    while (true)
    {
        std::cout << GREEN << "\nRun Choose LinkerHand:\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << BLUE << "[1]: L7\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << BLUE << "[2]: L10\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << BLUE << "[3]: L20\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << BLUE << "[4]: L25\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << RED << "[0]: Exit\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << GREEN << "Please enter options: " << RESET;
        std::cin >> choice;
        
        switch (choice)
        {
        case 1:
            linkerhand = LINKER_HAND::L7;
            break;
        case 2:
            linkerhand = LINKER_HAND::L10;
            break;
        case 3:
            linkerhand = LINKER_HAND::L20;
            break;
        case 4:
            linkerhand = LINKER_HAND::L25;
            break;
        case 0:
            exit();
            break;
        default:
            std::cout << "Invalid option, please re-enter!\n";
            continue;
        }
        break;
    }

    HAND_TYPE handType;
    while (true)
    {
        std::cout << YELLOW << "——————————————————————————————————————————————\n" << RESET;
        std::cout << GREEN << "Run Choose Hand Direction:\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << BLUE << "[1]: Left Hand\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << BLUE << "[2]: Right Hand\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << RED << "[0]: Exit\n" << RESET;
        std::cout << YELLOW << "—————————————————————————\n" << RESET;
        std::cout << GREEN << "Please enter options: " << RESET;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            handType = HAND_TYPE::LEFT;
            break;
        case 2:
            handType = HAND_TYPE::RIGHT;
            break;
        case 0:
            exit();
            break;
        default:
            std::cout << "Invalid option, please re-enter!\n";
            continue;
        }
        break;
    }

    // 调用API接口
    LinkerHandApi hand(linkerhand, handType);
    // LinkerHandApi hand(LINKER_HAND::L10, HAND_TYPE::RIGHT);

    std::cout << "——————————————————————————————————————————————" << std::endl;
    // 启动交互模式
    interactiveMode(hand);

    return 0;
}