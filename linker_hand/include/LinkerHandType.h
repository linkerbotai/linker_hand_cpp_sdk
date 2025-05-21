#ifndef LINKER_HAND_TYPES_H
#define LINKER_HAND_TYPES_H

enum LINKER_HAND {
    L7,
    L10,
    L20,
    L21,
    L25
};

enum HAND_TYPE {
    LEFT = 0x28,
    RIGHT = 0x27
};

enum COMM_TYPE {
    COMM_CAN_0,
    COMM_CAN_1,
    COMM_MODBUS
};

#endif // LINKER_HAND_TYPES_H