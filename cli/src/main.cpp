#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <signal.h>
#include "nlohmann/json.hpp"
#include "async_serial.hpp"


void get_usr_input(std::string& usr_in)
{
    std::cout << "\n> ";
    std::cin >> usr_in;
    // std::cout << "[DEBUG] usr_in: " << usr_in << std::endl;
}

void handle_usr_input(std::string& usr_in, AsyncSerial& async_serial)
{
    usr_in += "\n";
    if (usr_in == "exit\n" || usr_in == "quit\n")
    {
        exit(EXIT_SUCCESS);
    }
    else if (usr_in == "read\n"){
        return;
    }
    else {
        std::cout << "Sending: " << usr_in << std::endl;
        async_serial.write(usr_in);
    }
}

void clear_usr_input(std::string& usr_input)
{
    usr_input.clear();
}

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

    // IO executor for serial port 
    asio::io_context io_context;
   
    // Buffer for async read/write
    std::string data_str;
    void *data = &data_str;
    asio::mutable_buffer buffer(data, 256);

    // Serial object 
    AsyncSerial async_serial(port, baud, io_context, buffer);
    async_serial.open();

    try
    {  
        // Read buffer
        async_serial.read();
        io_context.run();

        std::string usr_in;
        // Main Loop
        while(true)
        {
            // if (buffer.size() > 0)
            // {
            //     std::cout << "Printing Buffer" << std::endl;
            //     // reading = false;
            //     async_serial.print_buffer(buffer);
            //     buffer.consume(buffer.size());
            // }
            get_usr_input(usr_in);
            handle_usr_input(usr_in, async_serial);
            clear_usr_input(usr_in);
            async_serial.read();
            io_context.restart();
            io_context.run();
        }
    }
    catch (std::system_error &e)
    {
        async_serial.close();
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    async_serial.close();
    return 0;
}