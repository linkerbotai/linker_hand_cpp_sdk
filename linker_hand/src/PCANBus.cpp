#ifdef _WIN32
#include "PCANBus.h"
namespace Communication
{
    PCANBus::PCANBus(TPCANHandle channel, TPCANBaudrate bitrate, const LINKER_HAND linkerhand)
        : channel(channel), linker_hand(linkerhand)
    {
        TPCANStatus status = CAN_Initialize(channel, bitrate);
        if (status != PCAN_ERROR_OK)
            throw std::runtime_error("Failed to initialize PCAN: " + std::to_string(status));

        std::cout << "PCAN initialized successfully!" << std::endl;

        send_count = 0;
        receive_count = 0;
        last_time = std::chrono::steady_clock::now();
        receive_last_time = std::chrono::steady_clock::now();
    }

    PCANBus::~PCANBus()
    {
        CAN_Uninitialize(channel);
        std::cout << "PCAN closed." << std::endl;
    }

    std::string PCANBus::printMillisecondTime()
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

        auto duration = now.time_since_epoch();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration % std::chrono::seconds(1)).count();
        int milliseconds = microseconds / 1000;
        ss << "." << std::setfill('0') << std::setw(3) << milliseconds;

        return ss.str();
    }

    void PCANBus::send(const std::vector<uint8_t> &data, uint32_t can_id, const bool wait)
    {
        std::unique_lock<std::mutex> lock(mutex_send);

        if (SEND_DEBUG)
        {
            static std::string hand_str;
            if (linker_hand == LINKER_HAND::L7) {
                hand_str = "L7";
            } else if (linker_hand == LINKER_HAND::L10) {
                hand_str = "L10";
            } else if (linker_hand == LINKER_HAND::L20) {
                hand_str = "L20";
            } else if (linker_hand == LINKER_HAND::L21) {
                hand_str = "L21";
            } else if (linker_hand == LINKER_HAND::L25) {
                hand_str = "L25";
            }
            std::cout << "\033[1;32m# " << hand_str << "-Send\033[0m " << printMillisecondTime() << " | can_id:" << std::hex << can_id << std::dec << " can_dlc:" << data.size() << " data:";
            for (auto &can : data)
                std::cout << std::hex << (int)can << std::dec << " ";
            std::cout << std::endl;
        }

        TPCANMsg msg;
        msg.ID = can_id;
        msg.MSGTYPE = PCAN_MESSAGE_STANDARD;
        msg.LEN = static_cast<BYTE>(data.size());
        for (int i = 0; i < data.size() && i < 8; ++i)
        {
            msg.DATA[i] = data[i];
        }
        TPCANStatus status = CAN_Write(channel, &msg);
        if (status != PCAN_ERROR_OK)
        {
            throw std::runtime_error("Failed to send CAN frame: " + std::to_string(status));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));

        // updateSendRate();

        #if 0
        // 提前解锁
        lock.unlock();

        if (linker_hand == LINKER_HAND::L10 || linker_hand == LINKER_HAND::L7)
        {
            // 压感
            if (data[0] >= 0xb1 && data[0] <= 0xb6)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(25)); // 获取每个指头的压感数据最少25毫秒等待，完整指令需要125毫秒，新压感的频率最高是8hz
            }
            // // 扭矩
            // if (data[0] == 0x02) {
            //     std::this_thread::sleep_for(std::chrono::milliseconds(2));
            // }
            // // 速度
            // if (data[0] == 0x05) {
            //     std::this_thread::sleep_for(std::chrono::milliseconds(2));
            // }
        }

        if (linker_hand == LINKER_HAND::L21 || linker_hand == LINKER_HAND::L25)
        {
            // std::cout << "L21 or L25" << std::endl;
            // 压感
            if (data[0] >= 0xb1 && data[0] <= 0xb6)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(40));
            }
            // 扭矩
            if (data[0] >= 0x51 && data[0] <= 0x55)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            // 速度
            if (data[0] >= 0x49 && data[0] <= 0x4D)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            // 获取位置
            if (data[0] >= 0x41 && data[0] <= 0x45)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
        }
        #endif
    }

    // CanFrame PCANBus::recv(uint32_t &id)
    // {
    //     TPCANMsg msg;
    //     TPCANTimestamp timestamp;
    //     TPCANStatus status = CAN_Read(channel, &msg, &timestamp);

    //     if (status == PCAN_ERROR_OK)
    //     {
    //         if (msg.MSGTYPE == PCAN_MESSAGE_STANDARD)
    //         {
    //             if (msg.ID == id)
    //             {
    //                 return msg;
    //             }
    //             // updateReceiveRate();
    //         }
    //     }
    //     else if (status != PCAN_ERROR_QRCVEMPTY)
    //     {
    //         throw std::runtime_error("Failed to receive CAN frame: " + std::to_string(status));
    //     }
    //     // return msg;
    //     // 如果接收队列为空，可以选择等待一段时间后再尝试
    //     // std::this_thread::sleep_for(std::chrono::milliseconds(3));

    //     return msg;
    // }

    CANFrame PCANBus::recv(uint32_t &id)
    {
        TPCANMsg msg;
        TPCANTimestamp timestamp;
        TPCANStatus status = CAN_Read(channel, &msg, &timestamp);

        if (status == PCAN_ERROR_OK)
        {
            CANFrame result;
            result.can_id = msg.ID;
            result.can_dlc = msg.LEN;
            for (int i = 0; i < msg.LEN; ++i){
                result.data[i] = msg.DATA[i];
            }
            return result;
        }
        else if (status != PCAN_ERROR_QRCVEMPTY)
        {
            throw std::runtime_error("Failed to receive CAN frame");
        }

        // 如果接收队列为空，返回一个空的 CANFrame
        return CANFrame{};
    }

    void PCANBus::updateSendRate()
    {
        std::lock_guard<std::mutex> lock(send_mutex);
        send_count++;

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_time).count();

        if (elapsed >= 1)
        {
            std::cout << "CAN帧发送速率: " << send_count << " 帧/秒" << std::endl;
            send_count = 0;
            last_time = current_time;
        }
    }

    void PCANBus::updateReceiveRate()
    {
        std::lock_guard<std::mutex> lock(receive_mutex);
        receive_count++;

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - receive_last_time).count();

        if (elapsed >= 1)
        {
            std::cout << "CAN帧接收速率: " << receive_count << " 帧/秒" << std::endl;
            receive_count = 0;
            receive_last_time = current_time;
        }
    }
}
#endif
