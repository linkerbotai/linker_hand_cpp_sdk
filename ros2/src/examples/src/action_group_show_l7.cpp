#include <iostream>
#include <chrono>
#include <signal.h>
#include <csignal>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/header.hpp>
#include <thread>
#include <map>
#include <vector>

using namespace std::chrono_literals;
using namespace std::placeholders;

int show_count = 0;
int show_count_obj = 0;
int show_step = 0;

std::map<std::string, int> hand = {
    {"joint1", 255},  // 拇指根部弯曲
    {"joint2", 128},  // 拇指侧摆
    {"joint3", 255},  // 食指根部弯曲
    {"joint4", 255},  // 中指根部弯曲
    {"joint5", 255},  // 无名指根部弯曲
    {"joint6", 255},  // 小指根部弯曲
    {"joint7", 255}   // 拇指旋转
};

class HandController : public rclcpp::Node {
public:
    HandController() : Node("dong_test_sender") {
        publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/right_hand_control", 10);
        timer_ = this->create_wall_timer(30ms, std::bind(&HandController::timer_callback, this));
    }

private:
    void timer_callback() {
        auto message = sensor_msgs::msg::JointState();
        message.header.stamp = this->now();
        message.name = {"joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7"};
        message.position = show_left();
        message.velocity.resize(message.position.size(), 100.0);
        message.effort.resize(message.position.size(), 200.0);
        publisher_->publish(message);
    }

