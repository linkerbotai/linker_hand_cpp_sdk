#include <cstdio>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include "LinkerHandApi.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"

using namespace std::chrono_literals;

std::string bytesToHex(const std::vector<uint8_t> &bytes)
{
    std::string hexStr;
    for (uint8_t byte : bytes)
    {
        hexStr += std::to_string(byte) + " ";
    }
    return hexStr;
}

std::string bytesToHex(const std::vector<std::vector<uint8_t>> &bytes)
{
    std::string hexStr;
    for (const auto &vec : bytes)
    {
        for (uint8_t byte : vec)
        {
            hexStr += std::to_string(byte) + " ";
        }
        hexStr += "\n";
    }
    return hexStr;
}

class LinkerHand : public rclcpp::Node
{
public:
    LinkerHand() : Node("linker_hand_cpp_ros2")
    {
        this->declare_parameter<std::string>("VERSION", "sdk-0.0");
        this->get_parameter("VERSION", version);

        // Left hand configuration
        this->declare_parameter<bool>("LEFT_HAND_EXISTS", false);
        this->get_parameter("LEFT_HAND_EXISTS", left_hand_exists); 
        
        this->declare_parameter<bool>("LEFT_TOUCH", false);
        this->get_parameter("LEFT_TOUCH", left_touch);

        this->declare_parameter<std::string>("LEFT_JOINT", "L7");
        this->get_parameter("LEFT_JOINT", left_joint);

        // Right hand configuration
        this->declare_parameter<bool>("RIGHT_HAND_EXISTS", false);
        this->get_parameter("RIGHT_HAND_EXISTS", right_hand_exists);

        this->declare_parameter<bool>("RIGHT_TOUCH", false);
        this->get_parameter("RIGHT_TOUCH", right_touch);

        this->declare_parameter<std::string>("RIGHT_JOINT", "L20");
        this->get_parameter("RIGHT_JOINT", right_joint);

        // General configuration
        this->declare_parameter<int>("HAND_SPEED", 50);
        this->get_parameter("HAND_SPEED", hand_speed);
        this->declare_parameter<int>("HAND_EFFORT", 200);
        this->get_parameter("HAND_EFFORT", hand_effort);


        std::cout << "----------------------------------------------------" << std::endl;
        // std::cout << "LinkerHand SDK version: " << version << std::endl;
        std::cout << "LEFT_HAND_EXISTS:" << left_hand_exists << "  LEFT_JOINT:" << left_joint << "  LEFT_TOUCH:" << left_touch << std::endl;
        std::cout << "RIGHT_HAND_EXISTS:" << right_hand_exists << "  RIGHT_JOINT:" << right_joint << "  RIGHT_TOUCH:" << right_touch << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;

        // LinkerHandType mapping
        std::map<std::string, LINKER_HAND> linker_hand_map = {
            {"L7", LINKER_HAND::L7},
            {"L10", LINKER_HAND::L10},
            {"L20", LINKER_HAND::L20},
            {"L25", LINKER_HAND::L25}
        };

        // Verify left hand
        if (left_hand_exists)
        {
            auto it = linker_hand_map.find(left_joint);
            if (it != linker_hand_map.end())
            {
                hand_left = std::make_unique<LinkerHandApi>(it->second, HAND_TYPE::LEFT);
            }
            else
            {
                std::cout << "Invalid left hand: " << left_joint << std::endl;
            }
        }

        // Verify right hand
        if (right_hand_exists)
        {
            auto it = linker_hand_map.find(right_joint);
            if (it != linker_hand_map.end())
            {
                hand_right = std::make_unique<LinkerHandApi>(it->second, HAND_TYPE::RIGHT);
            }
            else
            {
                std::cout << "Invalid right hand: " << right_joint << std::endl;
            }
        }

        if (left_hand_exists != true && right_hand_exists != true) return;
        

        if (left_hand_exists) initHand(hand_left, left_joint);
        if (right_hand_exists) initHand(hand_right, right_joint);

        auto left_hand_control_cmd_cb = [this](sensor_msgs::msg::JointState::SharedPtr msg) -> void
        {
            if (left_hand_exists) controlHand(hand_left, left_joint, msg);
        };
        auto right_hand_control_cmd_cb = [this](sensor_msgs::msg::JointState::SharedPtr msg) -> void
        {
            if (right_hand_exists) controlHand(hand_right, right_joint, msg);
        };

        auto left_hand_control_cmd_arc_cb = [this](sensor_msgs::msg::JointState::SharedPtr msg) -> void
        {
            if (left_hand_exists) controlHand(hand_left, left_joint, msg, true);
        };
        auto right_hand_control_cmd_arc_cb = [this](sensor_msgs::msg::JointState::SharedPtr msg) -> void
        {
            if (right_hand_exists) controlHand(hand_right, right_joint, msg, true);
        };

	    // Subscribe to Left and Right Hand Control Information
        sub_left_control = this->create_subscription<sensor_msgs::msg::JointState>("/cb_left_hand_control_cmd", 10, left_hand_control_cmd_cb);
        sub_right_control = this->create_subscription<sensor_msgs::msg::JointState>("/cb_right_hand_control_cmd", 10, right_hand_control_cmd_cb);

        sub_left_control_arc = this->create_subscription<sensor_msgs::msg::JointState>("/cb_left_hand_control_cmd_arc", 10, left_hand_control_cmd_arc_cb);
        sub_right_control_arc = this->create_subscription<sensor_msgs::msg::JointState>("/cb_right_hand_control_cmd_arc", 10, right_hand_control_cmd_arc_cb);

        // Publish Left and Right Hand State Information
        pub_left_state_ = this->create_publisher<sensor_msgs::msg::JointState>("/cb_left_hand_state", 10);
        pub_right_state_ = this->create_publisher<sensor_msgs::msg::JointState>("/cb_right_hand_state", 10);
        pub_left_state_arc_ = this->create_publisher<sensor_msgs::msg::JointState>("/cb_left_hand_state_arc", 10);
        pub_right_state_arc_ = this->create_publisher<sensor_msgs::msg::JointState>("/cb_right_hand_state_arc", 10);

        // Publish Left and Right Hand Force Information
        pub_left_force_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/cb_left_hand_force", 10);
        pub_right_force_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/cb_right_hand_force", 10);
        // Publish Left and Right Hand Info
        pub_left_info_ = this->create_publisher<std_msgs::msg::String>("/cb_left_hand_info", 10);
        pub_right_info_ = this->create_publisher<std_msgs::msg::String>("/cb_right_hand_info", 10);

        timer_ = this->create_wall_timer(100ms, [this]() -> void
            {
                // ======================== publisher ==============================
                if (left_hand_exists)
                {
                    if (left_touch)
                    {
                        publishForceData(hand_left, *this->pub_left_force_); // public force sensor data
                    }
                    publishJointState(hand_left, *this->pub_left_state_); // public joint state
                    publishJointState(hand_left, *this->pub_left_state_arc_, true); // public joint state

                    publishLinkerHandInfo(hand_left, *this->pub_left_info_); // public linker hand info
                }
                if (right_hand_exists)
                {
                    if (right_touch)
                    {
                        publishForceData(hand_right, *this->pub_right_force_); // public force sensor data
                    }
                    publishJointState(hand_right, *this->pub_right_state_); // public joint state
                    publishJointState(hand_right, *this->pub_right_state_arc_, true); // public joint state

                    publishLinkerHandInfo(hand_right, *this->pub_right_info_); // public linker hand info
                }
                // ===============================================================

            });
    }

