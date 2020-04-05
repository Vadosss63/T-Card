#include "port.h"

#include <qdebug.h>

#include <utility>

Port::Port(QObject *parent) : QObject(parent) {}

Port::~Port() { m_port.StopIO(); }

void Port::writeToPort(const std::vector<uint8_t> &data) {
  if (m_port.IsOpen()) {
    if (!m_port.Write(data)) {
      checkErrors();
    }
  }
}

void Port::writeToPort(const std::string &data) {
  if (m_port.IsOpen()) {
    if (!m_port.Write(data)) {
      checkErrors();
    }
  }
}

void Port::getData(std::vector<uint8_t> &data) { data = std::move(m_data); }

void Port::getData(std::string &data) {
  data.resize(m_data.size());
  for (size_t i = 0; i < m_data.size(); ++i) {
    data[i] = m_data[i];
  }
}

void Port::AsincRead() {
  if (!m_port.IsOpen()) return;

  m_port.SetCallback([this](std::vector<uint8_t> &&data) {
    m_data = std::move(data);
    emit readyData();
  });
  m_port.StartIO();
}

void Port::writeSettingsPort(QString name, int baudrate) {
  qDebug("writeSettingsPort");
  m_settingsPort.name = std::move(name);
  m_settingsPort.baudRate = static_cast<QSerialPort::BaudRate>(baudrate);
}

void Port::checkErrors() {
  QString errorMsg = "Ошибка открытия порта: " +
                     QString::fromStdString(m_port.GetErrorsMessage());
  error_(errorMsg.toLocal8Bit());
}

void Port::connectPort() {
  if (m_port.Open(m_settingsPort.name.toStdString())) {
    m_port.SetBaudRate(m_settingsPort.baudRate);
    error_((m_settingsPort.name + " >> Открыт!\r").toLocal8Bit());
  } else
    checkErrors();
}

void Port::disconnectPort() {
  if (m_port.Close()) {
    m_port.StopIO();
    error_(m_settingsPort.name.toLocal8Bit() + " >> Закрыт!\r");
  } else {
    checkErrors();
  }
}

void Port::readInPort(int size) {
  m_data.resize(size);
  if (m_port.IsOpen()) m_port.Read(m_data);
}
