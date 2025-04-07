#pragma once

#include "IHand.h"

enum class HandType {
    L10,
    L20
};

class HandFactory {
public:
    static std::unique_ptr<IHand> createHand(HandType type, uint32_t canId, const std::string& canChannel, int baudrate) {
        switch (type) {
            case HandType::L10:
                return std::make_unique<LinkerHandL10Can>(canId, canChannel, baudrate);
            case HandType::L20:
                return std::make_unique<LinkerHandL10Can>(canId, canChannel, baudrate);
            default:
                throw std::invalid_argument("Unknown hand type");
        }
    }
};
