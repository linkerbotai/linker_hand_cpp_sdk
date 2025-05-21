// main.cpp
#include "LinkerHandApi.h"

int main() {
    
    // 调用API接口
    LinkerHandApi hand(LINKER_HAND::L10, HAND_TYPE::LEFT, COMM_TYPE::COMM_MODBUS);

	std::vector<uint8_t> data;

	data = std::vector<uint8_t>(5, 100);
	hand.setSpeed(data);

	data = std::vector<uint8_t>(10, 200);
	hand.setTorque(data);


	for (int i = 0; i < 10; i++) {
	    if (i % 2 == 0) {
		    data = std::vector<uint8_t>(10, 0);
	    } else {
		    data = std::vector<uint8_t>(10, 255);
	    }

		hand.fingerMove(data);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	
    return 0;
}