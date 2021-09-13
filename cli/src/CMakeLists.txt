include_directories(../include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Tiny2Wheel_cli main.cpp test.cpp)