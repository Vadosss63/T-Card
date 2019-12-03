#include "port.h"
#include <qdebug.h>

#include <utility>


Port::Port(QObject *parent): QObject(parent){}

Port::~Port(){}

void Port::writeToPort(const std::vector<uint8_t>& data){
    if(m_port.IsOpen()){
        if(!m_port.Write(data)){
            checkErrors();
        }
    }
}

void Port::getData(std::vector<uint8_t> &data){
    data = std::move(m_data);
}

std::vector<uint8_t> Port::getDataAnswer()
{
    std::vector<uint8_t> data;
    m_port.ReadAnswer(data);
    return data;
}

void Port::writeSettingsPort(QString name, int baudrate){
    qDebug("writeSettingsPort");
    m_settingsPort.name = std::move(name);
    m_settingsPort.baudRate = (QSerialPort::BaudRate) baudrate;
}

void Port::checkErrors(){
    QString errorMsg = "Ошибка открытия порта: " + QString::fromStdString(m_port.GetErrorsMessage());
    error_(errorMsg.toLocal8Bit());
}

void Port::connectPort(){
    if(m_port.Open(m_settingsPort.name.toStdString()))
    {
        m_port.SetBaudRate(m_settingsPort.baudRate);
        error_((m_settingsPort.name + " >> Открыт!\r").toLocal8Bit());
    }else
        checkErrors();
}

void Port::disconnectPort(){
    if(m_port.Close())
    {
        error_(m_settingsPort.name.toLocal8Bit() + " >> Закрыт!\r");
    }else{
        checkErrors();
    }
}

void Port::readInPort(int size)
{
    m_data.resize(size);
    if(m_port.IsOpen())
        m_port.Read(m_data);
}
