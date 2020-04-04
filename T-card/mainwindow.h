#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"

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
    void sumBtn();
    void sendDataBtn();
    void readDataFromPort();

    void printConsole(const QString& data);

private:
    void showSum(const QString& number);

    void writeSum(uint16_t val);
    void printSendData(const std::vector<uint8_t>& data);
    void printReadData(const std::vector<uint8_t>& data);

    QString convertToString(const std::vector<uint8_t>& data);
    QString convertToStringNumber(const std::vector<uint8_t>& data);
    QString convertToStringInt(uint16_t dataInt);
    uint16_t getSumFromCard();

    Port* m_port;
    Ui::MainWindow *ui;

    std::string m_dataAnswer;
    bool m_isReady = false;
    std::condition_variable m_dataReady;
    std::mutex mtx;
    std::thread m_thread;
    std::mutex m_printMutex;
};


#endif // MAINWINDOW_H
