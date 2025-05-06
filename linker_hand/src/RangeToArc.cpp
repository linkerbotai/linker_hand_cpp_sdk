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
