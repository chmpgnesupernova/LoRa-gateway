#include "message.h"
#include "message_to_json.h"
#include "serial_input.h"
#include <vector>


std::vector<std::string> test_string = {
    "ABC123:hello..........................",
    "BBB333:hi.............................",
    "CCC444:ki.............................",
    "HME998:iamyujinshin...................."

};

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
    char* port_name = (char*)"/dev/ttyUSB0";
    int baud_rate = 9600;

    if (!open_port(serial, port_name, baud_rate)) {
        std::cerr << "[SERIAL] failed to open port" << std::endl;
        return -1;
    }

    // test string을 벡터로 추가, subscriber가 아닐경우 abort
    // counter 가 1000 이상일 경우 break
    int counter = 0;

    while (counter < 1000){

        std::string tmp_string = get_serial_message(serial);

        if (tmp_string.empty()) {
            counter++;
            continue;
        }

        std::string id = checker.slice_id(tmp_string);
        std::string pay = checker.slice_remain(tmp_string);

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
        
    }

    close_port(serial);


    // test string 전부 추가 후 확인용 출력
    for (auto elem : message_vec) {
        std::cout << "[PROCESSOR] ID: " << elem.get_id() << std::endl;
        std::cout << "[PROCESSOR] PAYLOAD: " << elem.get_payload() << std::endl;
        std::cout << "[PROCESSOR] TIME: " << elem.get_time() << std::endl;
    }

    // vector to json, json 파일로 변환
    vector_to_json(message_vec, json_filename);

}