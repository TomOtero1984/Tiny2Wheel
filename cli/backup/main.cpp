#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <signal.h>
#include "nlohmann/json.hpp"
#include "async_serial.hpp"


void async_read(AsyncSerial& async_serial)
{
    async_serial.async_read();
}

void get_usr_input(std::string& usr_in)
{
    std::cout << "> ";
    std::cin >> usr_in;
    std::cout << "usr_in: " << usr_in << std::endl;
}

void handle_usr_input(std::string& usr_in, AsyncSerial& async_serial)
{
    usr_in += "\n";
    if (usr_in == "exit\n" || usr_in == "quit\n")
    {
        exit(EXIT_SUCCESS);
    }
    else if (usr_in == "test"){
        async_serial.async_write("test\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
    else {
        std::cout << "Sending: " << usr_in << std::endl;
        async_serial.async_write(usr_in);
    }
}

void clear_usr_input(std::string& usr_input)
{
    usr_input.clear();
}

// void command_loop(AsyncSerial& async_serial)
// {
//     std::string usr_in;
//     bool loop = true;
//     while(loop) {
//         get_usr_input(usr_in);
//         handle_usr_input(usr_in, async_serial);
//         clear_usr_input(usr_in);
//     }
// }

void debug_print(std::string msg)
{
    std::cout << "[DEBUG] " << msg << std::endl;
}


int main(int argc, char *argv[]) {

    // Reading config JSON
    nlohmann::json json_config;
    std::ifstream iconfig("config.json");
    iconfig >> json_config;
    std::string port = json_config["port"];
    int baud = json_config["baud"];
    std::cout << "### Serial Info ###" << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "baud: " << baud << std::endl;

    // Initializing io_context and serial_port 
    asio::io_context io_context;
    AsyncSerial async_serial(port, baud, io_context);

    // Buffer
    async_serial.buffer.prepare(256);

    bool reading = false;
    bool sending = false;
    std::string usr_in;
    std::thread t;
    try
    {  
        async_serial.async_read();
        io_context.run();
        // Main Loop
        while(true)
        {

            // debug_print("[bool reading] " + std::to_string(reading));
            // debug_print("[read] " + std::to_string(async_serial.buffer.size()));
            // if (async_serial.buffer.size() == 0 && reading == false)
            // {
            //     reading = true;
            //     std::cout << "Reading..." << std::endl;
            //     t = std::thread(async_read, std::ref(async_serial));
            //     t.detach();
            // }
            debug_print("[print] " + std::to_string(async_serial.buffer.size()));
            if (async_serial.buffer.size() > 0)
            {
                std::cout << "Printing Buffer" << std::endl;
                reading = false;
                async_serial.print_buffer(async_serial.buffer);
                async_serial.buffer.consume(async_serial.buffer.size());
            }

            if(!reading)
            {
                get_usr_input(usr_in);
                handle_usr_input(usr_in, async_serial);
                clear_usr_input(usr_in);
            }

            // if(reading)
            // {
            //     t.join();
            // }

        }
    }
    catch (std::system_error &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}