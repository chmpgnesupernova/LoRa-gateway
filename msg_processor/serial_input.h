#include <iostream>
#include <string>
#include "../include/serialib.h"

/*
1. 시리얼 포트 열려있는지 확인하는 함수 (bool)
2. 포트 열려있을 때, 읽은 값을 반환하는 함수 (string)
3. 시리얼 포트 닫는 함수
*/
bool open_port(serialib& _serial, char* _port_name, int _baud_rate){
    if(_serial.openDevice(_port_name, _baud_rate) != 1) {
        std::cerr << "[SERIAL] Error, cannot open port" << std::endl;
        return false;
    } 
    return true;
}

std::string get_serial_message(serialib& _serial){
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    
    int bytes_read = _serial.readString(buffer, '\n', 255, 1000); 
    
    if (bytes_read > 0) {
        return std::string(buffer);
    }
    return "";
}

void close_port(serialib& _serial) {
    _serial.closeDevice();
}
