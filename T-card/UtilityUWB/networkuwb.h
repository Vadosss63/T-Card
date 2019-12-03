#ifndef UTILITYNETWORK_H
#define UTILITYNETWORK_H

#include <iostream>
#include <thread>
#include <functional>
#include <utility>
#include <list>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include "errorcodeboost.h"

namespace NetworkUWB
{
using namespace UtilityUWB;
// Вспомогательный класс для предачи данных
// по зволяет формировать сообщение путем склеивания заголовка
// и передоваемых данных
template <typename Data, typename Header>
union DataSend
{
    struct Message
    {
        // заголовок пакета
        Header header;
        // данные
        Data data;
    }__attribute__ ((packed));
    // Массив для отправки сырых данных
    std::array<uint8_t, sizeof(Header) + sizeof(Data)> dataSend;
    // Отправляемое сообщение
    Message message;
};


class ErrorTimout : public boost::system::error_category
{
public:
  const char* name() const BOOST_SYSTEM_NOEXCEPT
  {
    return "ErrorTimout";
  }

  std::string message(int) const override
  {
        return "The timeout of the response";
  }
};

class BufferReciveData
{
public:
    BufferReciveData();
    BufferReciveData(size_t size);
    ~BufferReciveData();
    uint8_t* Data();
    size_t Size();
    void Resize(size_t newSize);

private:
    void DeleteBuff();
    // Буффер для приема
    size_t m_sizeBufferRecive = 1432;
    uint8_t* m_bufferRecive = nullptr;
};

// Выполняет отправку данных на заданный адрес и порт
class SenderUDP: public ErrorCodeBoost
{
    typedef boost::asio::io_service IOservice;
    typedef boost::asio::ip::udp::socket Socket;
    typedef boost::asio::ip::udp::endpoint EndPoint;
    typedef boost::asio::deadline_timer::duration_type Timeout;
    typedef boost::asio::ip::address Address;
    typedef boost::asio::ip::udp Udp;

public:    
    SenderUDP();
    SenderUDP(ushort port);// отправляет на localhost
    SenderUDP(const std::string& adress, ushort port);
    ~SenderUDP();

    // Открывает сокет
    bool Open() noexcept;
    void Close();
    // Добавляет точку соединния
    void SetEndPoint(const std::string& adress, ushort port) noexcept;
    // включение режима прослушки
    // для ответа
    bool Bind(ushort port) noexcept;

    // отправка данных
    bool SendData(const std::string& data) noexcept;
    // отправка данных
    bool SendData(const uint8_t* data, size_t size) noexcept;

    // отправка данных
    // Простых структур
    template <typename SimpleStruct>
    bool SendData(const SimpleStruct& data) noexcept;

    // отправка данных
    template <typename T, size_t N>
    bool SendData(const std::array<T, N>& data) noexcept;
    template <typename T>
    bool SendData(const std::vector<T>& data) noexcept;
    template <typename Header, typename Data>
    bool SendData(const DataSend<Data, Header>& dataSend) noexcept;

    // асинхронное чтение данных с отключнием по таймеру
    bool ReadWithTimeout(const Timeout& expiry_time);
    // Получение простых стуктур
    template <typename SimpleStruct>
    SimpleStruct GetData();
    std::string GetData();

private:
    IOservice m_service;    // сервис для запуска сокета
    Socket m_sock;    // сокет
    EndPoint m_ep;    // ip адрес получателя
    // Буффер для приема ответа
    BufferReciveData m_bufferReciveData;
    size_t m_sizeRecivedByte = 0;
};

class ReciverUDP: public ErrorCodeBoost
{
    typedef boost::asio::io_service IOservice;
    typedef boost::asio::ip::udp::socket Socket;
    typedef boost::asio::ip::udp::endpoint EndPoint;
    typedef boost::asio::deadline_timer::duration_type Timeout;
    typedef boost::asio::ip::address Address;
    typedef boost::asio::ip::udp Udp;
    typedef boost::asio::ip::address_v4 Address_v4;

public:
    ReciverUDP();
    ReciverUDP(ushort port);
    // иницилизация сокета для прослушки UDP
    ReciverUDP(std::string adress, ushort port);
    ReciverUDP(std::string adress, ushort port, uint sizeBuf);
    // Закрытие сокета и завершение работы потока
    ~ReciverUDP();

