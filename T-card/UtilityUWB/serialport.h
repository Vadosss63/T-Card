#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio.hpp>
#include "errorcodeboost.h"

namespace UtilityUWB
{

class RSHandler: public ErrorCodeBoost
{
    typedef boost::asio::serial_port SerialPort;
    typedef boost::asio::io_service IOservice;
    typedef boost::system::error_code ErrorCode;
    typedef boost::asio::serial_port_base SerialPortBase;

public:
    RSHandler();
    ~RSHandler();
    RSHandler(const RSHandler& rhs) = delete;
    RSHandler& operator=(const RSHandler& rhs) = delete;
    bool IsOpen() noexcept;
    bool Open(const std::string &dev) noexcept;
    bool SetBaudRate(uint64_t rate = 115200) noexcept;
    bool Close() noexcept;

    bool Write(const std::vector<uint8_t> &reqest) noexcept;
    bool Read(std::vector<uint8_t>& answer) noexcept;
    bool ReadAnswer(std::vector<uint8_t>& answer) noexcept;

private:
    IOservice m_ioService;
    SerialPort m_serialPort;
    std::vector<uint8_t> m_readData;

};

}
#endif // SERIALPORT_H
