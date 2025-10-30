#include <iostream>
#include <string>
#include "../include/serialib.h"
#include <chrono> // 시간 관련 기능을 위해 필요
#include <thread> // sleep_for를 위해 필요
/*
1. 시리얼 포트 열려있는지 확인하는 함수 (bool)
2. 포트 열려있을 때, 읽은 값을 반환하는 함수 (string)
3. 시리얼 포트 닫는 함수
*/
// bool open_port(serialib& _serial, const char* _port_name, const int _baud_rate){
//     if(_serial.openDevice(_port_name, _baud_rate) != 1) {
//         std::cerr << "[SERIAL] Error, cannot open port" << std::endl;
//         return false;
//     } 
//     return true;
// }

void get_serial_message(serialib& _serial, std::string& _received_message){
    char final_char = '\n';
    int max_bytes = 32;
    int time_out_ms = 100;

    char* received_message;

    _serial.readString(received_message, final_char, max_bytes, time_out_ms);

    std::string return_string(received_message);
    _received_message = return_string;
    std::cout << _received_message << std::endl;
}

void close_port(serialib& _serial) {
    _serial.closeDevice();
}