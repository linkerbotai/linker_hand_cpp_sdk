#ifndef CAN_BUS_H
#define CAN_BUS_H

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>  // 包含 ifreq 和 IFNAMSIZ
#include <sys/ioctl.h>  // 包含 ioctl
#include <mutex>

namespace AIMOcommunicate
{
    class CanBus
    {
    public:
        CanBus(const std::string& interface, int bitrate);
        ~CanBus();
        
        void send(const std::vector<uint8_t>& data, uint32_t can_id);
        // void send(const uint8_t send_data[8], const size_t size, uint32_t id);
        std::vector<uint8_t> receive(uint32_t& id);
        
        void setReceiveTimeout(int seconds, int microseconds);

    private:
        int socket_fd;
        std::string interface;
        int bitrate;
        struct sockaddr_can addr;
        struct ifreq ifr;  // 确保 ifreq 是完整的类型
        std::mutex mutex; // 互斥锁
    };
}

#endif // CAN_BUS_H
