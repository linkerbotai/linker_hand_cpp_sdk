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
                show_count_obj = 50;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 1:
                show_step++;
                show_count_obj = 10;
                position = {250, 250, 250, 0, 0, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 0, 0};
                break;
            case 2:
                show_step++;
                show_count_obj = 30;
                position = {100, 250, 250, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 3:
                show_step++;
                show_count_obj = 10;
                position = {100, 250, 250, 0, 0, 180, 200, 200, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 4:
                show_step++;
                show_count_obj = 10;
                position = {100, 250, 250, 0, 0, 180, 50, 50, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 5:
                show_step++;
                show_count_obj = 15;
                position = {100, 250, 250, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 6:
                show_step++;
                show_count_obj = 10;
                position = {100, 250, 250, 0, 0, 180, 58, 200, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 7:
                show_step++;
                show_count_obj = 10;
                position = {100, 250, 250, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 8:
                show_step++;
                show_count_obj = 10;
                position = {100, 250, 250, 0, 0, 180, 58, 200, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 9:
                show_step++;
                show_count_obj = 10;
                position = {100, 250, 250, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 10:
                show_step++;
                show_count_obj = 15;
                position = {100, 100, 100, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 100, 100, 0, 0};
                break;
            case 11:
                show_step++;
                show_count_obj = 15;
                position = {100, 250, 250, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 12:
                show_step++;
                show_count_obj = 15;
                position = {100, 100, 100, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 100, 100, 0, 0};
                break;
            case 13:
                show_step++;
                show_count_obj = 15;
                position = {100, 250, 250, 0, 0, 180, 128, 128, 128, 128, 200, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 14:
                show_step++;
                show_count_obj = 40;
                position = {250, 250, 250, 0, 0, 150, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 15:
                show_step++;
                show_count_obj = 10;
                position = {250, 250, 250, 0, 0, 5, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 250, 250, 0, 0};
                break;
            case 16:
                show_step++;
                show_count_obj = 30;
                position = {250, 100, 100, 100, 100, 5, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 10, 10, 10, 10};
                break;
            case 17:
                show_step++;
                show_count_obj = 15;
                position = {250, 100, 100, 100, 250, 5, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 10, 10, 10, 250};
                break;
            case 18:
                show_step++;
                show_count_obj = 15;
                position = {250, 100, 100, 250, 250, 5, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 10, 10, 250, 250};
                break;
            case 19:
                show_step++;
                show_count_obj = 15;
                position = {250, 100, 250, 250, 250, 5, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 10, 250, 250, 250};
                break;
            case 20:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 250, 250, 250, 5, 128, 128, 128, 128, 250, 0, 0, 0, 0, 0, 250, 250, 250, 250};
                break;
            case 21:
                show_step++;
                show_count_obj = 10;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 22:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 10, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 23:
                show_step++;
                show_count_obj = 40;
                position = {0, 250, 250, 250, 250, 250, 128, 128, 128, 128, 10, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 24:
                show_step++;
                show_count_obj = 30;
                position = {0, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 25:
                show_step++;
                show_count_obj = 50;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 26:
                show_step++;
                show_count_obj = 10;
                position = {250, 250, 250, 250, 250, 250, 200, 200, 200, 200, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 27:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 250, 250, 250, 250, 80, 80, 80, 80, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 28:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 29:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 250, 250, 250};
                break;
            case 30:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 250, 250};
                break;
            case 31:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 250};
                break;
            case 32:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 33:
                show_step++;
                show_count_obj = 15;
                position = {250, 0, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 34:
                show_step++;
                show_count_obj = 15;
                position = {250, 0, 0, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 35:
                show_step++;
                show_count_obj = 15;
                position = {250, 0, 0, 0, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 36:
                show_step++;
                show_count_obj = 15;
                position = {250, 0, 0, 0, 0, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 37:
                show_step++;
                show_count_obj = 40;
                position = {0, 0, 0, 0, 0, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 200, 0, 0, 0, 0};
                break;
            case 38:
                show_step++;
                show_count_obj = 40;
                position = {250, 0, 0, 0, 0, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0};
                break;
            case 39:
                show_step++;
                show_count_obj = 30;
                position = {250, 250, 0, 0, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 0, 0, 250};
                break;
            case 40:
                show_step++;
                show_count_obj = 40;
                position = {250, 250, 0, 0, 250, 200, 128, 128, 128, 128, 100, 0, 0, 0, 0, 100, 250, 0, 0, 250};
                break;
            case 41:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 0, 0, 250, 200, 80, 128, 128, 200, 100, 0, 0, 0, 0, 100, 250, 0, 0, 250};
                break;
            case 42:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 0, 0, 250, 200, 200, 128, 128, 80, 100, 0, 0, 0, 0, 100, 250, 0, 0, 250};
                break;
            case 43:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 0, 0, 250, 200, 80, 128, 128, 200, 100, 0, 0, 0, 0, 100, 250, 0, 0, 250};
                break;
            case 44:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 0, 0, 250, 200, 200, 128, 128, 80, 100, 0, 0, 0, 0, 100, 250, 0, 0, 250};
                break;
            case 45:
                show_step++;
                show_count_obj = 15;
                position = {250, 250, 0, 0, 250, 200, 128, 128, 128, 128, 100, 0, 0, 0, 0, 100, 250, 0, 0, 250};
                break;
            case 46:
                show_step++;
                show_count_obj = 50;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 47:
                show_step++;
                show_count_obj = 50;
                position = {55, 0, 250, 250, 250, 170, 128, 128, 128, 128, 70, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 48:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 20, 250, 250, 220, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 49:
                show_step++;
                show_count_obj = 35;
                position = {55, 250, 0, 250, 250, 140, 128, 128, 128, 128, 60, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 50:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 20, 250, 170, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 51:
                show_step++;
                show_count_obj = 35;
                position = {55, 250, 250, 0, 250, 110, 128, 128, 128, 128, 50, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 52:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 20, 130, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 53:
                show_step++;
                show_count_obj = 40;
                position = {55, 250, 250, 250, 0, 60, 128, 128, 128, 128, 50, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 54:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 20, 130, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            case 55:
                show_step++;
                show_count_obj = 40;
                position = {160, 250, 250, 250, 160, 60, 128, 128, 128, 128, 50, 0, 0, 0, 0, 100, 250, 250, 250, 80};
                break;
            case 56:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 250, 130, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 250, 250, 50, 250};
                break;
            case 57:
                show_step++;
                show_count_obj = 35;
                position = {160, 250, 250, 150, 250, 100, 128, 128, 128, 128, 50, 0, 0, 0, 0, 100, 250, 250, 80, 250};
                break;
            case 58:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 250, 180, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 50, 250, 250, 250};
                break;
            case 59:
                show_step++;
                show_count_obj = 35;
                position = {160, 250, 150, 250, 250, 135, 128, 128, 128, 128, 70, 0, 0, 0, 0, 100, 250, 85, 250, 250};
                break;
            case 60:
                show_step++;
                show_count_obj = 20;
                position = {250, 250, 250, 250, 250, 220, 128, 128, 128, 128, 100, 0, 0, 0, 0, 250, 50, 250, 250, 250};
                break;
            case 61:
                show_step++;
                show_count_obj = 35;
                position = {165, 150, 250, 250, 250, 170, 128, 128, 128, 128, 70, 0, 0, 0, 0, 100, 80, 250, 250, 250};
                break;
            case 62:
                show_step++;
                show_count_obj = 60;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
                break;
            default:
                show_step = 0;
                position = {250, 250, 250, 250, 250, 250, 128, 128, 128, 128, 250, 0, 0, 0, 0, 250, 250, 250, 250, 250};
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