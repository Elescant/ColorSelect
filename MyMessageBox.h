#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QObject>
#include <QMessageBox>

class MyMessageBox : public QObject
{
    Q_OBJECT
public:
    explicit MyMessageBox(const QString &title,const QString &msg,int button0,int button1=0);
private:
    const QString m_title;
    const QString m_message;
    int m_button0;
    int m_button1;
public:
    static void information(const QString &tit, const QString &text,
                            int button0, int button1 = 0);
private:
    void readyShow(void);
private slots:
    void onShow(void);

signals:

public slots:
};

#endif // MYMESSAGEBOX_H
