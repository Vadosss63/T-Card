#ifndef PORT_H
#define PORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "UtilityUWB/serialport.h"

class Port: public QObject
{
    Q_OBJECT

    struct Settings {
        QString name;
        int32_t baudRate;
    };
public:

    Port(QObject *parent = nullptr);
    ~Port() override;

    void writeToPort(const std::vector<uint8_t>& data);
    void readInPort(int size);
    void getData(std::vector<uint8_t>& data);

    std::vector<uint8_t> getDataAnswer();
    void AsincRead();


signals:
    void error_(QString err);
    void readyData();

public slots:

    void connectPort();

    void disconnectPort();

    void writeSettingsPort(QString name, int baudrate);

private:

    void checkErrors();
    Settings m_settingsPort;
    UtilityUWB::RSHandler m_port;
    std::vector<uint8_t> m_data;
};

#endif // PORT_H
