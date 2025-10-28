FROM gcc:12 AS builder
WORKDIR /app

COPY html_generator/ ./html_generator/
COPY include/ ./include/

RUN g++ -std=c++17 -O2 ./html_generator/html_generator.cpp -o /app/html_generator_bin \
    && strip /app/html_generator_bin

FROM alpine:latest
RUN apk add --no-cache \
    gcompat \
    libstdc++ \
    ca-certificates

COPY --from=builder /app/html_generator_bin /app/html_generator

WORKDIR /app

VOLUME ["/data"]

CMD ["./html_generator"]

