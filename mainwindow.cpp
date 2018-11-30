#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{
    colordialog = new ColorDialog();
    serial = new QSerialPort();
    serial_valid = false;
    lin_handle = new Lin(serial);
    sendMsgTimer = new QTimer();
    sendMsgTimer->setInterval(100);

    connect(colordialog,&ColorDialog::colorSelect,this,&MainWindow::colorChangeSlot);
    connect(colordialog,&ColorDialog::cancelBtnsignal,this,&MainWindow::cancelBtnSlot);
    connect(sendMsgTimer,&QTimer::timeout,this,&MainWindow::sendMsgSlot);
}

void MainWindow::show()
{
    colordialog->showDialog();
}

bool MainWindow::config()
{
    serial->setPortName("ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setStopBits(QSerialPort::OneStop);

    if(serial->open(QIODevice::ReadWrite))
    {
        serial_valid = true;
    }else
    {
        serial_valid = false;
        QMessageBox::critical(NULL,"LIN 设备打开失败","未找到LIN设备或已被占用!",QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel);
    }

    if(serial_valid)
    {
        if(lin_handle->testlink())
        {
           lin_handle->setMode(Lin::MASTER);
           lin_handle->setIDLength(LINID_0x2A,8);
        }
    }
}

void MainWindow::colorChangeSlot(QColor color)
{
    //qDebug()<<color.red()<<color.green()<<color.blue();
    uint8_t dat[8];

    sendMsgTimer->stop();
    dat[0] = 0xFF;
    dat[1] = 0xFF;
    dat[2] = 0xFF;

    dat[3] = color.red();
    dat[4] = color.green();
    dat[5] = color.blue();

    dat[6] = 0x14;//function and brightness
    dat[7] = 0xAA;//crc


    if(serial_valid && serial->isOpen())
    {
        lin_handle->setIdData(LINID_0x2A,dat,8);
    }
    sendMsgTimer->start();
}

void MainWindow::sendMsgSlot()
{
    if(serial_valid && serial->isOpen())
    {
        lin_handle->sendHeader(LINID_0x2A);
    }
}

void MainWindow::cancelBtnSlot()
{
    if(sendMsgTimer->isActive())
    {
        sendMsgTimer->stop();
    }
}
