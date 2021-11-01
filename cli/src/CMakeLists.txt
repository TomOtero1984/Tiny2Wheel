find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/asio)
include_directories(${CMAKE_SOURCE_DIR}/include/asio/include)
include_directories(${CMAKE_SOURCE_DIR}/include/json/include/nlohmann)

add_executable(${CMAKE_PROJECT_NAME} main.cpp async_serial.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32 nlohmann_json::nlohmann_json Threads::Threads)

