#include "mainwindow.h"

#include <unistd.h>
#include <iostream>
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
    m_port = new Port();
    connect(m_port, SIGNAL(error_(QString)), this, SIGNAL(print(QString)));
    connect(m_port, SIGNAL(readyData()), this, SLOT(readDataFromPort()));

    // Слот - ввод настроек!
    connect(this, SIGNAL(saveSettings(QString, int)), m_port,
            SLOT(writeSettingsPort(QString, int)));
    connect(ui->ConnectBtn, SIGNAL(clicked()), this, SLOT(connectToPort()));
    connect(ui->DisconectBtn, SIGNAL(clicked()), m_port, SLOT(disconnectPort()));
    connect(this, SIGNAL(dispSum(int)), this, SLOT(displaySum(int)),
            Qt::QueuedConnection);
    ui->testTab->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
    m_port->disconnectPort();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    delete m_port;
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
    getSumFromCard();
}

void MainWindow::writeSumToCard() {
    uint16_t sum = ui->sumLine->value();
    writeSum(sum);
}

void MainWindow::writeSum(uint16_t val) {
    m_currentCMD = write;
    print("Write");
    sendToPort(getWriteSumCMD(val));
}

void MainWindow::sendDataBtn() {
    print("sendDataBtn");
    QString data = ui->send_data->text();
    sendToPort(data.toStdString());
    print(data);
}

void MainWindow::readDataFromPort() {
    std::string dataAnswer;
    m_port->getData(dataAnswer);
    if(m_currentCMD == read )
        emit dispSum(getSum(dataAnswer));
    else if(m_currentCMD == write){
         emit dispSum(getSumWrite(dataAnswer));
    }
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

void MainWindow::getSumFromCard() {
    m_currentCMD = read;
    sendToPort(getSumCMD());
}

