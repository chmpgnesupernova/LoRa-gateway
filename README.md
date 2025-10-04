# concept
lora 모듈에서 serial 로 받은 string을 파싱하여,
id만을 가지고 web 으로 POST하는 프로그램. 

각 메시지는 256 byte를 넘지 않으며, 가장 앞의 3 byte는 node의 ID이다.
아두이노는 9600 보드레이트, UART 통신을 사용한다. 


# API
함수들 정리

### serial.cpp
1. setup_port() -> int
컴퓨터와 아두이노 사이의 포트를 정한다. 
보통 /dev/ttyUSB0 이지만 추가 확인 필요.
만약 올바른 포트 경로라면, open한 serial port(int)를 반환

2. read_serial(int) -> string
열어놓은 serial port에서 들어온 문자열을 buffer에 저장 (256 byte)
이후 저장한 문자열 반환 (string)

### subscriber.cpp
1. is_subscriber(string, vector<string>&) -> bool
받은 string에서 가장 앞 3바이트와 구독자 ID를 저장한 벡터를 비교한다.
만약 벡터에 ID가 없다면 false 를 반환

2. send_to_web_curl(string&, string&) -> bool
string stream으로 curl 명령어 전달
id 와 전체 메시지 전달

3. send_message_to_web(string& vector<string>&) -> bool
send_to_web 함수 트리거

### main.cpp
구독자 id 벡터 형태로 저장
1000회 도달시 종료
100회마다 확인

### server.py
간단한 curl 테스트 서버
