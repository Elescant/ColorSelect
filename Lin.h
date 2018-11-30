#ifndef LIN_H
#define LIN_H

#include <QObject>
#include <QSerialPort>

class Lin : public QObject
{
    Q_OBJECT
public:
    explicit Lin(QSerialPort *port);
    ~Lin();

    enum {MSG_HEAD = 0x68,MSG_TAIL = 0x16,MSG_LENTH = 10,MSG_TIMEROUT = 1000};
    enum {U2LIN_COM_ENTER=0,
          U2LIN_COM_LINK=1,
          U2LIN_COM_RECV = 0x10,
          U2LIN_COM_SEND = 0x11,
          U2LIN_COM_SETRES = 0x12,
          U2LIN_COM_SETMODE = 0x13,
          U2LIN_COM_EVENT = 0x14,
          U2LIN_COM_SETLEN = 0x15,
          U2LIN_COM_SETBPS = 0x16,
          U2LIN_COM_SETRLEN = 0x17,
         };
    enum {SLAVE = 0x00, MASTER = 0x01,LIN2_0 = 0x04,LIN1_3 = 0x08};
    enum {U2LIN_STATE_INIT=0,U2LIN_STATE_LL,U2LIN_STATE_LH,U2LIN_STATE_DAT,U2LIN_STATE_CS,U2LIN_STATE_END};
    typedef enum {ERR_OK = 0,ERR_EMPTY,ERR_WAIT,ERR_TIMEOUT,ERR_FRAME} lin_err;

    bool sendMsgFrame(uint8_t command, uint8_t ack, uint8_t *dat, int num, uint8_t *buff);
    lin_err getMsgFrame(uint8_t *buff, int timeout_ms);
    bool set(int *ch,int cmd,void *para);
    bool setMode(uint8_t mode);
    bool setIDLength(uint8_t id,uint8_t len);
    bool setIdData(uint8_t id , uint8_t *dat, uint8_t bytes);
    bool sendHeader(uint8_t id);
    bool testlink(void);
signals:

public slots:
private:
    QSerialPort *serial;
    uint8_t packid_len[64];
};

#endif // LIN_H
