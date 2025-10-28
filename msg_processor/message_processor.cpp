#include "message.h"
#include "message_to_json.h"
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

    ID_checker checker;

    std::string json_filename = "../data/latest_messages.json";
    std::string subscriber_filename = "../data/subscriber.txt";


    // test string을 벡터로 추가, subscriber가 아닐경우 abort
    for (auto elem : test_string){

        std::string id = checker.slice_id(elem);
        std::string pay = checker.slice_remain(elem);

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
    }

    // test string 전부 추가 후 확인용 출력
    for (auto elem : message_vec) {
        std::cout << "[PROCESSOR] ID: " << elem.get_id() << std::endl;
        std::cout << "[PROCESSOR] PAYLOAD: " << elem.get_payload() << std::endl;
        std::cout << "[PROCESSOR] TIME: " << elem.get_time() << std::endl;
    }

    // vector to json, json 파일로 변환
    vector_to_json(message_vec, json_filename);

}