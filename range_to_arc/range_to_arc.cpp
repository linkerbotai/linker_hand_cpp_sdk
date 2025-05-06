#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>


//------------------------------------------------------------------------------------------
// URDF顺序
//    1       2       3       4        5       6       7        8        9      10
// 拇指旋转，拇指侧摆，拇指根部，食指侧摆，食指根部，中指根部，无名指侧摆，无名指根部，小指侧摆，小指根部
//---------------------------------------------------------------------------------------------------
//					   1     2      3      4    5     6     7      8    9    10
/*
const std::vector<double> l10_r_min = {-0.52, -1.43,    0, -0.26,    0,    0,    0,    0,    0,    0};
const std::vector<double> l10_r_max = { 1.01,     0, 0.75,     0, 1.62, 1.62, 0.13, 1.62, 0.26, 1.62};
const std::vector<int> l10_r_derict = {   -1,     0,   -1,    -1,   -1,   -1,    0,   -1,    0,   -1};

const std::vector<double> l10_l_min = {-1.01,    0, -1.45,    0,    0,    0, -0.26,    0, -0.26,    0};
const std::vector<double> l10_l_max = { 0.52, 1.43,     0, 0.26, 1.62, 1.62,     0, 1.62,     0, 1.62};
const std::vector<int> l10_l_derict = {    0,   -1,     0,    0,   -1,   -1,    -1,   -1,    -1,   -1};
*/
//------------------------------------------------------------------------------------------
// 范围顺序
//    1       2       3       4        5       6        7       8        9      10
// 拇指根部，拇指侧摆，食指根部，中指根部，无名指根部，小指根部，食指侧摆，无名指侧摆，小指侧摆，拇指旋转
//---------------------------------------------------------------------------------------------------
//					   1     2      3      4    5     6     7      8    9    10

const std::vector<double> l10_r_min = {   0, -1.43,    0,    0,    0,    0, -0.26,    0,    0, -0.52};
const std::vector<double> l10_r_max = {0.75,     0, 1.62, 1.62, 1.62, 1.62,     0, 0.13, 0.26,  1.01};
const std::vector<int> l10_r_derict = {  -1,     0,   -1,   -1,   -1,   -1,    -1,    0,    0,    -1};

const std::vector<double> l10_l_min = {-1.45,    0,    0,    0,    0,    0,    0, -0.26, -0.26, -1.01};
const std::vector<double> l10_l_max = {    0, 1.43, 1.62, 1.62, 1.62, 1.62, 0.26,     0,     0,  0.52};
const std::vector<int> l10_l_derict = {    0,   -1,   -1,   -1,   -1,   -1,    0,    -1,    -1,     0};


//------------------------------------------------------------------------------------------

// 确保值在范围内
double is_within_range(double value, double min_value, double max_value) {
    return std::min(max_value, std::max(min_value, value));
}

// 线性映射函数
double scale_value(double original_value, double a_min, double a_max, double b_min, double b_max) {
    return (original_value - a_min) * (b_max - b_min) / (a_max - a_min) + b_min;
}

// 右手范围到弧度（10 关节）
std::vector<double> range_to_arc_right_10(const std::vector<u_int8_t>& hand_range_r) {
    std::vector<double> hand_arc_r(10, 0);
    if (hand_range_r.size() != 10) {
        // std::cerr << "Error: hand_range_r size is not 10." << std::endl;
        return {};
    }
    for (size_t i = 0; i < 10; ++i) {
        double val_r = is_within_range(hand_range_r[i], 0, 255);
        if (l10_r_derict[i] == -1) {
            hand_arc_r[i] = scale_value(val_r, 0, 255, l10_r_max[i], l10_r_min[i]);
        } else {
            hand_arc_r[i] = scale_value(val_r, 0, 255, l10_r_min[i], l10_r_max[i]);
        }
    }
    return hand_arc_r;
}

