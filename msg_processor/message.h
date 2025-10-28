#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// checker class
// input으로 받은 메시지 slice (id, remain)
// 이후 허용된 id.txt 읽고 허용된 id일 경우 message에 추가


class ID_checker {
public:
    std::string id;
    std::string remain;

    // slice ID from input
    std::string slice_id(const std::string& _input_string) {
        std::string substr = _input_string.substr(0, 6);
        id = substr;
        return substr;
    }

    //slice remain from input
    std::string slice_remain(const std::string& _input_string){
        std::string substr = _input_string.substr(7, 32);
        remain = substr;
        return substr;
    }

    // find ID from subscriber.txt 
    bool check_id (std::string _subscriber_filename) {
        std::ifstream file(_subscriber_filename);

        if (!file.is_open()){
            std::cerr << "[CHECKER] Error, cannot open the subscriber.txt" << std::endl;
            return false;
        }

        std::string line;

        while(std::getline(file, line)) {

            if (line.find(id) != std::string::npos) {
                std::cout << "[CHECKER] subscriber found" << std::endl;
                return true;
            }
        }

        file.close();
        std::cout << "[CHECKER] not a subscriber, abort the message: " << id << std::endl;
        return false;
    }


};


// message class
// slicer 에서 검사를 마친 string을 message로 저장

class message {
private:
    std::string id;
    std::string payload;
    std::string time;

public:
    // message constructor
    message (std::string _id = "0", std::string _pay ="0", std::string time = "0") 
    : id(_id), payload(_pay), time(time) {
        set_time();
    }

    // set message id
    void set_id(std::string _sliced_id){
        id = _sliced_id;
    }

    // set message payload
    void set_payload(std::string _sliced_remain){
        payload = _sliced_remain;
    }

    // set current time
    void set_time() {
        auto now_chrono = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now_chrono);

        std::tm* local_tm = std::localtime(&now_time_t);
        char buffer[128]; 
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_tm);
    
        time = std::string(buffer);
    }

    std::string get_id() const {
        return id;
    }

    std::string get_payload () const {
        return payload;
    }

    std::string get_time() const {
        return time;
    }

};

#endif // MESSAGE_H