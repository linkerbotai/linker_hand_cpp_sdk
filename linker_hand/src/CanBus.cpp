#include "CanBus.h"
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace AIMOcommunicate
{
    CanBus::CanBus(const std::string& interface, int bitrate)
        : interface(interface), bitrate(bitrate)
    {
    	#if 0
        // 创建 CAN 套接字
        socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (socket_fd < 0)
        {
            throw std::runtime_error("Failed to create CAN socket");
        }

        // 获取接口索引
        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
        if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0)
        {
            throw std::runtime_error("Failed to get CAN interface index");
        }

        // 配置 CAN 地址
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        // 绑定套接字到 CAN 接口
        if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            throw std::runtime_error("Failed to bind CAN socket");
        }

        // 设置 CAN 波特率
        std::string command = "sudo ip link set " + interface + " up type can bitrate " + std::to_string(bitrate);
        
        std::cout << "command : " << command << std::endl;
        
        if (system(command.c_str()) != 0)
        {
            throw std::runtime_error("Failed to set CAN bitrate");
        }
        #endif
        
        socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW); // 打开套接字
        
        struct ifreq ifr = {0};
        strcpy(ifr.ifr_name, "can0");            // 绑定 socket 到 can0 接口
        ioctl(socket_fd, SIOCGIFINDEX, &ifr); // 获取接口索引

        // 设置 CAN 接口的波特率, 替代命令行命令 sudo ip link set can0 up type can bitrate 1000000
        // struct can_bittiming can_bt;
        // can_bt.bitrate = 1000000;
        // struct ifreq ifr_bt = {0};
        // strcpy(ifr_bt.ifr_name, "can0");
        // ifr_bt.ifr_ifru.ifru_data = static_cast<void*>(&can_bt);
        // if (ioctl(socket_fd, SIOCSCANBITTIMING, &ifr_bt) < 0) // 使接口"up"
        // {
        //     std::cerr << "Error setting CAN bitrate: " << strerror(errno) << std::endl;
        //     close(socket_fd);
        //     return false;
        // }
        
        std::string command = "sudo ip link set can0 down"; // 停止
        int result = system(command.c_str());                                      // 使用 system 函数执行命令
        
        command = "sudo ip link set can0 up type can bitrate 1000000"; // 构造命令字符串
        result = system(command.c_str());                                      // 使用 system 函数执行命令
        if (result == 0)
        {
            std::cout << "CAN interface configured successfully." << std::endl;
        }
        else
        {
            std::cerr << "Failed to configure CAN interface." << std::endl;
            
        }

        struct sockaddr_can addr;
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            std::cerr << "Error in socket bind." << std::endl;
            close(socket_fd);
            // return false;
        }
    }

    CanBus::~CanBus()
    {
    	std::string command = "sudo ip link set can0 down"; // 停止CAN接口
        int result = system(command.c_str());
    
        // 关闭 CAN 套接字
        close(socket_fd);
    }
    
    #if 0
	void CanBus::send(const uint8_t send_data[8], const size_t size, uint32_t id) {
        struct can_frame frame;
        frame.can_id = id;
        frame.can_dlc = size;
        memcpy(frame.data, send_data, frame.can_dlc);

        if (write(socket_fd, &frame, sizeof(frame)) != sizeof(frame)) {
            exit(1);
            throw std::runtime_error("Failed to send CAN frame");
        }

        #if 1
        std::cout << "can3_id:" << std::hex << id << " data:";
        for (size_t i = 0; i < frame.can_dlc; ++i) {
            std::cout << std::hex << (int)send_data[i] << " ";
        }
        std::cout << std::endl;
        #endif

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    #endif

    void CanBus::send(const std::vector<uint8_t>& data, uint32_t can_id)
    {
        std::lock_guard<std::mutex> lock(mutex); // 获取锁

        #if 0
        if (data[0] == 0x01 || data[0] == 0x04)
        {
            std::cout << "\033[1;32m# Send\033[0m can_id:" << std::hex << can_id << " can_dlc:" << data.size() << " data:";
            for (auto &can : data)
            {
                std::cout << std::hex << (int)can << " ";
            }
            std::cout << std::endl;
        }
        #endif
        
        struct can_frame frame;
        frame.can_id = can_id; // 左手/右手
        frame.can_dlc = data.size();
        memcpy(frame.data, data.data(), frame.can_dlc); // 指令+数据

        if (write(socket_fd, &frame, sizeof(frame)) != sizeof(frame))
        {
            exit(1);
            throw std::runtime_error("Failed to send CAN frame");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::vector<uint8_t> CanBus::receive(uint32_t& id)
    {
    	#if 1
        struct can_frame frame;
        if (read(socket_fd, &frame, sizeof(frame)) != sizeof(frame))
        {
            throw std::runtime_error("Failed to receive CAN frame");
        }

        id = frame.can_id;
        return std::vector<uint8_t>(frame.data, frame.data + frame.can_dlc);
    	#endif

    }
    
    void CanBus::setReceiveTimeout(int seconds, int microseconds)
    {
        struct timeval timeout;
        timeout.tv_sec = seconds;
        timeout.tv_usec = microseconds;

        if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
        {
            throw std::runtime_error("Failed to set receive timeout");
        }
    }
}
