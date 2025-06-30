#include <iostream>
#include <chrono>
#include <signal.h>
#include <csignal>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/header.hpp>
#include <thread>

using namespace std::chrono_literals;
using namespace std::placeholders;
using namespace std;

int show_count = 0;
int show_count_obj = 0;
int show_step = 0;
vector<string> joint_order = {"joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7", "joint8", "joint9", "joint10", "joint11", "joint12", "joint13", "joint14", "joint15", "joint16", "joint17", "joint18", "joint19", "joint20", "joint21", "joint22", "joint23", "joint24", "joint25"};

std::map<std::string, int> hand = {
    {"joint1", 250},
    {"joint2", 250},
    {"joint3", 250},
    {"joint4", 250},
    {"joint5", 250},
    {"joint6", 250},
    {"joint7", 250},
    {"joint8", 250},
    {"joint9", 250},
    {"joint10", 250},
    {"joint11", 250},
    {"joint12", 0},
    {"joint13", 0},
    {"joint14", 0},
    {"joint15", 0},
    {"joint16", 250},
    {"joint17", 250},
    {"joint18", 250},
    {"joint19", 250},
    {"joint20", 250},
    {"joint21", 250},
    {"joint22", 250},
    {"joint23", 250},
    {"joint24", 250},
    {"joint25", 250}
};

