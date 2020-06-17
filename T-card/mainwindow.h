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
  uint16_t getSumFromCard();

  void waitAnswer();

  template <typename T>
  void sendToPort(const T &data) {
    m_isReady = false;
    m_port->writeToPort(data);
  }

  Port *m_port;
  Ui::MainWindow *ui;

  std::string m_dataAnswer;
  std::atomic<bool> m_isReady = false;
  std::condition_variable m_dataReady;
  std::thread m_thread;
  std::mutex m_printMutex;
  std::mutex mtx;
};

#endif  // MAINWINDOW_H
