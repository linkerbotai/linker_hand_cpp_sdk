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

std::string hand_joint = "L25"; // 控制L25版本灵巧手
std::string hand_type = "right"; // 控制左手

class HandController : public rclcpp::Node {
public:
    HandController() : Node("dong_test_sender") {
        if (hand_type == "left") {
            publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/left_hand_control", 10);
        } else if (hand_type == "right") {
            publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/right_hand_control", 10);
        }

        if (hand_joint == "L25") {
            pos1_1 = {230, 0, 0, 15, 5, 250, 55, 0, 75, 95, 85, 0, 0, 0, 0, 250, 0, 40, 35, 5, 250, 0, 5, 0, 0};
            pos1_2 = {80, 255, 255, 255, 255, 180, 51, 51, 72, 202, 202, 255.0, 255.0, 255.0, 255.0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
            pos2_1 = {230, 0, 0, 15, 5, 250, 55, 0, 75, 95, 85, 0, 0, 0, 0, 80, 0, 40, 35, 5, 250, 0, 5, 0, 0};
            pos2_2 = {230, 0, 0, 15, 5, 42, 55, 0, 75, 95, 85, 0, 0, 0, 0, 90, 0, 40, 35, 5, 120, 0, 5, 0, 0};
        }

        joint_state.name = {
            "joint1", "joint2", "joint3", "joint4", "joint5", "joint6",
            "joint7", "joint8", "joint9", "joint10", "joint11", "joint12",
            "joint13", "joint14", "joint15", "joint16", "joint17", "joint18",
            "joint19", "joint20"
        };

        timer_ = this->create_wall_timer(33ms, std::bind(&HandController::timer_callback, this));
    }

private:
    void timer_callback() {
        auto message = sensor_msgs::msg::JointState();
        message.header.stamp = this->now();

        joint_state.position = pos2_1;
        // joint_state.velocity.resize(joint_state.position.size(), 0.0);
        // joint_state.effort.resize(joint_state.position.size(), 0.0);
        message = joint_state;
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published position 1");

        std::this_thread::sleep_for(1.3s);

        joint_state.position = pos2_2;
        // joint_state.velocity.resize(joint_state.position.size(), 0.0);
        // joint_state.effort.resize(joint_state.position.size(), 0.0);
        message = joint_state;
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published position 2");

        std::this_thread::sleep_for(5s);

        joint_state.position = pos1_1;
        // joint_state.velocity.resize(joint_state.position.size(), 0.0);
        // joint_state.effort.resize(joint_state.position.size(), 0.0);
        message = joint_state;
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published position 3");

        std::this_thread::sleep_for(0.5s);

        joint_state.position = pos1_2;
        // joint_state.velocity.resize(joint_state.position.size(), 0.0);
        // joint_state.effort.resize(joint_state.position.size(), 0.0);
        message = joint_state;
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published position 4");

        RCLCPP_INFO(this->get_logger(), "Cycle completed %d times", count);
        count++;
        std::this_thread::sleep_for(3s);
    }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    sensor_msgs::msg::JointState joint_state;
    std::vector<double> pos1_1, pos1_2, pos2_1, pos2_2;
    int count = 0;
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