    ~LinkerHand()
    {
        // Ensure the release of resources in the destructor
        hand_left.reset();
        hand_right.reset();
    }

    // General function: Publish force sensor data
    template <typename HandType>
    void publishForceData(const HandType& hand, rclcpp::Publisher<std_msgs::msg::Float32MultiArray>& publisher) 
    {
        auto message = std_msgs::msg::Float32MultiArray();

        const auto& force = hand->getForce();

        for (const auto& vec : force) {
            for (uint8_t byte : vec) {
                message.data.push_back(static_cast<float>(byte));
            }
        }
        publisher.publish(message);
    }

    // General function: public joint state
    template <typename HandType>
    void publishJointState(const HandType& hand, rclcpp::Publisher<sensor_msgs::msg::JointState>& publisher, const bool is_arc = false)
    {
        auto message = sensor_msgs::msg::JointState();

        (is_arc) ? message.position = hand->getStateArc() : message.position = convert<uint8_t, double>(hand->getState());
        message.velocity = convert<uint8_t, double>(hand->getSpeed());
        message.effort = convert<uint8_t, double>(hand->getTorque());

        publisher.publish(message);
    }

    // General function: publish linker hand info
    template <typename HandType>
    void publishLinkerHandInfo(const HandType& hand, rclcpp::Publisher<std_msgs::msg::String>& publisher)
    {
        auto message = std_msgs::msg::String();
        message.data = hand->getVersion() + "温度：" + vectorToString(hand->getTemperature()) + "\n故障码：" + vectorToString(hand->getFaultCode()) + "\n电流：" + vectorToString(hand->getCurrent());
        publisher.publish(message);
    }

    // General function: Convert vector
    template <typename T, typename U>
    std::vector<U> convert(const std::vector<T>& vec) {
        std::vector<U> result;
        for (const auto& value : vec) {
            result.push_back(static_cast<U>(value));
        }
        return result;
    }

