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

class MainWindow : public QObject
{
    Q_OBJECT
public:
    enum {LINID_0x2A=0x2A};
    explicit MainWindow(QObject *parent = nullptr);

    void show(void);
    bool config(void);
signals:

public slots:
    void colorChangeSlot(QColor color,uint8_t fun_bright,uint32_t id);
    void sendMsgSlot(void);
    void cancelBtnSlot();

private:
    ColorDialog *colordialog;
    QSerialPort *serial;
    Lin *lin_handle;
    bool serial_valid;
    QTimer *sendMsgTimer;
};

#endif // MAINWINDOW_H
