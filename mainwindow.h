#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include "ColorDialog.h"
#include <QSerialPort>
#include "Lin.h"
#include <QColor>
#include <QTimer>
#include <stdint.h>
#include <stddef.h>
#include "WorkThread.h"

class MainWindow : public QObject
{
    Q_OBJECT
public:
    enum {LINID_0x2A=0x2A};
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow();

    void show(void);
    bool config(void);
signals:
    void sendMsg(uint8_t *dat);
public slots:
    void colorChangeSlot(QColor color,uint8_t fun_bright,uint32_t id);
    void cancelBtnSlot();
    void sendMsgRetSlot(bool ret);
private:
    ColorDialog *colordialog;

    WorkThread *workThread;
    uint8_t dat[8];
};

#endif // MAINWINDOW_H
