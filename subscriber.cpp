#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>


bool is_subscriber(std::string _str, std::vector<std::string>& _sub_list) {
    std::string tmp_id = "";
    if (_str.length() < 3) {
        std::cout << "문자열의 길이가 너무 짧습니다" << std::endl;
        return false;
    }
    tmp_id = _str.substr(0,3);

    for (std::string subscriber : _sub_list) {
        if (subscriber == tmp_id) {
            return true;
        }
    }
    return false; 
}

std::string hand_msg(bool _sub) {
    return "";
}

// curl을 사용한 HTTP POST 방식으로 웹에 메시지 전송
bool send_to_web_curl(const std::string& id, const std::string& message) {
    // JSON에서 특수문자 이스케이프 처리
    std::string escaped_message = message;
    size_t pos = 0;
    while ((pos = escaped_message.find("\"", pos)) != std::string::npos) {
        escaped_message.replace(pos, 1, "\\\"");
        pos += 2;
    }
    // 줄바꿈 문자 처리
    pos = 0;
    while ((pos = escaped_message.find("\n", pos)) != std::string::npos) {
        escaped_message.replace(pos, 1, "\\n");
        pos += 2;
    }
    
    // 현재 타임스탬프 생성
    time_t now = time(nullptr);
    
    // curl 명령어 생성
    std::stringstream ss;
    ss << "curl -X POST http://localhost:5000/api/messages "
       << "-H 'Content-Type: application/json' "
       << "-H 'Accept: application/json' "
       << "-d '{\"id\":\"" << id << "\","
       << "\"message\":\"" << escaped_message << "\","
       << "\"timestamp\":" << now << ","
       << "\"source\":\"lora_gateway\"}' "
       << "--connect-timeout 5 "
       << "--max-time 10 "
       << ">/dev/null 2>&1";
    
    std::cout << "웹 전송 시도: " << id << std::endl;
    
    int result = system(ss.str().c_str());
    return (result == 0);
}

bool send_message_to_web(const std::string& full_message, std::vector<std::string>& subscriber_list) {
    if (is_subscriber(full_message, subscriber_list)) {
        std::string id = full_message.substr(0, 3);  
        
        std::cout << "구독자 ID " << id << " 감지됨. 웹으로 메시지 전송 중..." << std::endl;
        
        if (send_to_web_curl(id, full_message)) {
            std::cout << "HTTP로 메시지 전송 성공!" << std::endl;
            return true;
        } else {
            std::cerr << "웹 전송 실패" << std::endl;
            return false;
        }
    }
    
    std::cout << "비구독자 메시지 - 웹 전송 생략" << std::endl;
    return false;
}