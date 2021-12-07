#pragma once

#include <vector>
#include <boost/asio.hpp>

class Serial
{
public:
    Serial(const std::string& port, const unsigned int baud_rate)
    : io(), serial(io,port)
    {
        using namespace boost::asio;
        serial.set_option(serial_port_base::baud_rate(baud_rate));
    }

    uint8_t read()
    {
        using namespace boost;
        uint8_t c;
        asio::read(serial,asio::buffer(&c,1));
        return c;
    }

    std::vector<uint8_t> read(const size_t size){
        using namespace boost;
        std::vector<uint8_t> buf(size);
        asio::read(serial, asio::buffer(buf.data(),size));
        return buf;
    }

    template<typename T>
    void readInto(T& obj){
        boost::asio::read(serial, boost::asio::buffer(&obj, sizeof(T)));
    }

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};
