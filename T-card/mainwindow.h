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

private slots:

    void searchPort();
    void writeSettings();
    void connectToPort();

    void setBaudRate(int idx);
    void print(const QString& data);
    // Команды управления
    void activateBtn();
    void lockBtn();
    void unlockBtn();
    void passwordBtn();
    void sumBtn();
    void decBtn();
    void incBtn();
    void checkStatusBtn();

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
};

#endif // MAINWINDOW_H
