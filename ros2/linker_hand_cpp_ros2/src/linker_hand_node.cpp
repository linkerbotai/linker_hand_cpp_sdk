#include <cstdio>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <chrono>
#include <future>

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>

#include "LinkerHandApi.h"

using namespace std::chrono_literals;

static std::string can_channel;

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

        // 声明并获取话题参数
        this->declare_parameter<std::string>("VERSION", "sdk-0.0");
        this->declare_parameter<bool>("HAND_EXISTS", false);
        this->declare_parameter<int>("HAND_TYPE", 0);
        this->declare_parameter<bool>("HAND_TOUCH", false);
        this->declare_parameter<std::string>("HAND_JOINTS", "L7");
        this->declare_parameter<int>("HAND_SPEED", 50);
        this->declare_parameter<int>("HAND_EFFORT", 200);
        this->declare_parameter<std::string>("CAN_CHANNEL", "can0");

        this->declare_parameter<std::string>("HAND_SETTING_TOPIC", "/cb_hand_setting_cmd");
        this->declare_parameter<std::string>("HAND_CONTROL_TOPIC", "/cb_hand_control_cmd");
        this->declare_parameter<std::string>("HAND_STATE_TOPIC", "/cb_hand_state");
        this->declare_parameter<std::string>("HAND_TOUCH_TOPIC", "/cb_hand_touch");
        this->declare_parameter<std::string>("HAND_INFO_TOPIC", "/cb_hand_info");
        // 获取话题参数
        this->get_parameter("HAND_SETTING_TOPIC", hand_setting_topic);
        this->get_parameter("HAND_CONTROL_TOPIC", hand_control_topic);
        this->get_parameter("HAND_STATE_TOPIC", hand_state_topic);
        this->get_parameter("HAND_TOUCH_TOPIC", hand_touch_topic);
        this->get_parameter("HAND_INFO_TOPIC", hand_info_topic);

        // LinkerHand
        this->get_parameter("VERSION", version);
        this->get_parameter("HAND_EXISTS", hand_exists); 
        this->get_parameter("HAND_TYPE", hand_type); 
        this->get_parameter("HAND_TOUCH", hand_touch);
        this->get_parameter("HAND_JOINTS", hand_joints);
        this->get_parameter("HAND_SPEED", hand_speed);
        this->get_parameter("HAND_EFFORT", hand_effort);
        this->get_parameter("CAN_CHANNEL", can_channel);

        // std::cout << "LinkerHand SDK version: " << version << std::endl;
        std::cout << (hand_type == 0 ? "LEFT_HAND" : "RIGHT_HAND") << "  HAND_EXISTS:" << hand_exists << "  HAND_JOINTS:" << hand_joints << "  HAND_TOUCH:" << hand_touch << std::endl;

        if (hand_exists != true) return;

        // LinkerHandType mapping
        std::map<std::string, LINKER_HAND> linker_hand_map = {
            {"L7", LINKER_HAND::L7},
            {"L10", LINKER_HAND::L10},
            {"L20", LINKER_HAND::L20},
            {"L21", LINKER_HAND::L21},
            {"L25", LINKER_HAND::L25}
        };

	COMM_TYPE channel;

	(can_channel == "can0") ? channel = COMM_TYPE::COMM_CAN_0 : channel = COMM_TYPE::COMM_CAN_1;

        if (hand_exists) {
            auto it = linker_hand_map.find(hand_joints);
            if (it != linker_hand_map.end()) {
                if (hand_type == 0) {
                    hand_api = std::make_unique<LinkerHandApi>(it->second, HAND_TYPE::LEFT, channel);
                } else if (hand_type == 1) {
                    hand_api = std::make_unique<LinkerHandApi>(it->second, HAND_TYPE::RIGHT, channel);
                } else {
                    std::cout << "Invalid hand_type: " << hand_type << std::endl;
                }
            } else {
                std::cout << "Invalid hand: " << hand_joints << std::endl;
            }
        }

        if (hand_exists) initHand(hand_api, hand_joints);

        auto hand_control_cmd_cb = [this](sensor_msgs::msg::JointState::SharedPtr msg) -> void
        {
            if (hand_exists) controlHand(hand_api, hand_joints, msg);
        };

        // auto hand_control_cmd_arc_cb = [this](sensor_msgs::msg::JointState::SharedPtr msg) -> void
        // {
        //     if (hand_exists) controlHand(hand_api, hand_joints, msg, true);
        // };

        auto hand_setting_cb = [this](std_msgs::msg::String::SharedPtr msg) -> void
        {
            // std::cout << "hand_setting_cb: " << msg->data << std::endl;

            // bool setting_left = false, setting_right = false;
            try {
                nlohmann::json data = nlohmann::json::parse(msg->data);
                RCLCPP_INFO(this->get_logger(), "command:%s", data["setting_cmd"].get<std::string>().c_str());
                RCLCPP_INFO(this->get_logger(), "data:%s", data.dump().c_str());

                // if (data["params"]["hand_type"] == "left" && hand_exists) {
                //     setting_left = true;
                // } else if (data["params"]["hand_type"] == "right" && right_hand_exists) {
                //     setting_right = true;
                // } else {
                //     RCLCPP_ERROR(this->get_logger(), "hand type invalid !");
                //     return;
                // }

                // clearFaultCode 
                if (data["setting_cmd"] == "clear_faults") {
                    hand_api->clearFaultCode();
                }

                // setCurrent 
                if (data["setting_cmd"] == "set_electric_current") {
                    int tmp = data["params"]["electric_current"].get<int>();
                    std::vector<uint8_t> e_c(5, tmp);
                    hand_api->setCurrent(e_c);
                }

                // enable 
                if (data["setting_cmd"] == "enable") {
                    hand_api->setEnable();
                }
                
                // disable 
                if (data["setting_cmd"] == "disable") {
                    hand_api->setDisable();
                }
            } catch (const std::exception &e) {
                RCLCPP_ERROR(this->get_logger(), "Command parameter error : %s", e.what());
            }
        };

        sub_settings = this->create_subscription<std_msgs::msg::String>(hand_setting_topic, 10, hand_setting_cb);
        sub_hand_control = this->create_subscription<sensor_msgs::msg::JointState>(hand_control_topic, 10, hand_control_cmd_cb);
        // sub_hand_control_arc = this->create_subscription<sensor_msgs::msg::JointState>("/cb_hand_control_cmd_arc", 10, hand_control_cmd_arc_cb);

        pub_hand_touch_ = this->create_publisher<std_msgs::msg::Float32MultiArray>(hand_touch_topic, 10);
        pub_hand_info_ = this->create_publisher<std_msgs::msg::String>(hand_info_topic, 10);
        pub_hand_state_ = this->create_publisher<sensor_msgs::msg::JointState>(hand_state_topic, 10);
        // pub_hand_state_arc_ = this->create_publisher<sensor_msgs::msg::JointState>("/cb_hand_state_arc", 10);


        // auto start_time = std::chrono::high_resolution_clock::now();
        // auto end_time = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);

        // start_time = std::chrono::high_resolution_clock::now();

        // end_time = std::chrono::high_resolution_clock::now();
        // duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
        // std::cout << "delay time: " << duration.count() << " seconds" << std::endl;


        if (hand_exists && hand_touch) {
            pub_touch_thread = std::thread([this]() {
                while (rclcpp::ok()) {
                    // rclcpp::spin_some(this);
                    if (pub_hand_touch_->get_subscription_count() > 0) {
                        publishTouchData(hand_api, *this->pub_hand_touch_);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(125));
                }
            });
        }

        pub_state_thread = std::thread([this]() {
            while (rclcpp::ok()) {
                if (pub_hand_state_->get_subscription_count() > 0) {
                    publishJointState(hand_api, *this->pub_hand_state_);
                    // publishJointState(hand_api, *this->pub_hand_state_arc_, true);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        });

        pub_info_thread = std::thread([this]() {
            while (rclcpp::ok()) {
                if (pub_hand_info_->get_subscription_count() > 0) {
                    publishLinkerHandInfo(hand_api, *this->pub_hand_info_);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
    }

    ~LinkerHand()
    {
        // Ensure the release of resources in the destructor
        hand_api.reset();

        pub_touch_thread.join();
        pub_state_thread.join();
        pub_info_thread.join();
    }

    // General function: Publish touch sensor data
    template <typename HandType>
    void publishTouchData(const HandType& hand, rclcpp::Publisher<std_msgs::msg::Float32MultiArray>& publisher) 
    {
        auto message = std_msgs::msg::Float32MultiArray();

        const auto& touch = hand->getForce();

        for (const auto& vec : touch) {
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
        message.data = hand->getVersion() + "Temperature: " + vectorToString(hand->getTemperature()) + "\nFaultCode: " + vectorToString(hand->getFaultCode()) + "\nCurrent: " + vectorToString(hand->getCurrent());
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
        else if ((hand_name == "L25" || hand_name == "L21") && msg->position.size() == 25)
        {
            (msg->velocity.size() == 25) ? speed = convert<double, u_int8_t>(msg->velocity) : speed = std::vector<uint8_t>(25, hand_speed);
            (msg->effort.size() == 25) ? effort = convert<double, u_int8_t>(msg->effort) : effort = std::vector<uint8_t>(25, hand_effort);

            hand->setSpeed(speed); // speed
            hand->setTorque(effort); // torque
            (is_arc) ? hand->fingerMoveArc(msg->position) : hand->fingerMove(convert<double, u_int8_t>(msg->position)); // joint position
        }
        else 
        {
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
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_hand_control;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_hand_control_arc;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_settings;
    // rclcpp::TimerBase::SharedPtr timer_;
    // rclcpp::TimerBase::SharedPtr timer_state_;
    // rclcpp::TimerBase::SharedPtr timer_info_;
    // rclcpp::TimerBase::SharedPtr timer_touch_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_hand_info_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_hand_touch_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_hand_state_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_hand_state_arc_;

    std::unique_ptr<LinkerHandApi> hand_api;

    std::string version;
    bool hand_exists;
    bool hand_touch;
    std::string hand_joints;

    int hand_speed;
    int hand_effort;

    // 创建线程对象
    std::thread pub_touch_thread;
    std::thread pub_state_thread;
    std::thread pub_info_thread;

    std::string hand_setting_topic;
    std::string hand_state_topic;
    std::string hand_state_arc_topic;
    std::string hand_info_topic;
    std::string hand_touch_topic;
    std::string hand_control_topic;
    std::string hand_control_arc_topic;

    int hand_type;
};

// signal
void signalHandler(int signal) {
    if (signal == SIGINT) {
        // stop can channel
        int result = system(std::string("sudo ip link set " + can_channel + " down").c_str());
        if (result == 0) {
            std::cout << ((can_channel == "can0") ? "CAN0" : "CAN1") <<" interface stopped successfully." << std::endl;
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