    std::vector<double> show_left() {
        show_count++;
        if (show_count >= show_count_obj) {
            show_count = 0;
            switch (show_step) {
                case 0:
                    show_step++;
                    show_count_obj = 100;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint3"] = 250;
                    hand["joint4"] = 250;
                    hand["joint5"] = 250;
                    hand["joint6"] = 250;
                    hand["joint7"] = 250;
                    break;
                case 1:
                    show_step++;
                    show_count_obj = 10;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint5"] = 0;
                    hand["joint6"] = 0;
                    hand["joint7"] = 250;
                    break;
                case 2:
                    show_step += 8;
                    show_count_obj = 30;
                    hand["joint1"] = 40;
                    hand["joint2"] = 240;
                    hand["joint7"] = 80;
                    break;
                case 3:
                    show_step++;
                    break;
                case 4:
                    show_step++;
                    break;
                case 5:
                    show_step++;
                    break;
                case 6:
                    show_step++;
                    break;
                case 7:
                    show_step++;
                    break;
                case 8:
                    show_step++;
                    break;
                case 9:
                    show_step++;
                    break;
                case 10:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint3"] = 100;
                    hand["joint4"] = 100;
                    break;
                case 11:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint3"] = 250;
                    hand["joint4"] = 250;
                    break;
                case 12:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint3"] = 100;
                    hand["joint4"] = 100;
                    break;
                case 13:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint3"] = 250;
                    hand["joint4"] = 250;
                    hand["joint5"] = 250;
                    hand["joint6"] = 250;
                    hand["joint7"] = 250;
                    break;
                case 14:
                    show_step++;
                    show_count_obj = 40;
                    hand["joint1"] = 40;
                    hand["joint2"] = 240;
                    hand["joint7"] = 120;
                    break;
                case 15:
                    show_step++;
                    break;
                case 16:
                    show_step++;
                    show_count_obj = 30;
                    hand["joint3"] = 10;
                    hand["joint4"] = 10;
                    hand["joint5"] = 10;
                    hand["joint6"] = 10;
                    break;
                case 17:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint6"] = 250;
                    break;
                case 18:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint5"] = 250;
                    break;
                case 19:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint4"] = 250;
                    break;
                case 20:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint3"] = 250;
                    break;
                case 21:
                    show_step += 4;
                    show_count_obj = 20;
                    hand["joint1"] = 250;
                    hand["joint2"] = 110;
                    hand["joint7"] = 240;
                    break;
                case 22:
                    show_step++;
                    show_count_obj = 20;
                    hand["joint1"] = 250;
                    hand["joint2"] = 10;
                    hand["joint7"] = 110;
                    break;
                case 23:
                    show_step++;
                    show_count_obj = 40;
                    hand["joint1"] = 0;
                    hand["joint2"] = 10;
                    hand["joint7"] = 110;
                    break;
                case 24:
                    show_step++;
                    show_count_obj = 30;
                    hand["joint1"] = 0;
                    hand["joint2"] = 240;
                    hand["joint7"] = 110;
                    break;
                case 25:
                    show_step += 4;
                    show_count_obj = 50;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint7"] = 110;
                    break;
                case 26:
                    show_step++;
                    break;
                case 27:
                    show_step++;
                    break;
                case 28:
                    show_step++;
                    break;
                case 29:
                    show_step += 4;
                    show_count_obj = 15;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint7"] = 250;
                    break;
                case 30:
                    show_step++;
                    break;
                case 31:
                    show_step++;
                    break;
                case 32:
                    show_step++;
                    break;
                case 33:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint3"] = 0;
                    hand["joint4"] = 250;
                    hand["joint5"] = 250;
                    hand["joint6"] = 250;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint7"] = 250;
                    break;
                case 34:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint4"] = 0;
                    break;
                case 35:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint5"] = 0;
                    break;
                case 36:
                    show_step++;
                    show_count_obj = 15;
                    hand["joint6"] = 0;
                    break;
                case 37:
                    show_step++;
                    show_count_obj = 40;
                    hand["joint1"] = 0;
                    break;
                case 38:
                    show_step++;
                    show_count_obj = 40;
                    hand["joint1"] = 250;
                    hand["joint2"] = 230;
                    hand["joint7"] = 250;
                    break;
                case 39:
                    show_step++;
                    show_count_obj = 30;
                    hand["joint3"] = 250;
                    hand["joint6"] = 250;
                    break;
                case 40:
                    show_step++;
                    show_count_obj = 40;
                    hand["joint1"] = 10;
                    hand["joint2"] = 40;
                    hand["joint7"] = 60;
                    break;
                case 41:
                    show_step += 5;
                    show_count_obj = 5;
                    hand["joint1"] = 50;
                    break;
                case 42:
                    show_step++;
                    break;
                case 43:
                    show_step++;
                    break;
                case 44:
                    show_step++;
                    break;
                case 45:
                    show_step++;
                    break;
                case 46:
                    show_step++;
                    show_count_obj = 50;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint3"] = 250;
                    hand["joint4"] = 250;
                    hand["joint5"] = 250;
                    hand["joint6"] = 250;
                    hand["joint7"] = 250;
                    break;
                case 47:
                    show_step++;
                    show_count_obj = 50;
                    hand["joint1"] = 120;
                    hand["joint2"] = 130;
                    hand["joint3"] = 155;
                    hand["joint4"] = 250;
                    hand["joint5"] = 250;
                    hand["joint6"] = 250;
                    hand["joint7"] = 90;
                    break;
                case 48:
                    show_step++;
                    show_count_obj = 20;
                    hand["joint1"] = 250;
                    hand["joint3"] = 250;
                    break;
                case 49:
                    show_step++;
                    show_count_obj = 35;
                    hand["joint1"] = 120;
                    hand["joint4"] = 140;
                    hand["joint7"] = 60;
                    break;
                case 50:
                    show_step++;
                    show_count_obj = 30;
                    hand["joint1"] = 250;
                    hand["joint4"] = 250;
                    break;
                case 51:
                    show_step++;
                    show_count_obj = 35;
                    hand["joint1"] = 120;
                    hand["joint2"] = 125;
                    hand["joint5"] = 145;
                    hand["joint7"] = 40;
                    break;
                case 52:
                    show_step++;
                    show_count_obj = 30;
                    hand["joint1"] = 250;
                    hand["joint5"] = 250;
                    break;
                case 53:
                    show_step++;
                    show_count_obj = 40;
                    hand["joint1"] = 120;
                    hand["joint6"] = 135;
                    hand["joint7"] = 15;
                    break;
                case 54:
                    show_step++;
                    show_count_obj = 20;
                    hand["joint1"] = 250;
                    hand["joint2"] = 250;
                    hand["joint3"] = 250;
                    hand["joint4"] = 250;
                    hand["joint5"] = 250;
                    hand["joint6"] = 250;
                    hand["joint7"] = 250;
                    break;
                default:
                    show_step = 0;
                    break;
            }
        }

        std::vector<double> positions;
        for (const auto& entry : hand) {
            positions.push_back(static_cast<double>(entry.second));
        }
        return positions;
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