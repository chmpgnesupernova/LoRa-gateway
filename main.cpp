#include "serial.cpp"
#include "subscriber.cpp"
#include <unistd.h>
#include <iostream>


int main(){
    std::vector<std::string> subscriber_list = {
        "ad1", "ad2", "ad3"
    };

    int port = setup_port();
    std::string command = "";
    std::cin >> command;

    while (port != -1 && command == "start") {
        static int counter = 0;
        counter++;
        if (counter > 10000) {
            std::cout << "최대 횟수 1000회 도달, 종료" << std::endl;
            break;
        }

        std::string serial_data = read_serial(port);
        
        if (!serial_data.empty()) {
            std::cout << "시리얼 메시지 수신: " << serial_data << std::endl;
            
            if (send_message_to_web(serial_data, subscriber_list)) {
                std::cout << "웹 전송 완료" << std::endl;
            } else {
                std::cout << "웹 전송 실패 또는 비구독자" << std::endl;
            }
        } else {
            std::cout << "시리얼 데이터 없음 (루프 " << counter << ")" << std::endl;
        }
        
        usleep(100000); // 0.1초
        
        if (counter % 100 == 0) {
            std::cout << "계속하시겠습니까? 'q' 입력 시 종료: ";
            std::string user_input;
            std::getline(std::cin, user_input);
            if (user_input == "q" || user_input == "quit") {
                std::cout << "사용자가 종료를 요청했습니다." << std::endl;
                break;
            }
        }
    }

    if (port != -1) {
        close(port);
        std::cout << "시리얼 포트를 닫았습니다." << std::endl;
    }
    
    std::cout << "프로그램이 종료되었습니다." << std::endl;
    return 0;
}