# Entry 12/03/21
## async_serial
The `AsyncSerial` class should only handle the serial object and nothing else. A separate class should be used to handle the io_context and the buffer that the serial object uses. 

---

async_serial.hpp
```
asio::io_context& io_context_;
asio::serial_port serial;
asio::streambuf buffer;
```

