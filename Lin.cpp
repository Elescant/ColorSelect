#include "Lin.h"
#include "QTime"
#include <QDebug>
#include <QThread>

Lin::Lin(QSerialPort *port)
{
    serial = port;
    memset(packid_len,8,64);
}

Lin::~Lin()
{

}

bool Lin::sendMsgFrame(uint8_t command, uint8_t ack, uint8_t *dat, int num, uint8_t *buff)
{
    uint8_t cs;
    int i;

    serial->readAll();
    serial->clear(QSerialPort::Input);

    buff[0] = MSG_HEAD;
    buff[1] = (num + 1);
    buff[2] = (num + 1) >> 8;
    buff[3] = command;
    if(ack)
    {
        buff[3] |= 0x40;
    }
    memcpy(buff+4,dat,num);
    cs = 0;
    for(i = 0;i < num + 4;i++)
    {
        cs +=buff[i];
    }
    buff[i++] = cs;
    buff[i++] = MSG_TAIL;
    if(NULL != serial)
    {
        if(serial->write((char *)buff,i) == i)
        {
            serial->flush();
            return true;
        }
    }
    return false;
}

Lin::lin_err Lin::getMsgFrame(uint8_t *buff, int timeout_ms)
{
    static QByteArray arr;
    unsigned char c;
    static int length;
    static uint8_t writep = 0;
    uint8_t csum;
    static uint8_t state = U2LIN_STATE_INIT;
    int ret;
    QString str;

    while(1)
    {
        if(serial->bytesAvailable() == 0)
        {
            if(serial->waitForReadyRead(timeout_ms))
            {

            }else
            {
                qDebug()<<"time out";
                return ERR_TIMEOUT;
            }
        }
        ret = serial->read((char *)&c,1);
        if(ret == 1)
        {
            str.append(QString(" 0x%1").arg(c &0xff,2,16,QLatin1Char('0')));
            switch(state)
            {
            case U2LIN_STATE_INIT:
                if(c == MSG_HEAD)
                {
                    state = U2LIN_STATE_LL;
                }
                break;
            case U2LIN_STATE_LL:
                length = c;
                csum = 0x68 + c;
                state = U2LIN_STATE_LH;
                break;
            case U2LIN_STATE_LH:
                length += c<<8;
                qDebug()<<"length:"<<length;
                csum +=c;
                writep = 0;
                state = U2LIN_STATE_DAT;
                break;
            case U2LIN_STATE_DAT:
                buff[writep++] = c;
                csum +=c;
                if(writep == length)
                {
                    state = U2LIN_STATE_CS;
                }
                break;
            case U2LIN_STATE_CS:
                if(csum == c)
                {
                    qDebug()<<"check ok";
                    state = U2LIN_STATE_END;
                }else
                {
                    qDebug()<<"check failed";
                    state = U2LIN_STATE_INIT;
                }
                break;
            case U2LIN_STATE_END:
                state = U2LIN_STATE_INIT;
                if(c == MSG_TAIL)
                {
                    qDebug()<<"re:"<<str;
                    serial->clear(QSerialPort::Input);
                    return ERR_OK;
                }
                break;
            default:
                state = U2LIN_STATE_INIT;
                break;
            }
        }
    }



}

