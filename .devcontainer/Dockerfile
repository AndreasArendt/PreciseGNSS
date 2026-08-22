FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

USER root

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ccache \
        clang-format \
        gdb \
        libboost-chrono-dev \
        libboost-date-time-dev \
        libboost-filesystem-dev \
        libboost-program-options-dev \
        libboost-regex-dev \
        libboost-serialization-dev \
        libboost-system-dev \
        libboost-thread-dev \
        libboost-timer-dev \
        libeigen3-dev \
        libgtsam-dev \
        libmetis-dev \
        libsuitesparse-dev \
        libtbb-dev \
        ninja-build \
        pkg-config \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Ubuntu 24.04's GTSAM CMake export references this test-only archive, but
# libgtsam-dev does not ship it. An empty archive satisfies the export check;
# application targets link against libgtsam, not CppUnitLite.
RUN ar rcs /usr/lib/x86_64-linux-gnu/libCppUnitLite.a

USER vscode