// 左手范围到弧度（10 关节）
std::vector<double> range_to_arc_left_10(const std::vector<u_int8_t>& hand_range_l) {
    std::vector<double> hand_arc_l(10, 0);
    if (hand_range_l.size() != 10) {
        // std::cerr << "Error: hand_range_l size is not 10." << std::endl;
        return {};
    }
    for (size_t i = 0; i < 10; ++i) {
        double val_l = is_within_range(hand_range_l[i], 0, 255);
        if (l10_l_derict[i] == -1) {
            hand_arc_l[i] = scale_value(val_l, 0, 255, l10_l_max[i], l10_l_min[i]);
        } else {
            hand_arc_l[i] = scale_value(val_l, 0, 255, l10_l_min[i], l10_l_max[i]);
        }
    }
    return hand_arc_l;
}

// 右手弧度到范围（10 关节）
std::vector<u_int8_t> arc_to_range_right_10(const std::vector<double>& hand_arc_r) {
    std::vector<u_int8_t> hand_range_r(10, 0);
    if (hand_arc_r.size() != 10) {
        // std::cerr << "Error: hand_arc_r size is not 10." << std::endl;
        return {};
    }
    for (size_t i = 0; i < 10; ++i) {
        double val_r = is_within_range(hand_arc_r[i], l10_r_min[i], l10_r_max[i]);
        if (l10_r_derict[i] == -1) {
            hand_range_r[i] = static_cast<u_int8_t>(std::round(scale_value(val_r, l10_r_min[i], l10_r_max[i], 255, 0)));
        } else {
            hand_range_r[i] = static_cast<u_int8_t>(std::round(scale_value(val_r, l10_r_min[i], l10_r_max[i], 0, 255)));
        }
    }
    return hand_range_r;
}

// 左手弧度到范围（10 关节）
std::vector<u_int8_t> arc_to_range_left_10(const std::vector<double>& hand_arc_l) {
    std::vector<u_int8_t> hand_range_l(10, 0);
    if (hand_arc_l.size() != 10) {
        // std::cerr << "Error: hand_arc_l size is not 10." << std::endl;
        return {};
    }
    for (size_t i = 0; i < 10; ++i) {
        double val_l = is_within_range(hand_arc_l[i], l10_l_min[i], l10_l_max[i]);
        if (l10_l_derict[i] == -1) {
            hand_range_l[i] = static_cast<u_int8_t>(std::round(scale_value(val_l, l10_l_min[i], l10_l_max[i], 255, 0)));
        } else {
            hand_range_l[i] = static_cast<u_int8_t>(std::round(scale_value(val_l, l10_l_min[i], l10_l_max[i], 0, 255)));
        }
    }
    return hand_range_l;
}

int main() {

    // 示例输入
    // std::vector<u_int8_t> hand_range_r = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    // std::vector<u_int8_t> hand_range_r = {255, 255, 255, 0, 255, 255, 0, 255, 0, 255};
    std::vector<u_int8_t> hand_range_r = {255, 255, 255, 0, 255, 0, 0, 0, 0, 255};
    
    // std::vector<u_int8_t> hand_range_l = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    // std::vector<u_int8_t> hand_range_l = {128, 128, 128, 128, 128, 128, 128, 128, 128, 128};
	// std::vector<u_int8_t> hand_range_l = {255, 255, 255, 0, 255, 255, 0, 255, 0, 255};
    std::vector<u_int8_t> hand_range_l = {255, 255, 255, 0, 255, 0, 0, 0, 0, 255};
    
    // 测试右手范围到弧度
    std::vector<double> hand_arc_r = range_to_arc_right_10(hand_range_r);
    std::cout << "Right hand range to arc: ";
    for (double val : hand_arc_r) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试左手范围到弧度
    std::vector<double> hand_arc_l = range_to_arc_left_10(hand_range_l);
    std::cout << "Left hand range to arc: ";
    for (double val : hand_arc_l) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试右手弧度到范围
    std::vector<u_int8_t> hand_range_r_back = arc_to_range_right_10(hand_arc_r);
    std::cout << "Right hand arc to range: ";
    for (u_int8_t val : hand_range_r_back) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 测试左手弧度到范围
    std::vector<u_int8_t> hand_range_l_back = arc_to_range_left_10(hand_arc_l);
    std::cout << "Left hand arc to range: ";
    for (u_int8_t val : hand_range_l_back) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