bool Lin::set(int *ch, int cmd, void *para)
{
    unsigned char buff[128];
    unsigned char dat[128];

    switch(cmd)
    {
    case U2LIN_COM_SETBPS:
        dat[0] = *ch;
        memcpy(dat+1,para,5);
        if(sendMsgFrame(U2LIN_COM_SETBPS,1,dat,6,buff))
        {
            if(getMsgFrame(buff,MSG_TIMEROUT) == ERR_OK)
            {
                if(buff[0] == (U2LIN_COM_SETMODE | 0x80))
                {
                    memcpy(para,buff,9);
                    return true;
                }
            }
        }
        break;
    case U2LIN_COM_SETRES:
    {
        dat[0] = *ch;
        int num = *(unsigned char *)para;
        int len = 2;
        dat[1] = num;
        unsigned char *resp = (unsigned char *)para + 1;
        if(num)
        {
            for(int i = 0;i < num; i++)
            {
                memcpy(dat + len,resp,resp[1] + 2);
                len += resp[1] + 2;
                resp += resp[1] + 2;
            }
            if( sendMsgFrame(U2LIN_COM_SETRES,1,dat,len,buff))
            {
                if(getMsgFrame(buff,MSG_TIMEROUT) == ERR_OK)
                {
                    if(buff[0] == 0x80 && buff[1] == U2LIN_COM_SETRES)
                    {
                        return true;
                    }
                }
            }
        }
    }
        break;
    case U2LIN_COM_SETMODE:
        dat[0] = *ch;
        memcpy(dat + 1,para,1);
        if(sendMsgFrame(U2LIN_COM_SETMODE,1,dat,2,buff))
        {
            if(getMsgFrame(buff,MSG_TIMEROUT) == ERR_OK)
            {
                qDebug()<<"buff = "<<buff;
                if(buff[0] == (U2LIN_COM_SETMODE | 0x80))
                {
                    memcpy(para,buff,9);
                    return true;
                }
            }
        }
        break;
    case U2LIN_COM_SETLEN:
        dat[0] = *ch;
        memcpy(dat + 1,para,64);
        if(sendMsgFrame(U2LIN_COM_SETLEN,1,dat,65,buff))
        {
            if(getMsgFrame(buff,MSG_TIMEROUT) == ERR_OK)
            {
                if(buff[0] == 0x80 && buff[1] == U2LIN_COM_SETLEN)
                {
                    return true;
                }
            }
        }
        break;
    }
    return false;
}


bool Lin::setMode(uint8_t mode)
{
    int ch = 0;
    uint8_t dat[15];

    dat[0] = 0;
    if(mode == MASTER)
    {
        dat[0] = MASTER | LIN2_0;
    }else
    {
        dat[0] = SLAVE | LIN2_0;
    }
    if(set(&ch,U2LIN_COM_SETMODE,dat))
    {
        qDebug()<<"set mode success";
        return true;
    }else
    {
        qDebug()<<"set mode failed";
        return false;
    }
}

bool Lin::setIDLength(uint8_t id, uint8_t len)
{
    int ch=0;

    if( id<= 0x3F)
    {
        packid_len[id] = len;
        return set(&ch,U2LIN_COM_SETLEN,packid_len);
    }else
    {
        return false;
    }
}

bool Lin::setIdData(uint8_t id, uint8_t *dat,uint8_t bytes)
{
    uint8_t temp[11];
    int ch = 0;

    if(bytes>8)
    {
        bytes = 8;
    }
    temp[0] = 1;//num
    temp[1] = id;//id
    temp[2] = bytes;//bytes
    memcpy(temp + 3,dat,bytes);
    return set(&ch,U2LIN_COM_SETRES,temp);
}

bool Lin::sendHeader(uint8_t id)
{
    uint8_t temp[4];
    uint8_t buf[64];

    temp[0] = 0;
    temp[1] = 0x0d;
    temp[2] = id;
    return sendMsgFrame(U2LIN_COM_SEND,0,temp,3,buf);
}

bool Lin::testlink()
{
    uint8_t dat[1] ={'l'};
    uint8_t buff[7]={0};
    uint8_t re_buf[128]={0};

    if(sendMsgFrame(U2LIN_COM_LINK,1,dat,1,buff))
    {
        if(getMsgFrame(re_buf,MSG_TIMEROUT) == ERR_OK)
        {
            if((re_buf[0]&0x3F) == U2LIN_COM_LINK && re_buf[1] == 'U'
                    && re_buf[2] == '2' )//&& re_buf[3] =='L')
            {
                return true;
            }
        }
    }
    return false;
}

