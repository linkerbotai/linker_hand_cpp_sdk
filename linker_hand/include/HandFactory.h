#ifndef HAND_FACTORY_H
#define HAND_FACTORY_H

#include "IHand.h"
#include "LinkerHandL7.h"
#include "LinkerHandL10.h"
#include "LinkerHandL20.h"
#include "LinkerHandL25.h"

typedef enum {
    L7,
	L10,
    L20,
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
            case LINKER_HAND::L7:
                return std::make_unique<LinkerHandL7::LinkerHand>(handId, canChannel, baudrate);
            case LINKER_HAND::L10:
                return std::make_unique<LinkerHandL10::LinkerHand>(handId, canChannel, baudrate);
            case LINKER_HAND::L20:
                return std::make_unique<LinkerHandL20::LinkerHand>(handId, canChannel, baudrate);
            case LINKER_HAND::L25:
                return std::make_unique<LinkerHandL25::LinkerHand>(handId, canChannel, baudrate);
            default:
                throw std::invalid_argument("Unknown hand type");
        }
    }
};

#endif // HAND_FACTORY_H