# concept
lora 모듈에서 serial 로 받은 string을 파싱하여,
id만을 가지고 web 으로 POST하는 프로그램. 

# Serial
아두이노와 serial 통신을 진행한다. 
아두이노는 9600 보드레이트로, 컴퓨터와 UART 통신을 사용한다. 
컴퓨터는 아두이노로부터의 데이터을 string으로 받아, web으로 post한다. 

# subscriber
아두이노로부터 받은 메시지의 가장 앞에는 id 가 있으며, id는 총 3바이트 이다. 