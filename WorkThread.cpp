#include "WorkThread.h"
#include <QDebug>
#include "MyMessageBox.h"
#define LINID_0x2A 0x2A

WorkThread::WorkThread()
{
//    serial = new QSerialPort();
//    lin_handle = new Lin(serial);
//    need_send = false;
}

bool WorkThread::config(void)
{
    serial->setPortName("ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setStopBits(QSerialPort::OneStop);

    if(serial->isOpen())
    {
        serial_valid  = false;
        MyMessageBox::information("设备打开失败","LIN设备被占用,请关闭其被他程序",QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel);
    }else
    {
        if(serial->open(QIODevice::ReadWrite))
        {
            serial_valid = true;
        }else
        {
            serial_valid = false;
            MyMessageBox::information("LIN 设备打开失败","未找到LIN设备或以管理员权限运行",QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Cancel);
        }
    }

    if(!serial_valid)
    {
        return false;
    }
    return true;
}

void WorkThread::run()
{
    serial = new QSerialPort();
    lin_handle = new Lin(serial);
    need_send = false;

    config();
    if(serial_valid)
    {
        if(lin_handle->testlink())
        {
            lin_handle->setMode(Lin::MASTER);
            lin_handle->setIDLength(LINID_0x2A,8);
        }
    }
    while(1)
    {
        if(need_send)
        {
            if(serial_valid && serial->isOpen())
            {
                lin_handle->setIdData(LINID_0x2A,data,8);
                bool ret = lin_handle->sendHeader(LINID_0x2A);
                qDebug()<<"wellDone";
                emit wellDone(ret);
            }
            need_send = false;
        }
    }
}

void WorkThread::sendMsgSlot(uint8_t *dat)
{
    qDebug()<<"send";
    memcpy(data,dat,8);
    need_send = true;
}


