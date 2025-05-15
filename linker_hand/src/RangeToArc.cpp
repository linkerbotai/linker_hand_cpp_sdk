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

bool initialize_params(int joints_type, const std::string& left_or_right, std::vector<double>& min_limits, std::vector<double>& max_limits, std::vector<int>& derict) {
    min_limits.clear();
    max_limits.clear();
    derict.clear();
    
    switch(joints_type) {
        case 7:
            if (left_or_right == "left") {
                min_limits = l7_l_min;
                max_limits = l7_l_max;
                derict = l7_l_derict;
            } else if (left_or_right == "right") {
                min_limits = l7_r_min;
                max_limits = l7_r_max;
                derict = l7_r_derict;
            }
            break;
        case 10:
            if (left_or_right == "left") {
                min_limits = l10_l_min;
                max_limits = l10_l_max;
                derict = l10_l_derict;
            } else if (left_or_right == "right") {
                min_limits = l10_r_min;
                max_limits = l10_r_max;
                derict = l10_r_derict;
            }
            break;
        case 20:
            if (left_or_right == "left") {
                min_limits = l20_l_min;
                max_limits = l20_l_max;
                derict = l20_l_derict;
            } else if (left_or_right == "right") {
                min_limits = l20_r_min;
                max_limits = l20_r_max;
                derict = l20_r_derict;
            }
            break;
        case 21:
            if (left_or_right == "left") {
                min_limits = l21_l_min;
                max_limits = l21_l_max;
                derict = l21_l_derict;
            } else if (left_or_right == "right") {
                min_limits = l21_r_min;
                max_limits = l21_r_max;
                derict = l21_r_derict;
            }
            break;
        case 25:
            if (left_or_right == "left") {
                min_limits = l25_l_min;
                max_limits = l25_l_max;
                derict = l25_l_derict;
            } else if (left_or_right == "right") {
                min_limits = l25_r_min;
                max_limits = l25_r_max;
                derict = l25_r_derict;
            }
            break;
        default:
            return false;
    }
    
    return !min_limits.empty() && !max_limits.empty() && !derict.empty();
}

bool should_skip_joint(int joints_type, int joint_index) {
    if (joints_type == 21) {
        return (11 <= joint_index && joint_index <= 14) || (16 <= joint_index && joint_index <= 19);
    } else if (joints_type == 25) {
        return 11 <= joint_index && joint_index <= 14;
    }
    return false;
}

std::vector<double> range_to_arc(const int &joints_type, const std::string &left_or_right, const std::vector<uint8_t> &hand_range) {
    
	std::vector<double> hand_arc;
	std::vector<double> min_limits;
	std::vector<double> max_limits;
	std::vector<int> derict;
	
	if (!initialize_params(joints_type, left_or_right, min_limits, max_limits, derict)) {
        std::cerr << "Error: Invalid joints_type or left_or_right parameter." << std::endl;
        return {};
    }
    
    if (hand_range.size() != max_limits.size()) {
        std::cerr << "Error: Invalid hand_range size." << std::endl;
        return {};
    }
    
    hand_arc = std::vector<double>(max_limits.size(), 0);
    
    for (size_t i = 0; i < hand_arc.size(); ++i) {
    	if (should_skip_joint(joints_type, i)) {
            continue;
        }
    
        double val = is_within_range(hand_range[i], 0, 255);
        if (derict[i] == -1) {
            hand_arc[i] = scale_value(val, 0, 255, max_limits[i], min_limits[i]);
        } else {
            hand_arc[i] = scale_value(val, 0, 255, min_limits[i], max_limits[i]);
        }
    }
    
    return hand_arc;
}


std::vector<uint8_t> arc_to_range(const int &joints_type, const std::string &left_or_right, const std::vector<double> &hand_arc) {

	std::vector<uint8_t> hand_range;
	std::vector<double> min_limits;
	std::vector<double> max_limits;
	std::vector<int> derict;
	
	if (!initialize_params(joints_type, left_or_right, min_limits, max_limits, derict)) {
        std::cerr << "Error: Invalid joints_type or left_or_right parameter." << std::endl;
        return {};
    }
    
    if (hand_arc.size() != max_limits.size()) {
        return {};
    }
    
	hand_range = std::vector<uint8_t>(max_limits.size(), 0);
	
	for (int i = 0; i < hand_range.size(); ++i) {
        if (should_skip_joint(joints_type, i)) {
            continue;
        }
    	
        double val = is_within_range(hand_arc[i], min_limits[i], max_limits[i]);
        if (derict[i] == -1) {
            hand_range[i] = static_cast<uint8_t>(std::round(scale_value(val, min_limits[i], max_limits[i], 255, 0)));
        } else {
            hand_range[i] = static_cast<uint8_t>(std::round(scale_value(val, min_limits[i], max_limits[i], 0, 255)));
        }
    }
    
    return hand_range;
}
