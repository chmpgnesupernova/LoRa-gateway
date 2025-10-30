#ifndef MESSAGE_TO_JSON_H
#define MESSAGE_TO_JSON_H

#include "message.h"
#include "../include/json.hpp"

using json = nlohmann::json;

void to_json(json& _json, const message& _message) {

    std::string tmp_id = _message.get_id();
    std::string tmp_payload = _message.get_payload();
    std::string tmp_time = _message.get_time();

    _json = json{
        {"ID", tmp_id},
        {"payload", tmp_payload},
        {"time", tmp_time}
    };
}

void vector_to_json(const std::vector<message>& _message_vec, const std::string& _filename){
    json json_vec = _message_vec;

    try {
        std::ofstream file(_filename);
        if (!file.is_open()){
            std::cerr << "[PROCESSOR] Error, cannot open json file" << std::endl;
            return;
        }

        file << json_vec.dump(4);
        std::cout << "[PROCESSOR] successfully saved, vector has " << _message_vec.size() << "messages in" << _filename << std::endl;
    }

    catch (const std::exception& exeption) {
        std::cerr << "[PROCESSOR] Error, cannot saved json file, " << exeption.what() << std::endl; 
    }



}

#endif