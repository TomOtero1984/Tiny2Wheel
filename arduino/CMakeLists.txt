arduino_cmake_project(T2W_arduino BOARD_NAME uno)

add_arduino_executable(T2W_arduino hello_world.cpp)

set_target_upload_port(T2W_arduino COM6)

