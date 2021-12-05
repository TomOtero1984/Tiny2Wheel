#ifndef _ASYNCSERIAL_H
#define _ASYNCSERIAL_H
#include "asio.hpp"


class AsyncSerial
{
public:
    AsyncSerial(std::string port,
                int baud,
                asio::io_context &io_context,
                asio::mutable_buffer &buffer);

    void open();
    void close();
    void read();
    void write(std::string msg);
    void read_handler(const asio::error_code &ec, std::size_t bytes_transferred);
    void write_handler(const asio::error_code &ec, std::size_t bytes_transferred);
    void timer_handler(const asio::error_code &ec);

    asio::io_context &io_context_;
    asio::serial_port serial;
    asio::mutable_buffer &buffer_;
    asio::steady_timer timer;
    asio::const_buffer empty_buffer;

private:
    std::string port;
    int baud;
};

#endif //_ASYNCSERIAL_H
