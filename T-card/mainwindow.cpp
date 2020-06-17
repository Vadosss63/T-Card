#include "mainwindow.h"

#include <unistd.h>

#include <QDesktopWidget>
#include <QMessageBox>
#include <QMetaEnum>
#include <QScreen>
#include <QString>
#include <cerrno>

#include "command.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->SerchBtn, SIGNAL(clicked()), this, SLOT(searchPort()));
  connect(ui->activateBtn, SIGNAL(clicked()), this, SLOT(activateBtn()));
  connect(ui->deactivateBtn, SIGNAL(clicked()), this, SLOT(deactivateBtn()));

  connect(ui->getSumBtn, SIGNAL(clicked()), this, SLOT(sumBtn()));
  connect(ui->addSumBtn, SIGNAL(clicked()), this, SLOT(writeSumToCard()));

  connect(ui->SendDataBtn, SIGNAL(clicked()), this, SLOT(sendDataBtn()));

  connect(this, SIGNAL(print(QString)), SLOT(printConsole(QString)),
          Qt::QueuedConnection);

  connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setBaudRate(int)));
  ui->BaudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
  ui->BaudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
  ui->BaudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
  ui->BaudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
  ui->BaudRateBox->addItem(QLatin1String("Custom"));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  m_port = new Port(this);
  connect(m_port, SIGNAL(error_(QString)), this, SIGNAL(print(QString)));
  connect(m_port, SIGNAL(readyData()), this, SLOT(readDataFromPort()));

  // Слот - ввод настроек!
  connect(this, SIGNAL(saveSettings(QString, int)), m_port,
          SLOT(writeSettingsPort(QString, int)));
  connect(ui->ConnectBtn, SIGNAL(clicked()), this, SLOT(connectToPort()));
  connect(ui->DisconectBtn, SIGNAL(clicked()), m_port, SLOT(disconnectPort()));
  connect(this, SIGNAL(dispSum(int)), this, SLOT(displaySum(int)),
          Qt::QueuedConnection);
  //  ui->operationCardGB->setEnabled(false);
}

MainWindow::~MainWindow() {
  delete ui;
  if (m_thread.joinable()) m_thread.join();
}

void MainWindow::searchPort() {
  qDebug("searchPort");
  ui->PortNameBox->clear();
  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    ui->PortNameBox->addItem(info.portName());
  }
}

void MainWindow::setBaudRate(int idx) {
  bool isCustomBaudRate = !ui->BaudRateBox->itemData(idx).isValid();
  ui->BaudRateBox->setEditable(isCustomBaudRate);
  if (isCustomBaudRate) {
    ui->BaudRateBox->clearEditText();
  }
}

void MainWindow::writeSettings() {
  emit saveSettings(ui->PortNameBox->currentText(),
                    ui->BaudRateBox->currentText().toInt());
}

void MainWindow::connectToPort() {
  writeSettings();
  m_port->connectPort();
  m_port->AsincRead();
}

void MainWindow::activateBtn() {
  print("ACTIVATE");
  sendToPort(getActivationCMD());
}

void MainWindow::deactivateBtn() {
  print("DEACTIVATE");
  sendToPort(getDeactivationCMD());
}

void MainWindow::sumBtn() {
  print("SUM");
  uint16_t sumInt = getSumFromCard();

  emit dispSum(sumInt);
}

void MainWindow::writeSumToCard() {
  uint16_t sum = ui->sumLine->value();
  writeSum(sum);
}

void MainWindow::writeSum(uint16_t val) {
  print("Write");
  sendToPort(getWriteSumCMD(val));
}

void MainWindow::sendDataBtn() {
  print("sendDataBtn");
  QString data = ui->send_data->text();
  //    QStringList sl = data.split("0x", QString::SkipEmptyParts);

  //    std::vector<uint8_t> dataByte(sl.size());
  //    bool ok;
  //    for (size_t i = 0; i < sl.size(); ++i) {

  //        dataByte[i] = sl[i].toInt(&ok, 16);
  //    }
  //    ok = false;

  sendToPort(data.toStdString());
  print(data);
}

void MainWindow::readDataFromPort() {
  std::string dataAnswer;
  m_port->getData(dataAnswer);
  m_dataAnswer = dataAnswer;
  m_isReady = true;
  m_dataReady.notify_one();
  print(QString::fromStdString(dataAnswer));
}

void MainWindow::printConsole(const QString &data) {
  std::lock_guard<std::mutex> l(m_printMutex);
  ui->consol->textCursor().insertText(data + '\r');  // Вывод текста в консоль
  ui->consol->moveCursor(QTextCursor::End);  // Scroll
}

void MainWindow::displaySum(int sum) {
  std::lock_guard<std::mutex> l(m_printMutex);

  ui->sumDisplay->setText(QString("%1 руб.").arg(sum));
}

QString MainWindow::convertToStringNumber(const std::vector<uint8_t> &data) {
  QString dataString;
  for (auto ch : data) {
    QString str(QString::number(ch, 16));
    if (str.size() == 1) str = "0" + str;
    dataString.append(str);
  }
  return dataString.toUpper();
}

QString MainWindow::convertToStringInt(uint16_t dataInt) {
  return QString::number(dataInt);
}

uint16_t MainWindow::getSumFromCard() {
  sendToPort(getSumCMD());
  waitAnswer();
  return getSum(m_dataAnswer);
}

void MainWindow::waitAnswer() {
  std::unique_lock<std::mutex> lck(mtx);
  m_dataReady.wait_for(lck, std::chrono::seconds(1));
}
