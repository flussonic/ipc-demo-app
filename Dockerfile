FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    binutils-arm-linux-gnueabi \
    gcc-arm-linux-gnueabi \
    libc6-dev-armel-cross \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY src src

RUN mkdir -p build \
  && arm-linux-gnueabi-gcc -std=c99 -Os -Wall -Wextra -pedantic src/http_200.c -o build/http_200 -static \
  && arm-linux-gnueabi-readelf -h build/http_200

CMD ["arm-linux-gnueabi-readelf", "-h", "build/http_200"]
