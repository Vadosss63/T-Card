#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <functional>

#include "errorcodeboost.h"

namespace UtilityUWB
{

class RSHandler: public ErrorCodeBoost
{
    using SerialPort = boost::asio::serial_port;
    using IOservice = boost::asio::io_service;
    using ErrorCode = boost::system::error_code;
    using SerialPortBase = boost::asio::serial_port_base;

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
    bool Write(const std::string &reqest) noexcept;
    bool Read(std::vector<uint8_t>& answer) noexcept;
    bool ReadAnswer(std::vector<uint8_t>& answer) noexcept;

    void SetCallback(std::function<void(std::vector<uint8_t>&&)> callback);
    void AsyncReadSome();

    void StartIO();
    void StopIO();
private:


    void OnReceive(const ErrorCode &ec, size_t bytes_transferred);

    IOservice m_ioService;
    SerialPort m_serialPort;
    std::vector<uint8_t> m_readData;
//    uint8_t m_curentBeginByte = 0;
    std::mutex m_mutex;
//    std::chrono::steady_clock::time_point m_lastTime;
    std::function<void(std::vector<uint8_t>&&)> m_callback;
};

}
#endif // SERIALPORT_H
