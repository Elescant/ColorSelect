#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QListWidget>
//#include <QEventLoop>

class ColorDialog : public QWidget
{
	Q_OBJECT

public:
	ColorDialog(QWidget *parent = 0);
	~ColorDialog();
    void showDialog();
	void setCurColor(const QColor &);
	QColor getColor();
	QRgb getRgb();

protected:
	bool eventFilter(QObject *, QEvent *);
	void show();

private:
	void initSignalAndSlotConn();
	void updateRGBColor(const QColor &);

signals:
    void colorSelect(QColor color,uint8_t fuc_bright,uint32_t id);
    void cancelBtnsignal(void);

private slots:
	void colorItemSelcSlot(const QColor &);
	void addCustomColorSlot();
	void okBtnClickedSlot();
	void cancelBtnClickedSlot();
    void closeBtnClickedSlot();
	void editChangedSlot(const QString &);
	void editFinishedSlot();

	void hValueChangedSlot(int);
	void sValueChangedSlot(int);
	void vValueChangedSlot(int);
	void rValueChangedSlot(int);
	void gValueChangedSlot(int);
	void bValueChangedSlot(int);

	void updateEditData(int, int, int);

    void listWdtItemPressSlot(QListWidgetItem *item);

private:
	class ColorSetting;
	ColorSetting * const m_pSetting;
	QLabel *m_pTitleLbl;
	QPushButton *m_pCloseBtn;
//	QEventLoop *m_pEvtLoop;
    QButtonGroup *m_btnGroup;
    QListWidget m_idView;
//	ButtonRole m_buttonRole;
    bool m_bNotEdit;
};

#endif // COLORDIALOG_H
