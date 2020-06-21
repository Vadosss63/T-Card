#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "port.h"

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow {
    Q_OBJECT

    enum CMD
    {
        read,
        write,
        activate,
        deactivate
    };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void saveSettings(QString name, int baudrate);
    void print(const QString &data);
    void dispSum(int sum);

private slots:

    void searchPort();
    void writeSettings();
    void connectToPort();
    void setBaudRate(int idx);

    // Команды управления
    void activateBtn();
    void deactivateBtn();
    void sumBtn();
    void writeSumToCard();
    void sendDataBtn();
    void readDataFromPort();

    void printConsole(const QString &data);

    void displaySum(int sum);

private:
    void writeSum(uint16_t val);

    QString convertToStringNumber(const std::vector<uint8_t> &data);
    QString convertToStringInt(uint16_t dataInt);
    void getSumFromCard();

    template <typename T>
    void sendToPort(const T &data) {
        m_port->writeToPort(data);
    }

    CMD m_currentCMD;

    Port *m_port;
    Ui::MainWindow *ui;

    std::mutex m_printMutex;
};

#endif  // MAINWINDOW_H
