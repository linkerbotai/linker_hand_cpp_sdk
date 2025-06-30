// 本demo仅支持L20，默认为右手，如需左手，请修改hand_type

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <csignal>

using namespace std;
using namespace std::chrono_literals;

bool running = true;
vector<double> pos1 = {255, 255, 255, 255, 255, 255, 10, 100, 180, 240, 245, 255, 255, 255, 255, 255, 255, 255, 255, 255};
vector<double> pos2 = {69.0, 0.0, 0.0, 0.0, 0.0, 151.0, 10.0, 100.0, 180.0, 240.0, 14.0, 255.0, 255.0, 255.0, 255.0, 109.0, 0.0, 0.0, 0.0, 0.0};
vector<string> joint_names = {"joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7", "joint8", "joint9", "joint10", "joint11", "joint12", "joint13", "joint14", "joint15", "joint16", "joint17", "joint18", "joint19", "joint20"};


class JointStatePublisher : public rclcpp::Node {
public:
    JointStatePublisher() : Node("dong_test_sender") {
        string hand_type = "right"; // 控制右手
        string topic_name;
        if (hand_type == "left") {
            topic_name = "/left_hand_control";
        } else if (hand_type == "right") {
            topic_name = "/right_hand_control";
        }
        joint_state_pub = this->create_publisher<sensor_msgs::msg::JointState>(topic_name, 10);
        timer = this->create_wall_timer(33ms, std::bind(&JointStatePublisher::publishJointState, this));
    }

private:
    void publishJointState() {
        sensor_msgs::msg::JointState joint_state;
        joint_state.header.stamp = this->now();
        joint_state.name = joint_names;
        joint_state.velocity.resize(joint_names.size(), 100.0);
        joint_state.effort.resize(joint_names.size(), 200.0);

        static int msg_count = 0;
        if (msg_count < 100) {
            joint_state.position = pos1;
            msg_count++;
        } else if (msg_count < 200) {
            joint_state.position = pos2;
            msg_count++;
        } else {
            msg_count = 0;
            count++;
            RCLCPP_INFO(this->get_logger(), "Loop completed %d times", count);
        }
        if (joint_state.position.size() > 0)
            joint_state_pub->publish(joint_state);
    }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub;
    rclcpp::TimerBase::SharedPtr timer;
    int count = 0;
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