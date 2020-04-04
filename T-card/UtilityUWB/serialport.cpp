#include "serialport.h"
#include <iostream>

namespace UtilityUWB
{
RSHandler::RSHandler(): m_serialPort(m_ioService), m_readData(30){}

RSHandler::~RSHandler()
{
    Close();
}

bool RSHandler::IsOpen() noexcept
{
    return m_serialPort.is_open();
}

bool RSHandler::Open(const std::string &dev) noexcept
{
    ClearErrors();
    m_serialPort.open(dev, GetErrors());
    if(m_serialPort.is_open()){
        m_serialPort.set_option(SerialPortBase::character_size(8));
        m_serialPort.set_option(SerialPortBase::flow_control(SerialPortBase::flow_control::none));
        m_serialPort.set_option(SerialPortBase::parity(SerialPortBase::parity::none));
        m_serialPort.set_option(SerialPortBase::stop_bits(SerialPortBase::stop_bits::one));
    }
    return IsNoError();
}

bool RSHandler::SetBaudRate(uint64_t rate) noexcept
{
    ClearErrors();
    if(m_serialPort.is_open())
        m_serialPort.set_option(SerialPortBase::baud_rate(rate), GetErrors());
    return IsNoError();
}

bool RSHandler::Close() noexcept
{
    ClearErrors();
    if(m_serialPort.is_open())
        m_serialPort.close(GetErrors());
    return IsNoError();
}

bool RSHandler::Write(const std::vector<uint8_t>& reqest) noexcept
{
    ClearErrors();
    m_serialPort.write_some(boost::asio::buffer(reqest), GetErrors());
    return IsNoError();
}

bool RSHandler::Write(const std::string& reqest) noexcept
{
    ClearErrors();
    m_serialPort.write_some(boost::asio::buffer(reqest), GetErrors());
    return IsNoError();
}

bool RSHandler::Read(std::vector<uint8_t>& answer) noexcept
{
    ClearErrors();
    // ожидание ответ в N байт
    size_t sizeReciveDelta = answer.size();
    while (sizeReciveDelta > 0) {
        if(!IsNoError())
            return false;

        size_t index = (answer.size() - sizeReciveDelta);
        size_t size = m_serialPort.read_some(boost::asio::buffer(&answer[index], sizeReciveDelta), GetErrors());
        sizeReciveDelta -= size;
        std::cout << "size recive - " << size << "; delta - " << sizeReciveDelta << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "================================" << std::endl;
    return IsNoError();
}

bool RSHandler::ReadAnswer(std::vector<uint8_t> &answer) noexcept
{
    ClearErrors();
    answer.resize(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    if(!IsNoError())
        return false;
    size_t size = m_serialPort.read_some(boost::asio::buffer(m_readData.data(), m_readData.size()), GetErrors());
    answer.insert(answer.end(), m_readData.begin(), m_readData.begin() + size);
    std::cout << "size recive - " << size << "; count data - " << answer.size() << std::endl;

    std::cout << "================================" << std::endl;
    return IsNoError();
}

void RSHandler::SetCallback(std::function<void(std::vector<uint8_t>&&)> callback)
{
    m_callback = callback;
}

void RSHandler::AsyncReadSome()
{
    if (!m_serialPort.is_open()) return;

    m_serialPort.async_read_some(
                boost::asio::buffer(m_readData.data() , m_readData.size()),
                boost::bind(&RSHandler::OnReceive,
                            this, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void RSHandler::StartIO()
{
    boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
    AsyncReadSome();
}

void RSHandler::StopIO()
{
    std::lock_guard<std::mutex> look(m_mutex);

    if (m_serialPort.is_open()) {
        m_serialPort.cancel();
        m_serialPort.close();
    }
    m_ioService.stop();
    m_ioService.reset();
}

void RSHandler::OnReceive(const ErrorCode &ec, size_t bytes_transferred)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_serialPort.is_open()) return;
    if (ec) {
        AsyncReadSome();
        return;
    }
    std::vector<uint8_t> tmp(m_readData.begin(), m_readData.begin() + bytes_transferred);
    m_callback(std::move(tmp));
    AsyncReadSome();
}

}
