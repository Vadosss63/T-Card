#include <QMessageBox>
#include <QString>
#include <QDesktopWidget>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <unistd.h>
#include <cerrno>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->SerchBtn, SIGNAL(clicked()), this, SLOT(searchPort()));
    connect(ui->activateBtn, SIGNAL(clicked()), this, SLOT(activateBtn()));
    connect(ui->SumBtn, SIGNAL(clicked()), this, SLOT(sumBtn()));

    connect(ui->SendDataBtn, SIGNAL(clicked()), this, SLOT(sendDataBtn()));

    connect(this, SIGNAL(print(QString)), SLOT(printConsole(QString)), Qt::QueuedConnection);

    connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)) ,this, SLOT(setBaudRate(int)));
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
    connect(this, SIGNAL(saveSettings(QString, int)), m_port, SLOT(writeSettingsPort(QString, int)));
    connect(ui->ConnectBtn, SIGNAL(clicked()), this, SLOT(connectToPort()));
    connect(ui->DisconectBtn, SIGNAL(clicked()), m_port, SLOT(disconnectPort()));
    ui->OperationCardGB->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_thread.joinable())
        m_thread.join();
}

void MainWindow::searchPort()
{
    qDebug("searchPort");
    ui->PortNameBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->PortNameBox->addItem(info.portName());
    }
}

void MainWindow::setBaudRate(int idx)
{
    bool isCustomBaudRate = !ui->BaudRateBox->itemData(idx).isValid();
    ui->BaudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->BaudRateBox->clearEditText();
    }
}

void MainWindow::writeSettings()
{
    emit saveSettings(ui->PortNameBox->currentText(), ui->BaudRateBox->currentText().toInt());
}

void MainWindow::connectToPort()
{
    writeSettings();
    m_port->connectPort();
    m_port->AsincRead();
}

void MainWindow::activateBtn()
{
    print("ACTIVATE");
//    m_port->writeToPort(CMD::getActivate());
//    printSendData(CMD::getActivate());
//    m_port->readInPort(1);
//    std::vector<uint8_t> data;
//    m_port->getData(data);
//    printReadData(data);
//    m_port->readInPort(19);
//    m_port->getData(data);
//    printReadData(data);
//    QString number = convertToStringNumber(CMD::getNumberCard(data));
//    showNumberCard(number);
}

void MainWindow::sumBtn()
{
    print("SUM");
    uint16_t sumInt = getSumFromCard();
    QString sum = convertToStringInt(sumInt);
    showSum(sum);
}

void MainWindow::writeSum(uint16_t val)
{
//    std::vector<uint8_t> wd(CMD::getWriteSum(val));
//    m_port->writeToPort(wd);
//    printSendData(wd);
//    m_port->readInPort(1);
//    std::vector<uint8_t> data;
//    m_port->getData(data);
//    printReadData(data);
//    m_port->readInPort(12);
//    m_port->getData(data);
//    printReadData(data);
}

void MainWindow::sendDataBtn()
{
    print("sendDataBtn");
    QString data = ui->send_data->text();
    QStringList sl = data.split("0x", QString::SkipEmptyParts);

    std::vector<uint8_t> dataByte(sl.size());
    bool ok;
    for (size_t i = 0; i < sl.size(); ++i) {

        dataByte[i] = sl[i].toInt(&ok, 16);
    }
    ok = false;
    m_port->writeToPort(dataByte);
    printSendData(dataByte);
}

void MainWindow::readDataFromPort()
{
    std::vector<uint8_t> dataAnswer;
    m_port->getData(dataAnswer);
    m_dataAnswer = dataAnswer;
    m_isReady = true;
    m_dataReady.notify_one();
    printReadData(dataAnswer);
}

void MainWindow::printConsole(const QString &data)
{
    std::lock_guard<std::mutex> l(m_printMutex);
    ui->consol->textCursor().insertText(data + '\r'); // Вывод текста в консоль
    ui->consol->moveCursor(QTextCursor::End); //Scroll
}

void MainWindow::showSum(const QString &number)
{
    ui->SumLine->setText(number);
}

void MainWindow::printReadData(const std::vector<uint8_t>& data)
{
    qDebug("printReadData");
    print("Recive:" +  convertToString(data));
}

void MainWindow::printSendData(const std::vector<uint8_t>& data)
{
    QString m = convertToString(data);
    qDebug(m.toStdString().c_str());
    print("Send:" +  convertToString(data));
}

QString MainWindow::convertToString(const std::vector<uint8_t> &data)
{
    QString dataString;
    for (auto ch : data){
        dataString.append(" 0x");
        QString str(QString::number(ch, 16));
        if(str.size() == 1) str = "0" + str;
        dataString.append(str);
    }
    return dataString;
}

QString MainWindow::convertToStringNumber(const std::vector<uint8_t>& data)
{
    QString dataString;
    for (auto ch : data){
        QString str(QString::number(ch, 16));
        if(str.size() == 1) str = "0" + str;
        dataString.append(str);
    }
    return dataString.toUpper();
}

QString MainWindow::convertToStringInt(uint16_t dataInt)
{         
    return QString::number(dataInt);
}

uint16_t MainWindow::getSumFromCard()
{
//    m_port->writeToPort(CMD::getSum());
//    printSendData(CMD::getSum());
//    m_port->readInPort(1);
//    std::vector<uint8_t> data;
//    m_port->getData(data);
//    printReadData(data);
//    m_port->readInPort(28);
//    m_port->getData(data);
//    printReadData(data);
//    return CMD::getSumCard(data);
}



