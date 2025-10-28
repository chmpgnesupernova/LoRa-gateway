FROM gcc:12 AS builder
WORKDIR /app

COPY msg_processor/ ./msg_processor/
COPY include/ ./include/

RUN g++ -std=c++17 -O2 ./msg_processor/message_processor.cpp ./include/serialib.cpp -o /app/message_processor \
    && strip /app/message_processor

FROM alpine:latest
RUN apk add --no-cache \
    gcompat \
    libstdc++ \
    ca-certificates

COPY --from=builder /app/message_processor /app/message_processor

WORKDIR /app

VOLUME ["/data"]

CMD ["./message_processor"]