#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"
#include "rfid/ReadAndWrite.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void saveSettings(QString name, int baudrate);
    void print(const QString& data);

private slots:

    void searchPort();
    void writeSettings();
    void connectToPort();

    void setBaudRate(int idx);

    // Команды управления
    void activateBtn();
    void lockBtn();
    void unlockBtn();
    void passwordBtn();
    void sumBtn();
    void decBtn();
    void incBtn();
    void checkStatusBtn();
    void sendDataBtn();
    void InitBtn();
    void TestConnect();
    void TestBtn();

    void readDataFromPort();

    void printConsole(const QString& data);

    void setTimeoutSendData();

private:

    void showNumberCard(const QString& number);
    void showSum(const QString& number);
    void writeSum(uint16_t val);
    void printSendData(const std::vector<uint8_t>& data);
    void printReadData(const std::vector<uint8_t>& data);
    QString convertToString(const std::vector<uint8_t>& data);
    QString convertToStringNumber(const std::vector<uint8_t>& data);
    QString convertToStringInt(uint16_t dataInt);

    Port* m_port;
    Ui::MainWindow *ui;

    uint16_t getSumFromCard();

    uint16_t m_timeoutForSendData = 30;

    MFRC522Test m_MFRC522Test;
    std::vector<uint8_t> m_dataAnswer;
    bool m_isReady = false;
    std::condition_variable m_dataReady;
    std::mutex mtx;
    std::thread m_thread;
    std::mutex m_printMutex;
};


#endif // MAINWINDOW_H
