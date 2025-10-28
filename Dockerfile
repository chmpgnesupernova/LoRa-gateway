# Base Image 
FROM ubuntu:24.04

# Dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    libcurl4-openssl-dev \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Copy
WORKDIR /app
COPY . .

# Compile the Code
RUN g++ main.cpp serial.cpp subscriber.cpp -o lora_gateway -lcurl

# Expose Port
EXPOSE 5000

# Create script
RUN echo '#!/bin/bash' > /app/start.sh && \
    echo 'echo "Starting Python server in the background..."' >> /app/start.sh && \
    echo 'python3 server.py &' >> /app/start.sh && \
    echo 'sleep 2 # Give the server a moment to start' >> /app/start.sh && \
    echo 'echo "Starting C++ LoRa Gateway client..."' >> /app/start.sh && \
    echo './lora_gateway' >> /app/start.sh && \
    chmod +x /app/start.sh

# Set Entrypoint
ENTRYPOINT ["/app/start.sh"]
