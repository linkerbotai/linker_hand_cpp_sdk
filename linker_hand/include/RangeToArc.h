#ifndef RANGE_TO_ARC_H
#define RANGE_TO_ARC_H

#include <vector>
#include <stdexcept>
#include <cmath>


//---------------------------------------------------------------------------------------------------
// L10 L OK
const std::vector<double> l10_l_min = {0, 0, 0, 0, 0, 0, 0, -0.26, -0.26, -0.52};
const std::vector<double> l10_l_max = {1.45, 1.43, 1.62, 1.62, 1.62, 1.62, 0.26, 0, 0, 1.01};
const std::vector<int> l10_l_derict = {-1, -1, -1, -1, -1, -1, 0, -1, -1, -1};
// L10 R OK
const std::vector<double> l10_r_min = {0, 0, 0, 0, 0, 0, -0.26, 0, 0, -0.52};
const std::vector<double> l10_r_max = {0.75, 1.43, 1.62, 1.62, 1.62, 1.62, 0, 0.13, 0.26, 1.01};
const std::vector<int> l10_r_derict = {-1, -1, -1, -1, -1, -1, -1, 0, 0, -1};
//---------------------------------------------------------------------------------------------------
// L20 L 待验证
const std::vector<double> l20_l_min = {0, 0, 0, 0, 0, -0.297, -0.26, -0.26, -0.26, -0.26, 0.122, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const std::vector<double> l20_l_max = {0.87, 1.4, 1.4, 1.4, 1.4, 0.683, 0.26, 0.26, 0.26, 0.26, 1.78, 0, 0, 0, 0, 1.29, 1.08, 1.08, 1.08, 1.08};
const std::vector<int> l20_l_derict = {-1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1};
// L20 R OK
const std::vector<double> l20_r_min = {0, 0, 0, 0, 0, -0.297, -0.26, -0.26, -0.26, -0.26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const std::vector<double> l20_r_max = {0.87, 1.4, 1.4, 1.4, 1.4, 0.683, 0.26, 0.26, 0.26, 0.26, 1.78, 0, 0, 0, 0, 1.29, 1.08, 1.08, 1.08, 1.08};
const std::vector<int> l20_r_derict = {-1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1};
//---------------------------------------------------------------------------------------------------
// L7 L OK
const std::vector<double> l7_l_min = {0, 0, 0, 0, 0, 0, -0.52};
const std::vector<double> l7_l_max = {0.44, 1.43, 1.62, 1.62, 1.62, 1.62, 1.01};
const std::vector<int> l7_l_derict = {-1, -1, -1, -1, -1, -1, -1};
// L7 R OK (urdf后续会更改！！！)
const std::vector<double> l7_r_min = {0, -1.43, 0, 0, 0, 0, 0};
const std::vector<double> l7_r_max = {0.75, 0, 1.62, 1.62, 1.62, 1.62, 1.54};
const std::vector<int> l7_r_derict = {-1, 0, -1, -1, -1, -1, -1};
//---------------------------------------------------------------------------------------------------

// 确保值在范围内
double is_within_range(double value, double min_value, double max_value);

// 线性映射函数
double scale_value(double original_value, double a_min, double a_max, double b_min, double b_max);

// 右手范围到弧度（L10）
std::vector<double> range_to_arc_right_10(const std::vector<u_int8_t>& hand_range_r);

// 左手范围到弧度（L10）
std::vector<double> range_to_arc_left_10(const std::vector<u_int8_t>& hand_range_l);

// 右手弧度到范围（L10）
std::vector<u_int8_t> arc_to_range_right_10(const std::vector<double>& hand_arc_r);

// 左手弧度到范围（L10）
std::vector<u_int8_t> arc_to_range_left_10(const std::vector<double>& hand_arc_l);


// 右手范围到弧度（L20）
std::vector<double> range_to_arc_right_20(const std::vector<u_int8_t>& hand_range_r);

// 左手范围到弧度（L20）
std::vector<double> range_to_arc_left_20(const std::vector<u_int8_t>& hand_range_l);

// 右手弧度到范围（L20）
std::vector<u_int8_t> arc_to_range_right_20(const std::vector<double>& hand_arc_r);

// 左手弧度到范围（L20）
std::vector<u_int8_t> arc_to_range_left_20(const std::vector<double>& hand_arc_l);

#endif // RANGE_TO_ARC_H
