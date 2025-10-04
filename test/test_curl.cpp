#include "../subscriber.cpp"
#include <iostream>
#include <vector>
#include <unistd.h>

int main() {
    // 구독자 리스트 생성
    std::vector<std::string> subscriber_list = {"001", "002", "ABC", "XYZ"};
    
    std::cout << "=== LoRa Gateway 웹 전송 테스트 ===" << std::endl;
    
    // 테스트 메시지들
    std::vector<std::string> test_messages = {
        "001:온도=25.3도,습도=60%",
        "002:압력=1013hPa",
        "999:비구독자 메시지",
        "ABC:상태=정상",
        "DEF:미등록 ID"
    };
    
    for (const auto& message : test_messages) {
        std::cout << "\n--- 테스트 메시지: " << message << " ---" << std::endl;
        bool result = send_message_to_web(message, subscriber_list);
        std::cout << "결과: " << (result ? "성공" : "실패/미전송") << std::endl;
        
        // 잠시 대기
        sleep(1);
    }
    
    std::cout << "\n=== 테스트 완료 ===" << std::endl;
    std::cout << "받은 메시지 확인: /tmp/lora_received_messages.json" << std::endl;
    
    return 0;
}