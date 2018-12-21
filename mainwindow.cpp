#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <WorkThread.h>

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{
    colordialog = new ColorDialog();

    connect(colordialog,&ColorDialog::colorSelect,this,&MainWindow::colorChangeSlot);
    connect(colordialog,&ColorDialog::cancelBtnsignal,this,&MainWindow::cancelBtnSlot);
}

MainWindow::~MainWindow()
{
    if(colordialog)
    {
        delete colordialog;
    }
    if(workThread->isRunning())
    {
        workThread->terminate();
        workThread->deleteLater();
    }
}

void MainWindow::show()
{
    colordialog->showDialog();
}

bool MainWindow::config()
{
    workThread = new WorkThread;
    workThread->start();

    connect(this,&MainWindow::sendMsg,workThread,&WorkThread::sendMsgSlot);
    connect(workThread,&WorkThread::wellDone,this,&MainWindow::sendMsgRetSlot);
    return true;
}

void MainWindow::colorChangeSlot(QColor color, uint8_t fun_bright, uint32_t id)
{
    //qDebug()<<color.red()<<color.green()<<color.blue();
    dat[0] = (id & 0xFF);
    dat[1] = (uint8_t)((id >> 8) & 0xFF);
    dat[2] = (uint8_t)((id >> 16) & 0xFF);

    dat[3] = color.red();
    dat[4] = color.green();
    dat[5] = color.blue();

    dat[6] = fun_bright;//function and brightness
    dat[7] = checkSumCrc(dat,7);//crc

    emit sendMsg(dat);
}

void MainWindow::sendMsgRetSlot(bool ret)
{
    qDebug()<<"ret "<<ret;
}

uint8_t MainWindow::checkSumCrc(uint8_t *ptr, uint8_t len)
{
    uint8_t crc = 0;
    uint8_t i;

    while(len--)
    {
        for(i=0x80;i!=0;i>>=1) /* means data is 8 bits. it do 8 times. */
        {
            if((crc&0x80) != 0)
            {
                crc<<=1;
                crc^=0x23; /* 0x23 means CRC8=X5+X1+1 */
            }
            else
            {
                crc<<=1;
            }
            if(((*ptr)&i)!=0)
            {
                crc^=0x23;
            }
        }
        ptr++;
    }
    return(crc);
}

void MainWindow::cancelBtnSlot()
{
}


