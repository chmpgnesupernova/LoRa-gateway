#ifndef SERIAL
#define SERIAL

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <filesystem>

namespace fs = std::filesystem;

int setup_port() {

    std::cout << "포트 경로를 입력하세요." << std::endl;
    std::string port_path = "";
    std::cin >> port_path;

    bool isExists = fs::exists(fs::status(port_path));

    std::string port_string = (isExists) ? port_path : "" ;
    
    if (port_string.empty()) { 
        std::cerr << "유효한 포트가 아닙니다." << std::endl;
        return -1; 
    }
    
    const char* port_name = port_string.c_str();

    int serial_port = open(port_name, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "포트 열기 실패 " << port_name << std::endl;
        return -1;
    }

    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "tty 설정 실패" << std::endl;
        return -1;
    }

    // 시리얼 포트 속성 설정
    tty.c_cflag &= ~PARENB;      // 패리티 비트 비활성화
    tty.c_cflag &= ~CSTOPB;      // 정지 비트 1개
    tty.c_cflag |= CS8;          // 8비트 데이터
    tty.c_cflag &= ~CRTSCTS;     // 흐름 제어 비활성화
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;      // 캐노니컬 모드 비활성화
    tty.c_lflag &= ~ECHO;        // 에코 비활성화
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST;       // 출력 처리 비활성화
    tty.c_oflag &= ~ONLCR;       // 줄바꿈 문자 변환 비활성화

    tty.c_cc[VTIME] = 5; // 0.5초 읽기 타임아웃
    tty.c_cc[VMIN] = 0;  // 최소 0 바이트라도 읽기

    cfsetispeed(&tty, B9600);    // 입력 속도 설정
    cfsetospeed(&tty, B9600);    // 출력 속도 설정

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "tty 설정 적용 실패" << std::endl;
        return -1;
    }

    std::cout << "포트가 성공적으로 열렸습니다!" << std::endl;
    std::cout << "Port: " << port_name << std::endl;

    return serial_port;
}

std::string read_serial(int _serial_port){
    char buffer[256] = {0};
    int length = read(_serial_port, buffer, sizeof(buffer) - 1); // -1 for null terminator
    
    if (length < 0) {
        std::cerr << "시리얼 읽기 오류" << std::endl;
        return "";
    }
    
    if (length > 0) {
        buffer[length] = '\0'; // null terminator 추가
        std::string input_str(buffer);
        std::cout << "수신: " << input_str << std::endl;
        return input_str;
    }
    
    // 데이터가 없을 때
    return "";
}

#endif