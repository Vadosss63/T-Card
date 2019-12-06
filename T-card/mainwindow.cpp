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
#include "CMD_data.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->SerchBtn, SIGNAL(clicked()), this, SLOT(searchPort()));
    connect(ui->activateBtn, SIGNAL(clicked()), this, SLOT(activateBtn()));
    connect(ui->LockBtn, SIGNAL(clicked()), this, SLOT(lockBtn()));
    connect(ui->UnlockBtn, SIGNAL(clicked()), this, SLOT(unlockBtn()));
    connect(ui->PasswordBtn, SIGNAL(clicked()), this, SLOT(passwordBtn()));
    connect(ui->SumBtn, SIGNAL(clicked()), this, SLOT(sumBtn()));
    connect(ui->DecBtn, SIGNAL(clicked()), this, SLOT(decBtn()));
    connect(ui->IncBtn, SIGNAL(clicked()), this, SLOT(incBtn()));
    connect(ui->CheckStatusBtn, SIGNAL(clicked()), this, SLOT(checkStatusBtn()));

    connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)) ,this, SLOT(setBaudRate(int)));
    ui->BaudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->BaudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->BaudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->BaudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->BaudRateBox->addItem(QLatin1String("Custom"));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    m_port = new Port(this);
    connect(m_port, SIGNAL(error_(QString)), this, SLOT(print(QString)));

    // Слот - ввод настроек!
    connect(this, SIGNAL(saveSettings(QString, int)), m_port, SLOT(writeSettingsPort(QString, int)));
    connect(ui->ConnectBtn, SIGNAL(clicked()), this, SLOT(connectToPort()));
    connect(ui->DisconectBtn, SIGNAL(clicked()), m_port, SLOT(disconnectPort()));
    ui->OperationCardGB->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::print(const QString& data)
{
    ui->consol->textCursor().insertText(data + '\r'); // Вывод текста в консоль
    ui->consol->moveCursor(QTextCursor::End); //Scroll
}

void MainWindow::writeSettings()
{
    emit saveSettings(ui->PortNameBox->currentText(), ui->BaudRateBox->currentText().toInt());
}

void MainWindow::connectToPort()
{
    writeSettings();
    m_port->connectPort();
}

void MainWindow::activateBtn()
{
    print("ACTIVATE");
    m_port->writeToPort(CMD::getActivate());
    printSendData(CMD::getActivate());
    m_port->readInPort(1);
    std::vector<uint8_t> data;
    m_port->getData(data);
    printReadData(data);
    m_port->readInPort(19);
    m_port->getData(data);
    printReadData(data);
    QString number = convertToStringNumber(CMD::getNumberCard(data));
    showNumberCard(number);
}

void MainWindow::lockBtn()
{
    print("LOCK");
    m_port->writeToPort(CMD::getLock());
    printSendData(CMD::getLock());
    m_port->readInPort(1);
    std::vector<uint8_t> data;
    m_port->getData(data);
    printReadData(data);
    m_port->readInPort(10);
    m_port->getData(data);
    printReadData(data);
}

void MainWindow::unlockBtn()
{
    print("UNLOCK");
    m_port->writeToPort(CMD::getUnlock());
    printSendData(CMD::getUnlock());
    m_port->readInPort(1);
    std::vector<uint8_t> data;
    m_port->getData(data);
    printReadData(data);
    m_port->readInPort(10);
    m_port->getData(data);
    printReadData(data);
}

void MainWindow::passwordBtn()
{
    print("PASSWORD");
    m_port->writeToPort(CMD::getPasswordSum());
    printSendData(CMD::getPasswordSum());
    m_port->readInPort(1);
    std::vector<uint8_t> data;
    m_port->getData(data);
    printReadData(data);
    m_port->readInPort(12);
    m_port->getData(data);
    printReadData(data);
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
    std::vector<uint8_t> wd(CMD::getWriteSum(val));
    m_port->writeToPort(wd);
    printSendData(wd);
    m_port->readInPort(1);
    std::vector<uint8_t> data;
    m_port->getData(data);
    printReadData(data);
    m_port->readInPort(12);
    m_port->getData(data);
    printReadData(data);
}

void MainWindow::decBtn()
{
    uint16_t currentSum = getSumFromCard();
    uint16_t val = static_cast<uint16_t>(ui->CountSumBox->value());
    print("SUM " + QString::number(currentSum) + " -- " + QString::number(val));
    if(currentSum >= val){
        val = currentSum - val;
        writeSum(static_cast<uint16_t>(val));
        sumBtn();
    }else{
        print("error sum");
    }

}

void MainWindow::incBtn()
{
    uint16_t currentSum = getSumFromCard();
    uint32_t val = static_cast<uint16_t>(ui->CountSumBox->value());
    print("SUM " + QString::number(currentSum) + " ++ " + QString::number(val));
    val += currentSum;
    if(val <= 65000){
        writeSum(static_cast<uint16_t>(val));
        sumBtn();
    }else{
        print("error sum");
    }
}

void MainWindow::checkStatusBtn()
{
    print("Status");
    m_port->writeToPort(CMD::getStatusCard());
    printSendData(CMD::getStatusCard());
    //m_port->readInPort(1);
//    std::vector<uint8_t> data = m_port->getDataAnswer();
//    //m_port->getData(data);
//    printReadData(data);
////    m_port->readInPort(12);
////    m_port->getData(data);
////    printReadData(data);

//    data = m_port->getDataAnswer();
//    printReadData(data);

//    if(CMD::checkStatusCard(data))
//    {
//        ui->StatusCardLabel->setText("Card ok");
//        ui->OperationCardGB->setEnabled(true);
//    }
//    else
//    {
//        ui->StatusCardLabel->setText("No card");
//        ui->OperationCardGB->setEnabled(false);
//    }
}

void MainWindow::showNumberCard(const QString& number)
{
    ui->NuberCardLine->setText(number);
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
    qDebug("printSendData");
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
    m_port->writeToPort(CMD::getSum());
    printSendData(CMD::getSum());
    m_port->readInPort(1);
    std::vector<uint8_t> data;
    m_port->getData(data);
    printReadData(data);
    m_port->readInPort(28);
    m_port->getData(data);
    printReadData(data);
    return CMD::convertToInt(CMD::getSumCard(data));
}
