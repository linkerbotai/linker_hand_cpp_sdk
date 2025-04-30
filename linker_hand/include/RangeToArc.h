#ifndef RANGE_TO_ARC_H
#define RANGE_TO_ARC_H

#include <vector>
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
double is_within_range(double value, double min_value, double max_value);

// 线性映射函数
double scale_value(double original_value, double a_min, double a_max, double b_min, double b_max);

// int 转换为 uint8_t
std::vector<uint8_t> convertToUInt8(const std::vector<int>& input);

// uint8_t 转换为 int
std::vector<int> convertToInt(const std::vector<uint8_t>& input);

// 右手范围到弧度（10 关节）
std::vector<double> range_to_arc_right_10(const std::vector<int>& hand_range_r);

// 左手范围到弧度（10 关节）
std::vector<double> range_to_arc_left_10(const std::vector<int>& hand_range_l);

// 右手弧度到范围（10 关节）
std::vector<int> arc_to_range_right_10(const std::vector<double>& hand_arc_r);

// 左手弧度到范围（10 关节）
std::vector<int> arc_to_range_left_10(const std::vector<double>& hand_arc_l);

#endif // RANGE_TO_ARC_H