    // General function: subscribe Hand Control Information
    template <typename HandType>
    void controlHand(const HandType& hand, const std::string& hand_name, const sensor_msgs::msg::JointState::SharedPtr msg, const bool is_arc = false)
    {
        // for (auto &p : msg->position)
        // {
        //     std::cout << p << " ";
        // }
        // std::cout << std::endl;

        // init hand speed and effort
        std::vector<uint8_t> speed(5, hand_speed);
        std::vector<uint8_t> effort(5, hand_effort);

        if (hand_name == "L7" && msg->position.size() == 7)
        {
            (msg->velocity.size() == 7) ? speed = convert<double, u_int8_t>(msg->velocity) : speed = std::vector<uint8_t>(7, hand_speed);
            (msg->effort.size() == 7) ? effort = convert<double, u_int8_t>(msg->effort) : effort = std::vector<uint8_t>(7, hand_effort);

            hand->setSpeed(speed); // speed
            hand->setTorque(effort); // torque
            (is_arc) ? hand->fingerMoveArc(msg->position) : hand->fingerMove(convert<double, u_int8_t>(msg->position)); // joint position
        }
        else if (hand_name == "L10" && msg->position.size() == 10)
        {
            if (msg->velocity.size() == 5) speed = convert<double, u_int8_t>(msg->velocity);

            hand->setSpeed(speed); // speed
            hand->setTorque(effort); // torque
            (is_arc) ? hand->fingerMoveArc(msg->position) : hand->fingerMove(convert<double, u_int8_t>(msg->position)); // joint position
        }
        else if (hand_name == "L20" && msg->position.size() == 20)
        {
            if (msg->velocity.size() == 5) speed = convert<double, u_int8_t>(msg->velocity);

            hand->setSpeed(speed); // speed
            // hand->setTorque({100, 100, 100, 100, 100}); // torque
            (is_arc) ? hand->fingerMoveArc(msg->position) : hand->fingerMove(convert<double, u_int8_t>(msg->position)); // joint position
        }
        else if (hand_name == "L25" && msg->position.size() == 25)
        {
            (msg->velocity.size() == 25) ? speed = convert<double, u_int8_t>(msg->velocity) : speed = std::vector<uint8_t>(25, hand_speed);
            (msg->effort.size() == 25) ? effort = convert<double, u_int8_t>(msg->effort) : effort = std::vector<uint8_t>(25, hand_effort);

            hand->setSpeed(speed); // speed
            hand->setTorque(effort); // torque
            (is_arc) ? hand->fingerMoveArc(msg->position) : hand->fingerMove(convert<double, u_int8_t>(msg->position)); // joint position
        }
        else {
            std::cout << hand_name << " Invalid joint number: " << msg->position.size() << std::endl;
            return;
        }
    }

    // std::vector<unsigned char> to std::string
    std::string vectorToString(const std::vector<unsigned char>& vec) {
        std::ostringstream oss;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) {
                oss << " ";
            }
            oss << static_cast<int>(vec[i]);
        }
        return oss.str();
    }

    template <typename HandType>
    void initHand(const HandType& hand, const std::string& hand_name)
    {
        if (hand_name == "L10")
        {
            hand->setSpeed(std::vector<uint8_t>(5, hand_speed)); // speed
            hand->setTorque(std::vector<uint8_t>(5, hand_effort)); // torque
            hand->fingerMove({255, 128, 255, 255, 255, 255, 128, 128, 128, 128}); // joint position
        }
    }

private:
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_left_control, sub_left_control_arc;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_right_control, sub_right_control_arc;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_left_info_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_right_info_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_left_force_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_right_force_;
    rclcpp::TimerBase::SharedPtr timer_state_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_left_state_, pub_left_state_arc_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_right_state_, pub_right_state_arc_;

    std::unique_ptr<LinkerHandApi> hand_left;
    std::unique_ptr<LinkerHandApi> hand_right;

    std::string version;
    bool left_hand_exists;
    bool left_touch;
    std::string left_joint;

    bool right_hand_exists;
    bool right_touch;
    std::string right_joint;

    int hand_speed;
    int hand_effort;
};

// signal
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::string command = "sudo ip link set can0 down"; // stop can0 interface
        int result = system(command.c_str());
        if (result == 0) {
            std::cout << "CAN interface stopped successfully." << std::endl;
        }

        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ctrl+C detected. Shutting down...");
        rclcpp::shutdown();
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    // register signal
    std::signal(SIGINT, signalHandler);

    auto node = std::make_shared<LinkerHand>();

    rclcpp::spin(node);
    return 0;
}
