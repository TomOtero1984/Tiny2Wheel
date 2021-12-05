#ifndef _ASYNCSERIAL_H
#define _ASYNCSERIAL_H
#include "asio.hpp"


class AsyncSerial
{
public:
    AsyncSerial(std::string port, int baud, asio::io_context &io_context);

    void signal_callback_handler(int signum);
    void print_buffer(asio::streambuf &buffer);
    void read(asio::serial_port &serial,
              asio::streambuf &buffer,
              asio::error_code &ec);
    void async_read();
    void async_write(std::string msg);
    static void handler(const asio::error_code &ec, std::size_t bytes_transferred);
    
    asio::io_context& io_context_;
    asio::serial_port serial;
    asio::streambuf buffer;
};

#endif //_ASYNCSERIAL_H
