#include <iostream>
#include "async_serial.hpp"


// AsyncSerial::AsyncSerial(std::string port, int baud) 
//     : io_context(), serial(io_context), buffer()
AsyncSerial::AsyncSerial(std::string port, int baud, asio::io_context &io_context)
    : io_context_(io_context), serial(io_context_), buffer()
{
    serial.open(port);
    serial.set_option(asio::serial_port_base::baud_rate(baud));
    serial.set_option(asio::serial_port::character_size(8));
}

void AsyncSerial::signal_callback_handler(int signum)
{
    std::cout << "Caught signal " << signum << std::endl;
    exit(signum);
}

void AsyncSerial::print_buffer(asio::streambuf &buffer)
{
    asio::const_buffers_1 buf = buffer.data();
    std::string bufstr(
        asio::buffers_begin(buf),
        asio::buffers_begin(buf) + buf.size());
    std::cout << bufstr << std::endl;
}

void AsyncSerial::read(asio::serial_port &serial,
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

void AsyncSerial::async_read()
{
    // std::cout << "buffer in thread: " << AsyncSerial::buffer.size() << std::endl;
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
    // io_context.restart();
    // io_context.run();
}

void AsyncSerial::async_write(std::string msg)
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

void AsyncSerial::handler(const asio::error_code &ec, std::size_t bytes_transferred)
{
    std::cout << "Bytes Transferred: " << bytes_transferred << std::endl;
    if (ec)
    {
        std::cout << "Error: " << ec << std::endl;
    }
}
