#! /bin/bash

cmake -S . -B out/build -DCMAKE_TOOLCHAIN_FILE=./Arduino-CMake-NG/cmake/Arduino-Toolchain.cmake -G "MinGW Makefiles"