    // Устанновливает
    void SetEndPoint(const std::string& adress, ushort port) noexcept;
    void CreateBuffer(uint size);

    bool OpenSocket() noexcept;
    bool Bind() noexcept;

    // Проверка работоспособность сокета
    bool IsStart() noexcept;
    // Запуск прослушки
    bool Start();
    // Остановка прослушки
    void Stop();
    // Получение простых стуктур
    template <typename SimpleStruct>
    SimpleStruct GetData();
    std::string GetData();
    // требуется освобождение ресурсов
    std::pair<uint8_t*, size_t> GetData_uint8();

    // устанавливает функцию которую надо
    // вызвать при получении данных
    // Приннимает на вход колбек и список параметров
    template<typename Func, typename... CallbackArg>
    void AddCallback(const Func callback, const CallbackArg&... callbackArg);
    // отправляет ответ
    bool SendAnswer(const std::string& data) noexcept;
    // отправляет ответ
    bool SendAnswer(uint8_t* data, size_t size) noexcept;
    // отправка простых стуктур
    template <typename SimpleStruct>
    bool SendAnswer(const SimpleStruct& data) noexcept;

    // установка режима мультикаста
    bool SetMulticastMode() noexcept;

private:
    // Прислушка входящих сообщений по UDP
    void ThreadReceiver();
    // Ассинхронное чтение даннных из сокета
    void AsyncRecive();
    // Чтение данных
    void ReadData(const ErrorCode &ec, size_t bytesTransferred);
    // Параметры создания сокета
    IOservice* m_service;
    Socket m_socket;
    EndPoint m_epSender;
    EndPoint m_epReciver;
    // Поток для прослушки сокета
    std::thread m_thread;
    std::atomic_bool m_isStart;
    // callback вызывается при получении данных
    std::function<void()> m_callback = [](){};
    // Буффер для приема
    BufferReciveData m_bufferReciveData;
    size_t m_sizeRecivedByte = 0;
};

/// ReciverStringUDP
template<typename Func, typename... CallbackArg>
void ReciverUDP::AddCallback(const Func callback, const CallbackArg&... callbackArg)
{
    if(IsStart())
        Stop();
    m_callback = std::bind(callback, callbackArg...);
}

template <typename SimpleStruct>
bool ReciverUDP::SendAnswer(const SimpleStruct& data) noexcept
{
    ClearErrors();
    m_socket.send_to(boost::asio::buffer((uint8_t*)&data, sizeof (SimpleStruct)), m_epSender, 0, GetErrors());
    return IsNoError();
}

template <typename SimpleStruct>
SimpleStruct ReciverUDP::GetData()
{
    SimpleStruct simpleStruct;
    memcpy((uint8_t*)(&simpleStruct), m_bufferReciveData.Data(), m_sizeRecivedByte);
    return simpleStruct;
}

/// SenderUDP
template <typename T, size_t N>
bool SenderUDP::SendData(const std::array<T, N>& data) noexcept
{
    ClearErrors();
    m_sock.send_to(boost::asio::buffer(data), m_ep, 0, GetErrors());
    return IsNoError();
}

template <typename T>
bool SenderUDP::SendData(const std::vector<T>& data) noexcept
{
    ClearErrors();
    m_sock.send_to(boost::asio::buffer(data), m_ep, 0, GetErrors());
    return IsNoError();
}

template <typename Header, typename Data>
bool SenderUDP::SendData(const DataSend<Data, Header>& dataSend) noexcept
{
    ClearErrors();
    m_sock.send_to(boost::asio::buffer(dataSend.dataSend), m_ep, 0, GetErrors());
    return IsNoError();
}
// отправка данных
template <typename SimpleStruct>
bool SenderUDP::SendData(const SimpleStruct& data) noexcept
{
    ClearErrors();
    m_sock.send_to(boost::asio::buffer((uint8_t*)&data, sizeof (SimpleStruct)), m_ep, 0, GetErrors());
    return IsNoError();
}

template <typename SimpleStruct>
SimpleStruct SenderUDP::GetData()
{
    SimpleStruct simpleStruct;
    memcpy((uint8_t*)(&simpleStruct), m_bufferReciveData.Data(), m_sizeRecivedByte);
    return simpleStruct;
}


} // UtilityUWB

#endif // UTILITYNETWORK_H
