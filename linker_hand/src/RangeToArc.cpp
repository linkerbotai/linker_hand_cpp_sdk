#include <iostream>
#include "RangeToArc.h"

// 确保值在范围内
double is_within_range(double value, double min_value, double max_value) {
    return std::min(max_value, std::max(min_value, value));
}

// 线性映射函数
double scale_value(double original_value, double a_min, double a_max, double b_min, double b_max) {
    return (original_value - a_min) * (b_max - b_min) / (a_max - a_min) + b_min;
}

std::vector<uint8_t> convertToUInt8(const std::vector<int>& input) {
    std::vector<uint8_t> output;
    output.reserve(input.size()); // 预分配内存以提高效率

    for (int value : input) {
        // 确保值在 uint8_t 的范围内
        if (value < 0 || value > 255) {
            std::cout << "警告：值 " << value << " 超出 uint8_t 的范围，将被截断为 " << (value & 0xFF) << std::endl;
        }
        output.push_back(static_cast<uint8_t>(value & 0xFF)); // 截断并转换
    }

    return output;
}

std::vector<int> convertToInt(const std::vector<uint8_t>& input)
{
    std::vector<int> output;
    output.reserve(input.size()); // 预分配内存以提高效率

    for (uint8_t value : input) {
        output.push_back(static_cast<int>(value));
    }

    return output;
}

// 右手范围到弧度（10 关节）
std::vector<double> range_to_arc_right_10(const std::vector<int>& hand_range_r) {
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
std::vector<double> range_to_arc_left_10(const std::vector<int>& hand_range_l) {
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
std::vector<int> arc_to_range_right_10(const std::vector<double>& hand_arc_r) {
    std::vector<int> hand_range_r(10, 0);
    if (hand_arc_r.size() != 10) {
        // std::cerr << "Error: hand_arc_r size is not 10." << std::endl;
        return {};
    }
    for (size_t i = 0; i < 10; ++i) {
        double val_r = is_within_range(hand_arc_r[i], l10_r_min[i], l10_r_max[i]);
        if (l10_r_derict[i] == -1) {
            hand_range_r[i] = static_cast<int>(std::round(scale_value(val_r, l10_r_min[i], l10_r_max[i], 255, 0)));
        } else {
            hand_range_r[i] = static_cast<int>(std::round(scale_value(val_r, l10_r_min[i], l10_r_max[i], 0, 255)));
        }
    }
    return hand_range_r;
}

// 左手弧度到范围（10 关节）
std::vector<int> arc_to_range_left_10(const std::vector<double>& hand_arc_l) {
    std::vector<int> hand_range_l(10, 0);
    if (hand_arc_l.size() != 10) {
        // std::cerr << "Error: hand_arc_l size is not 10." << std::endl;
        return {};
    }
    for (size_t i = 0; i < 10; ++i) {
        double val_l = is_within_range(hand_arc_l[i], l10_l_min[i], l10_l_max[i]);
        if (l10_l_derict[i] == -1) {
            hand_range_l[i] = static_cast<int>(std::round(scale_value(val_l, l10_l_min[i], l10_l_max[i], 255, 0)));
        } else {
            hand_range_l[i] = static_cast<int>(std::round(scale_value(val_l, l10_l_min[i], l10_l_max[i], 0, 255)));
        }
    }
    return hand_range_l;
}
