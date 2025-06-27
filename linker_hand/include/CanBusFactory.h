#ifndef CAN_BUS_FACTORY_H
#define CAN_BUS_FACTORY_H

#include "ICanBus.h"
#include "CanBus.h"
#include "PCANBus.h"

namespace Communication
{
    class CanBusFactory
    {
    public:
        static std::unique_ptr<ICanBus> createCanBus(
            const std::string& interfaceOrChannel,
            int bitrate,
            const LINKER_HAND linkerHand
        )
        {
            #ifdef _WIN32
                TPCANHandle channel = PCAN_USBBUS1;
                TPCANBaudrate baudrate = PCAN_BAUD_1M;
                if (interfaceOrChannel == "can1") channel = PCAN_USBBUS2;
                return std::make_unique<PCANBus>(channel, baudrate, linkerHand);
            #else
                return std::make_unique<CanBus>(interfaceOrChannel, bitrate, linkerHand);
            #endif
        }
    };
}

#endif // CAN_BUS_FACTORY_H
