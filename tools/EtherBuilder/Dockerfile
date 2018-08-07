FROM ubuntu:16.04
MAINTAINER Umesh Jayasinghe

RUN apt-get update && apt-get install -y build-essential \
    sudo \
    cmake \
    libboost-all-dev \
    libprotobuf-dev \
    protobuf-compiler \
    clang-3.6 \
    clang-format-3.6 \
    ninja-build \
    wget \
    git \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ADD setup.sh /docker_setup.sh
RUN chmod +x /docker_setup.sh

CMD ["/bin/bash"]
ENTRYPOINT ["/docker_setup.sh"]