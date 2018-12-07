#ifndef WorkThread_H
#define WorkThread_H

#include <QThread>
#include <QSerialPort>
#include <Lin.h>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();

    // QThread interface
    bool config(void);
public slots:
    void sendMsgSlot(uint8_t *dat);
signals:
    void wellDone(bool ret);
protected:
    void run();
private:
    Lin *lin_handle;
    QSerialPort *serial;
    bool serial_valid;
    bool need_send;
    uint8_t data[8];
};

#endif // WorkThread_H
