#include "serialib.h"
#include <iostream>
#include <string>



int main(){

    serialib serial;
    const char* device = "/dev/ttyACM0";
    const unsigned int baud = 9600; 

    serial.openDevice(device, baud);

    std::cout << "Device open: " << serial.isDeviceOpen() << std::endl;

    // Use a safe buffer for a single character and check return value

    std::cout << "[test_readChar]" << std::endl;

    char read_c = 0;
    int count = 0;
    int return_val = -1;

    while(count < 10){
        if (serial.isDeviceOpen()) {
            return_val = serial.readChar(&read_c, 10000);
            if (return_val == 1) {
                std::cout << "counter: "<< count << read_c << std::endl;
            } else if (return_val == 0) {
                std::cout << "readChar: timeout" << std::endl;
            } else {
                std::cout << "readChar: error (" << return_val << ")" << std::endl;
            }
        }
        count ++;
    }
    std::cout << "readChar done" << std::endl;

    std::cout << "[test_readString]"<< std::endl;
    char read_s[32];
    count = 0;
    int bytes = -1;

    while (count < 100){
        if (serial.isDeviceOpen()){
            bytes = serial.readString(read_s, '\n', 32, 10000);
            if (bytes > 0) {
                read_s[31] = '\0';
                std::cout << "counter: " << count << ", received string: "<< read_s << std::endl;
            }
        count ++;
        }

    std::cout << "readString done" << std::endl;
    }
}