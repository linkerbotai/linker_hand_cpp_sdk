#ifndef MOD_BUS_H
#define MOD_BUS_H

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
#include <atomic>

#include <unistd.h>
#include <chrono>
#include <thread>
#include <queue>
#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include "rm_service.h"
#include "LinkerHandType.h"
#include "ICommunication.h"

namespace Communication //Communicator
{
    class ModBus : public ICommunication
    {
    public:
        ModBus(uint32_t handId);
        ~ModBus();

        void send(const std::vector<uint8_t>& data, uint32_t &id, const int &start_address = 0, const int &num = 0) override;
        std::vector<uint8_t> recv(uint32_t& id, const int &start_address = 0, const int &num = 0) override;

        bool writeHoldingRegister(const int &id, const int &start_address, const int &num, const std::vector<uint8_t> &data);
        std::vector<uint8_t> readHoldingRegister(const int &id, const int &start_address, const int &num);

    private:
        uint32_t handId;
        int result = -1;
        rm_robot_handle *robot_handle;
        RM_Service robotic_arm;
    };
}

#endif // MOD_BUS_H
