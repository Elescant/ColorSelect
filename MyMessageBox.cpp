#include "MyMessageBox.h"
#include <QEventLoop>
#include <QApplication>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

MyMessageBox::MyMessageBox(const QString &title, const QString &msg, int button0, int button1) :
    m_title(title),m_message(msg),m_button0(button0),m_button1(button1)

{

}

void MyMessageBox::information(const QString &title, const QString &text, int button0, int button1)
{
    QEventLoop eventloop;

    auto *msgbox = new MyMessageBox(title,text,button0,button1);
    connect(msgbox,&MyMessageBox::destroyed,&eventloop,&QEventLoop::quit);
    msgbox->readyShow();
    eventloop.exec();
}

void MyMessageBox::readyShow()
{
    this->moveToThread(qApp->thread());
    QTimer::singleShot(0,this,SLOT(onShow()));
}

void MyMessageBox::onShow()
{
    QMessageBox::information(NULL,m_title,m_message,(QMessageBox::StandardButton)m_button0,
                             (QMessageBox::StandardButton)m_button1);
    this->deleteLater();
}



