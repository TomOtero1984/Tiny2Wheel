cmake_minimum_required(VERSION 3.14)

project(Tiny2Wheel_cli)

# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
set(CMAKE_C_COMPILER $ENV{x86_64-w64-mingw32_path}${TOOLCHAIN_PREFIX}-gcc.exe)
# set(CMAKE_CXX_COMPILER $ENV{x86_64-w64-mingw32_path}${TOOLCHAIN_PREFIX}-g++.exe)
set(CMAKE_RC_COMPILER $ENV{x86_64-w64-mingw32_path}${TOOLCHAIN_PREFIX}-windres.exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${CMAKE_SOURCE_DIR}/include/json)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
