#include <iostream>
#include "async_serial.hpp"

AsyncSerial::AsyncSerial(std::string port,
                         int baud,
                         asio::io_context &io_context,
                         asio::mutable_buffer &buffer)
    : io_context_(io_context), serial(io_context_), buffer_(buffer),
      timer(io_context_), empty_buffer()

{
    AsyncSerial::port = port;
    AsyncSerial::baud = baud;
}

/*
Open serial port
*/
void AsyncSerial::open()
{
    std::cout << "Opening port: " << port << std::endl;
    serial.open(port);
    serial.set_option(asio::serial_port::baud_rate(baud));
    serial.set_option(asio::serial_port::character_size(8));
    serial.set_option(asio::serial_port::flow_control());
    serial.set_option(asio::serial_port::parity());
    serial.set_option(asio::serial_port::stop_bits());
    if(!serial.is_open()){
        std::cout << "Failed to open port: " << port << std::endl;
    }
}

/*
Close serial port
*/
void AsyncSerial::close()
{
    std::cout << "Closing port: " << port << std::endl;
    serial.close();
}

/*
Async Serial read, wrapper for asio::serial_port::async_read_some
*/
// void AsyncSerial::read()
// {
//     timer.expires_after(std::chrono::seconds(5));
//     timer.async_wait(std::bind(&AsyncSerial::timer_handler,
//                                    this, std::placeholders::_1));
//     serial.async_read_some(buffer_,
//                            std::bind(&AsyncSerial::read_handler,
//                                      this, std::placeholders::_1,
//                                      std::placeholders::_2));
// }
void AsyncSerial::read()
{
    timer.expires_after(std::chrono::seconds(5));
    timer.async_wait(std::bind(&AsyncSerial::timer_handler,
                               this, std::placeholders::_1));
    serial.async_read_some(buffer_,
                           std::bind(&AsyncSerial::read_handler,
                                     this, std::placeholders::_1,
                                     std::placeholders::_2));
}

/*
Async Serial write, wrapper for asio::serial_port::async_write_some
*/
void AsyncSerial::write(std::string msg)
{
    asio::async_write(serial, asio::buffer(msg, msg.size()),
                    [&](const asio::error_code &ec, std::size_t bytes_transferred)
                    {
                        if (ec)
                        {
                            std::cout << ec << std::endl;
                        }
                    });
}

void AsyncSerial::read_handler(const asio::error_code &ec, std::size_t bytes_transferred)
{
    if (ec)
    {
        std::cout << "Read Handler Error: " << ec.message() << std::endl;
    }
    std::cout << "[DEBUG] Bytes Transferred: " << bytes_transferred << std::endl;
    std::cout << static_cast<unsigned char *>(buffer_.data()) << std::endl;
    std::cout << "[DEBUG] Cancel timer" << std::endl;
    timer.cancel();
}

void AsyncSerial::write_handler(const asio::error_code &ec, std::size_t bytes_transferred)
{
    //[TODO]
    throw std::runtime_error("AsyncSerial::write_handler Not implemented");
}

void AsyncSerial::timer_handler(const asio::error_code &ec)
{
    if(!ec)
    {
        std::cout << "Read timed out" << std::endl;
        serial.cancel();
        return;
    }
    std::cout << ec << std::endl;
}