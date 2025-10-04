#!/usr/bin/env python3
import http.server
import socketserver
import json
import urllib.parse
from datetime import datetime

class TestHandler(http.server.BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/api/messages':
            # Content-Length 읽기
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            
            try:
                # JSON 파싱
                data = json.loads(post_data.decode('utf-8'))
                
                # 현재 시간 추가
                data['received_at'] = datetime.now().isoformat()
                
                # 콘솔에 출력
                print(f"\n=== 새 메시지 수신 ===")
                print(f"ID: {data.get('id', 'N/A')}")
                print(f"Message: {data.get('message', 'N/A')}")
                print(f"Timestamp: {data.get('timestamp', 'N/A')}")
                print(f"Source: {data.get('source', 'N/A')}")
                print(f"Received at: {data['received_at']}")
                print("=" * 25)
                
                # 파일에도 저장
                with open('/tmp/lora_received_messages.json', 'a') as f:
                    f.write(json.dumps(data) + '\n')
                
                # 성공 응답
                response = {
                    "status": "success",
                    "message": "Message received successfully"
                }
                
                self.send_response(200)
                self.send_header('Content-Type', 'application/json')
                self.end_headers()
                self.wfile.write(json.dumps(response).encode())
                
            except Exception as e:
                print(f"Error: {e}")
                self.send_response(400)
                self.send_header('Content-Type', 'application/json')
                self.end_headers()
                error_response = {"status": "error", "message": str(e)}
                self.wfile.write(json.dumps(error_response).encode())
        else:
            self.send_response(404)
            self.end_headers()
    
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.end_headers()
            html = """
            <h1>LoRa Gateway Test Server</h1>
            <p>Server is running on port 5000</p>
            <p>POST endpoint: /api/messages</p>
            <p>Received messages are logged to console and saved to /tmp/lora_received_messages.json</p>
            """
            self.wfile.write(html.encode())
        else:
            self.send_response(404)
            self.end_headers()

if __name__ == '__main__':
    PORT = 5000
    
    with socketserver.TCPServer(("", PORT), TestHandler) as httpd:
        print(f"LoRa Gateway Test Server Starting...")
        print(f"Server running on http://localhost:{PORT}")
        print(f"POST endpoint: http://localhost:{PORT}/api/messages")
        print(f"Received messages will be saved to: /tmp/lora_received_messages.json")
        print(f"\nPress Ctrl+C to stop the server")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")