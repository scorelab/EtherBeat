FROM ubuntu:16.04
MAINTAINER Umesh Jayasinghe

RUN apt-get update && apt-get install -y build-essential \
    sudo \
    gcc \
    g++ \
    libboost-all-dev \
    libprotobuf-dev \
    protobuf-compiler \
    clang-3.6 \
    clang-format-3.6 \
    ninja-build \
    wget \
    git \
    libsnappy-dev \
    autoconf \
    autogen \
    libtool \
    libgtest-dev \
    libleveldb-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ADD docker_setup.sh /usr/src/docker_setup.sh
RUN chmod +x /usr/src/docker_setup.sh

CMD ["/bin/bash"]