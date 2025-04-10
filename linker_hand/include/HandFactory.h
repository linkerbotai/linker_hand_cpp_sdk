#pragma once

#include "IHand.h"

typedef enum {
    L7,
	L10,
    L20,
	L24,
	L25
} LINKER_HAND;

typedef enum {
    RIGHT = 0x27,
    LEFT = 0x28
} HAND_TYPE;

class HandFactory {
public:
    static std::unique_ptr<IHand> createHand(LINKER_HAND type, uint32_t handId, const std::string& canChannel, int baudrate) {
        switch (type) {
            case LINKER_HAND::L10:
                return std::make_unique<LinkerHandL10Can>(handId, canChannel, baudrate);
            case LINKER_HAND::L20:
                return std::make_unique<LinkerHandL10Can>(handId, canChannel, baudrate);
            default:
                throw std::invalid_argument("Unknown hand type");
        }
    }
};
