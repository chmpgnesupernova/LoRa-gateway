#!/bin/bash

# LoRa Gateway 주기적 실행 스크립트
# 5분마다 message-processor와 html-generator 컨테이너를 실행합니다

# 스크립트가 있는 디렉토리로 이동
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "$(date): LoRa Gateway 주기적 실행 시작 (5분 간격)"
echo "현재 디렉토리: $SCRIPT_DIR"
echo "데이터 디렉토리: $SCRIPT_DIR/data"

# Ctrl+C로 종료할 수 있도록 트랩 설정
trap 'echo -e "\n$(date): 스크립트 종료"; exit 0' INT

while true; do
    echo "=========================================="
    echo "$(date): 컨테이너 실행 시작"
    
    # Message Processor 실행
    echo "$(date): Message Processor 실행 중..."
    if docker run --rm -v "$SCRIPT_DIR/data:/data" message-processor:0.0.1; then
        echo "$(date): Message Processor 완료"
    else
        echo "$(date): Message Processor 실행 실패"
    fi
    
    # HTML Generator 실행
    echo "$(date): HTML Generator 실행 중..."
    if docker run --rm -v "$SCRIPT_DIR/data:/data" html_generator:0.0.1; then
        echo "$(date): HTML Generator 완료"
    else
        echo "$(date): HTML Generator 실행 실패"
    fi
    
    echo "$(date): 모든 컨테이너 실행 완료"
    echo "$(date): 30초 대기 중... (Ctrl+C로 종료)"
    
    # 5분 (300초) 대기
    sleep 30
done