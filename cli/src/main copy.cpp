#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <signal.h>

#include "asio.hpp"
#include "nlohmann/json.hpp"


void signal_callback_handler(int signum);
void print_buffer(asio::streambuf &buffer);
void read(asio::serial_port &serial, 
          asio::streambuf &buffer, 
          asio::error_code& ec);
void async_read();
void async_write();
void handler(const asio::error_code &ec, std::size_t bytes_transferred);

asio::io_context io_context;
asio::serial_port serial(io_context);
asio::streambuf buffer;
int main()
{
    signal(SIGINT, signal_callback_handler);
    // Reading config JSON
    nlohmann::json json_config;
    std::ifstream iconfig("config.json");
    iconfig >> json_config;
    std::string port = json_config["port"];
    int baud = json_config["baud"];

    std::cout << "### Serial Info ###" << std::endl; 
    std::cout << "port: " << port << std::endl;
    std::cout << "baud: " << baud << std::endl;

    // Buffer
    // asio::streambuf buffer;
    buffer.prepare(256);
    // Serial
    // asio::error_code ec;
    // std::size_t bytes_transferred;
    
    serial.open(port);
    serial.set_option(asio::serial_port_base::baud_rate(baud));
    serial.set_option(asio::serial_port::character_size(8));
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Strand
    asio::io_context::strand strand(io_context);
    std::allocator<char[]> alc;
    
    // strand.post(async_write, alc);
    async_write(); 
    bool reading = false;
    while (true)
    {
        std:: cout << buffer.size() << std::endl;
        if(buffer.size() == 0 && reading == false){
            reading = true;
            std::cout << "Reading..." << std::endl;

            // strand.post(async_read, alc);
            asio::thread t(async_read);
        }
        if (buffer.size() > 0)
        {
            std::cout << "Printing Buffer" << std::endl;
            reading = false;
            print_buffer(buffer);
            buffer.consume(buffer.size());
            async_write();
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        // asio::write(serial, asio::buffer("test\n"));
    }

    // while(true){
    //     read(serial, buffer, ec);
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    // }
    serial.close();
    return 0;
}


void signal_callback_handler(int signum)
{
    std::cout << "Caught signal " << signum << std::endl;
    exit(signum);
}


void print_buffer(asio::streambuf &buffer)
{
    asio::const_buffers_1 buf = buffer.data();
    std::string bufstr(
        asio::buffers_begin(buf),
        asio::buffers_begin(buf) + buf.size());
    std::cout << bufstr << std::endl;
}


void read(asio::serial_port &serial,
          asio::streambuf &buffer,
          asio::error_code &ec)
{
    asio::read_until(serial, buffer, '\n', ec);
    if (ec.value())
    {
        std::cerr << ec.message() << std::endl;
    }
    print_buffer(buffer);
}

void async_read()
{
    std::cout << "buffer in thread: " << buffer.size() << std::endl;
    asio::async_read_until(serial, buffer, "\n",
        [&](const asio::error_code &ec, std::size_t bytes_transferred)
        {
            std::cout << buffer.size() << std::endl;
            std::cout << "Bytes Transferred: " << bytes_transferred << std::endl;
            if (ec)
            {
                std::cout << "Error: " << ec << std::endl;
            }
        });
    io_context.restart();
    io_context.run();
}

void async_write()
{
    std::string msg = "test\n";
    asio::async_write(serial, asio::buffer(msg, msg.size()),
        [&](const asio::error_code &ec, std::size_t bytes_transferred)
        {
            if (ec)
            {
                std::cout << ec << std::endl;
            }
        });
}

void handler(const asio::error_code &ec, std::size_t bytes_transferred)
{
    std::cout << "Bytes Transferred: " << bytes_transferred << std::endl;
    if (ec)
    {
        std::cout << "Error: " << ec << std::endl;
    }
}