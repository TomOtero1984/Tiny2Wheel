cmake_minimum_required(VERSION 3.8.2)

project(Tiny2Wheel_arduino)

arduino_cmake_project(T2W_arduino BOARD_NAME uno)

add_arduino_executable(T2W_arduino t2w_arduino_controller.ino)

set_target_upload_port(T2W_arduino COM6)

