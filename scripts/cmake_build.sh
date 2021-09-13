#! /bin/bash

build_arduino () {
    echo "### BUILDING ARDUINO ###"
    cd arduino
    cmake -S . -B ../out/build/arduino -DCMAKE_TOOLCHAIN_FILE=./Arduino-CMake-NG/cmake/Arduino-Toolchain.cmake -G "MinGW Makefiles"
    cd $intial_dir
} 

build_cli () {
    echo "### BUILDING CLI ###"
    cd cli
    cmake -S . -B ../out/build/cli -G "MinGW Makefiles"
    cd $intial_dir
}

help_msg() {
    echo -e "\n### HELP ###"
    echo -e "Description:\n\tTool for building the CMake projects in Tiny2Wheel"
    echo -e "Syntax:\n\t<Tiny2Wheels_root_dir>/scripts/cmake_build.sh <command>"
    echo -e "\nCommands:"
    echo "> arduino  --   Builds arduino CMake project"
    echo "> cli      --   Builds cli CMake project"
    echo "> all      --   Builds all CMake projects"
    echo "> help     --   Displays this help message"
    echo -e "\nExample:\n\t ./scripts/cmake_build.sh all"
}


intial_dir=$PWD
if [[ $1 == "arduino" ]]
then
    build_arduino
elif [[ $1 == "cli" ]]
then
    build_cli
elif [[$1 == "all" ]]
then
    build_arduino
    build_cli
elif [[ $1 == "help" ]] || [[ $1 == "h" ]] || [[ $1 == "?" ]]
then
    help_msg
else
    help_msg
fi
