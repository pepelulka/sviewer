FROM ubuntu:latest

WORKDIR /root/sviewer

RUN ls
RUN apt update
RUN apt -y install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    cmake \
    make \
    g++ \
    git

COPY ./ ./src

WORKDIR /root/sviewer/src

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build
