#include "message.h"
#include "message_to_json.h"
#include "serial_input.h"
#include <vector>


int find_index_of_id(std::string& _id, std::vector<message>& _vec){
    int ret = -1;

    for (int i = 0; i < _vec.size(); i++) {
        if (_vec[i].get_id() == _id) ret = i;
    }

    return ret;
}

int main(){

    // message vector
    std::vector<message> message_vec;

    // ID checker 인스턴스 생성
    ID_checker checker;

    // file 경로 설정
    std::string json_filename = "../data/latest_messages.json";
    std::string subscriber_filename = "../data/subscriber.txt";

    //port 설정
    serialib serial;
    const char* port_name = "/dev/ttyACM0";
    const unsigned int baud_rate = 9600;

    serial.openDevice(port_name, baud_rate);

    // serial을 위한 변수들
    int counter = 0;
    char received_string[32];
    int message_bytes = -1;

    //while (counter < 100){
    while ( true ) {

        if (serial.isDeviceOpen()) {
            message_bytes = serial.readString(received_string, '\n', 32, 1000);
            if (message_bytes > 0) {
                received_string[message_bytes] = '\0';
                std::cout << "[SERIAL] counter " << counter 
                << ", received string: " << received_string << std::endl;
            }
        } else {
            std::cout << "[SERIAL] device is closed, exit" << std::endl;
            break;
        }

        std::string tmp_string(received_string);
        std::string id = checker.slice_id(tmp_string);
        std::string pay = checker.slice_remain(tmp_string);

        // test string을 벡터로 추가, subscriber가 아닐경우 abort
        bool is_subscriber = checker.check_id(subscriber_filename);

        if (is_subscriber) {

            int index = find_index_of_id(id, message_vec);

            if (index == -1) {
                message_vec.push_back(message(id,pay));
            }
            else {
                message_vec[index].set_payload(pay);
                message_vec[index].set_time();
            }
        }

        counter ++;
        vector_to_json(message_vec, json_filename);
        std::cout << "==================================================" << std::endl;
        
    }

    close_port(serial);


    // test string 전부 추가 후 확인용 출력
    for (auto elem : message_vec) {
        std::cout << "[PROCESSOR] ID: " << elem.get_id() << std::endl;
        std::cout << "[PROCESSOR] PAYLOAD: " << elem.get_payload() << std::endl;
        std::cout << "[PROCESSOR] TIME: " << elem.get_time() << std::endl;
    }

    // vector to json, json 파일로 변환

}