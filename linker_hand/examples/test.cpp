// main.cpp
#include "LinkerHandApi.h"

int main() {

    // 调用API接口
    LinkerHandApi hand(LINKER_HAND::L10, HAND_TYPE::LEFT);

    // 获取版本信息
    std::cout << hand.getVersion() << std::endl;

    hand.setTorque({200,200,200,200,200,200,200,200,200,200});
    hand.setSpeed({200,200,200,200,200,200,200,200,200,200});

    std::vector<uint8_t> torque = hand.getTorque();
    std::cout << "torque size:" << torque.size() << std::endl;
    for (int i = 0; i < torque.size(); i++) {
        std::cout << std::hex << (int)torque[i] << std::dec << std::endl;
    }

    std::vector<uint8_t> speed = hand.getSpeed();
    std::cout << "speed size:" << speed.size() << std::endl;
    for (int i = 0; i < speed.size(); i++) {
        std::cout << std::hex << (int)speed[i] << std::dec << std::endl;
    }

    
    // 握拳
    std::vector<uint8_t> fist_pose = {101, 60, 0, 0, 0, 0, 255, 255, 255, 51};
    hand.fingerMove(fist_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 张开
    std::vector<uint8_t> open_pose = {255, 104, 255, 255, 255, 255, 255, 255, 255, 71};
    hand.fingerMove(open_pose);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    

    return 0;
}