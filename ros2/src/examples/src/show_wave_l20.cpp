// 本demo仅支持L20，默认为右手，如需左手，请修改话题名

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <vector>
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;
using namespace std::chrono_literals;

bool running = true;

int show_count = 0;
int show_count_obj = 0;
int show_step = 0;

vector<double> show_left() {
    vector<double> position;
    show_count++;
    if (show_count >= show_count_obj) {
        show_count = 0;
        switch (show_step) {
            case 0:
                show_step++;
                show_count_obj = 40;
                position = {128, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 1:
                show_step++;
                show_count_obj = 30;
                position = {128, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 2:
                show_step++;
                show_count_obj = 33;
                position = {128, 0, 0, 0, 0, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 3:
                show_step++;
                show_count_obj = 30;
                position = {128, 0, 0, 0, 0, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 4:
                show_step++;
                show_count_obj = 30;
                position = {128, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            default:
                show_step = 0;
                position = {128, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
        }
    }
    return position;
}

class JointStatePublisher : public rclcpp::Node {
public:
    JointStatePublisher() : Node("dong_test_sender") {
        joint_state_pub = this->create_publisher<sensor_msgs::msg::JointState>("/right_hand_control", 10);
        timer = this->create_wall_timer(33ms, std::bind(&JointStatePublisher::publishJointState, this));
    }

private:
    void publishJointState() {
        sensor_msgs::msg::JointState joint_state;
        joint_state.header.stamp = this->now();
        joint_state.name = {"joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7", "joint8", "joint9", "joint10", "joint11", "joint12", "joint13", "joint14", "joint15", "joint16", "joint17", "joint18", "joint19", "joint20"};
        vector<double> position = show_left();
        if (!position.empty()) {
            joint_state.position = position;
        }
        joint_state.velocity.resize(joint_state.position.size(), 100.0);
        joint_state.effort.resize(joint_state.position.size(), 200.0);
        if (joint_state.position.size() > 0) joint_state_pub->publish(joint_state);
    }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub;
    rclcpp::TimerBase::SharedPtr timer;
};

void signalHandler(int sig) {
    (void) sig;
    running = false;
    rclcpp::shutdown();
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    signal(SIGINT, signalHandler);
    rclcpp::spin(std::make_shared<JointStatePublisher>());
    return 0;
}