std::vector<double> show_left() {
    static int show_count = 0;
    static int show_count_obj = 0;
    static int show_step = 0;

    show_count++;
    if (show_count >= show_count_obj) {
        show_count = 0;
        switch (show_step) {
            case 0:
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
            case 1:
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
            case 2:
                show_step++;
                show_count_obj = 30;
                hand["joint1"] = 100;
                hand["joint6"] = 180;
                hand["joint16"] = 0;
                hand["joint21"] = 0;
                break;
            case 3:
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 200;
                hand["joint8"] = 200;
                hand["joint11"] = 200;
                break;
            case 4:
                show_step++;
                show_count_obj = 13;
                hand["joint7"] = 50;
                hand["joint8"] = 50;
                break;
            case 5:
                show_step++;
                show_count_obj = 13;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                break;
            case 6:
                show_step++;
                show_count_obj = 2;
                hand["joint7"] = 50;
                hand["joint8"] = 200;
                break;
            case 7:
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                break;
            case 8:
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 50;
                hand["joint8"] = 200;
                break;
            case 9:
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                break;
            case 10:
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 100;
                hand["joint3"] = 100;
                hand["joint17"] = 100;
                hand["joint18"] = 100;
                hand["joint22"] = 100;
                hand["joint23"] = 100;
                break;
            case 11:
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 250;
                hand["joint3"] = 250;
                hand["joint17"] = 250;
                hand["joint18"] = 250;
                hand["joint22"] = 250;
                hand["joint23"] = 250;
                break;
            case 12:
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 100;
                hand["joint3"] = 100;
                hand["joint17"] = 100;
                hand["joint18"] = 100;
                hand["joint22"] = 100;
                hand["joint23"] = 100;
                break;
            case 13:
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 250;
                hand["joint3"] = 250;
                hand["joint17"] = 250;
                hand["joint18"] = 250;
                hand["joint22"] = 250;
                hand["joint23"] = 250;
                break;
            case 14:
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 250;
                hand["joint6"] = 150;
                hand["joint11"] = 250;
                hand["joint23"] = 250;
                break;
            case 15:
                show_step++;
                show_count_obj = 10;
                hand["joint6"] = 5;
                break;
            case 16:
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
            case 17:
                show_step++;
                show_count_obj = 15;
                hand["joint5"] = 250;
                hand["joint20"] = 250;
                hand["joint25"] = 250;
                break;
            case 18:
                show_step++;
                show_count_obj = 15;
                hand["joint4"] = 250;
                hand["joint19"] = 250;
                hand["joint24"] = 250;
                break;
            case 19:
                show_step++;
                show_count_obj = 15;
                hand["joint3"] = 250;
                hand["joint18"] = 250;
                hand["joint23"] = 250;
                break;
            case 20:
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 250;
                hand["joint17"] = 250;
                hand["joint22"] = 250;
                break;
            case 21:
                show_step++;
                show_count_obj = 10;
                hand["joint6"] = 250;
                hand["joint16"] = 250;
                hand["joint21"] = 250;
                break;
            case 22:
                show_step++;
                show_count_obj = 20;
                hand["joint11"] = 10;
                break;
            case 23:
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 0;
                break;
            case 24:
                show_step++;
                show_count_obj = 30;
                hand["joint11"] = 250;
                break;
            case 25:
                show_step++;
                show_count_obj = 50;
                break;
            case 26:
                show_step++;
                show_count_obj = 10;
                hand["joint7"] = 200;
                hand["joint8"] = 200;
                hand["joint9"] = 200;
                hand["joint10"] = 200;
                break;
            case 27:
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 80;
                hand["joint8"] = 80;
                hand["joint9"] = 80;
                hand["joint10"] = 80;
                break;
            case 28:
                show_step++;
                show_count_obj = 20;
                hand["joint7"] = 128;
                hand["joint8"] = 128;
                hand["joint9"] = 128;
                hand["joint10"] = 128;
                break;
            case 29:
                show_step++;
                show_count_obj = 15;
                hand["joint17"] = 0;
                hand["joint22"] = 0;
                break;
            case 30:
                show_step++;
                show_count_obj = 15;
                hand["joint18"] = 0;
                hand["joint23"] = 0;
                break;
            case 31:
                show_step++;
                show_count_obj = 15;
                hand["joint19"] = 0;
                hand["joint24"] = 0;
                break;
            case 32:
                show_step++;
                show_count_obj = 15;
                hand["joint20"] = 0;
                hand["joint25"] = 0;
                break;
            case 33:
                show_step++;
                show_count_obj = 15;
                hand["joint2"] = 0;
                break;
            case 34:
                show_step++;
                show_count_obj = 15;
                hand["joint3"] = 0;
                break;
            case 35:
                show_step++;
                show_count_obj = 15;
                hand["joint4"] = 0;
                break;
            case 36:
                show_step++;
                show_count_obj = 15;
                hand["joint5"] = 0;
                break;
            case 37:
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 0;
                hand["joint16"] = 200;
                break;
            case 38:
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 250;
                hand["joint16"] = 250;
                break;
            case 39:
                show_step++;
                show_count_obj = 30;
                hand["joint2"] = 250;
                hand["joint5"] = 250;
                hand["joint17"] = 250;
                hand["joint20"] = 250;
                hand["joint22"] = 250;
                hand["joint25"] = 250;
                break;
            case 40:
                show_step++;
                show_count_obj = 40;
                hand["joint1"] = 100;
                hand["joint6"] = 200;
                hand["joint11"] = 100;
                hand["joint16"] = 100;
                break;
            case 41:
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 80;
                hand["joint10"] = 200;
                break;
            case 42:
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 200;
                hand["joint10"] = 80;
                break;
            case 43:
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 80;
                hand["joint10"] = 200;
                break;
            case 44:
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 200;
                hand["joint10"] = 80;
                break;
            case 45:
                show_step++;
                show_count_obj = 15;
                hand["joint7"] = 128;
                hand["joint10"] = 128;
                break;
            case 46:
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
            case 47:
                show_step++;
                show_count_obj = 50;
                hand["joint1"] = 40;
                hand["joint2"] = 0;
                hand["joint6"] = 100;
                hand["joint11"] = 70;
                hand["joint17"] = 240;
                hand["joint22"] = 240;
                break;
            case 48:
                show_step++;
                show_count_obj = 20;
                hand["joint2"] = 250;
                hand["joint6"] = 220;
                hand["joint11"] = 100;
                hand["joint17"] = 250;
                hand["joint22"] = 250;
                break;
            case 49:
                show_step++;
                show_count_obj = 35;
                hand["joint3"] = 0;
                hand["joint6"] = 70;
                hand["joint11"] = 60;
                hand["joint18"] = 220;
                hand["joint23"] = 220;
                break;
            case 50:
                show_step++;
                show_count_obj = 20;
                hand["joint3"] = 250;
                hand["joint6"] = 100;
                hand["joint11"] = 100;
                hand["joint18"] = 250;
                hand["joint23"] = 250;
                break;
            case 51:
                show_step++;
                show_count_obj = 35;
                hand["joint4"] = 0;
                hand["joint6"] = 30;
                hand["joint11"] = 50;
                hand["joint19"] = 220;
                hand["joint24"] = 220;
                break;
            case 52:
                show_step++;
                show_count_obj = 20;
                hand["joint4"] = 250;
                hand["joint6"] = 100;
                hand["joint11"] = 100;
                hand["joint19"] = 250;
                hand["joint24"] = 250;
                break;
            case 53:
                show_step++;
                show_count_obj = 40;
                hand["joint5"] = 0;
                hand["joint6"] = 0;
                hand["joint11"] = 40;
                hand["joint20"] = 230;
                hand["joint25"] = 230;
                break;
            case 54:
                show_step++;
                show_count_obj = 20;
                hand["joint5"] = 20;
                hand["joint6"] = 0;
                hand["joint11"] = 100;
                hand["joint20"] = 250;
                hand["joint25"] = 250;
                break;
            case 55:
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
            case 56:
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
            case 57:
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
            case 58:
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
            case 59:
                show_step++;
                show_count_obj = 35;
                hand["joint1"] = 155;
                hand["joint3"] = 155;
                hand["joint6"] = 70;
                hand["joint11"] = 60;
                hand["joint16"] = 130;
                hand["joint18"] = 90;
                hand["joint21"] = 130;
                hand["joint24"] = 80;
                break;
            case 60:
                show_step++;
                show_count_obj = 20;
                hand["joint1"] = 250;
                hand["joint3"] = 250;
                hand["joint6"] = 100;
                hand["joint11"] = 100;
                hand["joint16"] = 250;
                hand["joint18"] = 250;
                hand["joint21"] = 250;
                hand["joint24"] = 250;
                break;
            case 61:
                show_step++;
                show_count_obj = 35;
                hand["joint1"] = 165;
                hand["joint2"] = 165;
                hand["joint6"] = 100;
                hand["joint11"] = 70;
                hand["joint16"] = 130;
                hand["joint18"] = 80;
                hand["joint21"] = 130;
                hand["joint24"] = 80;
                break;
            default:
                show_step = 0;
                break;
        }
    }

    std::vector<double> positions;
    positions.clear();
    for (const auto& joint : joint_order) { // 按照固定的顺序提取关节值
        positions.push_back(static_cast<double>(hand[joint]));
    }
    return positions;
}

class HandController : public rclcpp::Node {
public:
    HandController() : Node("dong_test_sender") {
        publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/right_hand_control", 10);
        timer_ = this->create_wall_timer(33ms, std::bind(&HandController::publishJointState, this));
    }

private:

    void publishJointState() {
        sensor_msgs::msg::JointState joint_state;
        joint_state.header.stamp = this->now();
        std::vector<std::string> joint_names;
        for (const auto& entry : hand) {
            joint_names.push_back(entry.first);
        }
        joint_state.name = joint_names;
        vector<double> position = show_left();
        joint_state.position = position;
        joint_state.velocity.resize(position.size(), 100.0);
        joint_state.effort.resize(position.size(), 200.0);
        publisher_->publish(joint_state);
    }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

void signal_handler(int sig) {
    (void) sig;
    rclcpp::shutdown();
}

int main(int argc, char** argv) {
    std::signal(SIGINT, signal_handler);
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HandController>());
    rclcpp::shutdown();
    return 0;
}