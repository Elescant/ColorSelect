/************************************************************************
** 自定义颜色区域
************************************************************************/

#ifndef CUSTOMCOLORAREA_H
#define CUSTOMCOLORAREA_H

#include <QWidget>
#include <QMap>
#include "BasicColorItem.h"

class CustomColorArea : public QWidget
{
	Q_OBJECT

public:
	CustomColorArea(QWidget *parent);
	~CustomColorArea();
	void setGivenColor(const QColor &);

private slots:
    void colorItem11Slot(const QColor &color);
    void colorItem12Slot(const QColor &color);
    void colorItem13Slot(const QColor &color);
    void colorItem14Slot(const QColor &color);
    void colorItem15Slot(const QColor &color);
    void colorItem16Slot(const QColor &color);
    void colorItem17Slot(const QColor &color);
    void colorItem18Slot(const QColor &color);
    void colorItem21Slot(const QColor &color);
    void colorItem22Slot(const QColor &color);
    void colorItem23Slot(const QColor &color);
    void colorItem24Slot(const QColor &color);
    void colorItem25Slot(const QColor &color);
    void colorItem26Slot(const QColor &color);
    void colorItem27Slot(const QColor &color);
    void colorItem28Slot(const QColor &color);
private:
	void initColorItems();
	void setGivenItemColor(int iIndex, const QColor &);
    QColor readColor(int index);
    void saveColor(int index, QColor color);
    void resetColor(void);

signals:
	void colorItemSelcSignal(const QColor &);

private:
	QMap<int, BasicColorItem *> m_mapIndexToItem;
	int m_iCurIndex;
};

#endif // CUSTOMCOLORAREA_H
