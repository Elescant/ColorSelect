#include <QHBoxLayout>
#include <QVBoxLayout>
#include "CustomColorArea.h"
#include <QSettings>
#include <QDebug>

#define COLORPATH "ColorSelectConfig/color.ini"

CustomColorArea::CustomColorArea(QWidget *parent)
	: QWidget(parent)
	, m_iCurIndex(0)
{
	initColorItems();
}

CustomColorArea::~CustomColorArea()
{
	
}

void CustomColorArea::setGivenColor(const QColor &c)
{
	int iIndex = m_iCurIndex % 16;
	BasicColorItem *pCurItem = m_mapIndexToItem[iIndex];
	pCurItem->setColor(c);
    saveColor(m_iCurIndex,c);
	m_iCurIndex++;
}

void CustomColorArea::initColorItems()
{
    QColor color;
//    resetColor();
    color = readColor(0);
    BasicColorItem *pItem11 = new BasicColorItem(color);
    color = readColor(2);
    BasicColorItem *pItem12 = new BasicColorItem(color);
    color = readColor(4);
    BasicColorItem *pItem13 = new BasicColorItem(color);
    color = readColor(6);
    BasicColorItem *pItem14 = new BasicColorItem(color);
    color = readColor(8);
    BasicColorItem *pItem15 = new BasicColorItem(color);
    color = readColor(10);
    BasicColorItem *pItem16 = new BasicColorItem(color);
    color = readColor(12);
    BasicColorItem *pItem17 = new BasicColorItem(color);
    color = readColor(14);
    BasicColorItem *pItem18 = new BasicColorItem(color);

	QHBoxLayout *pLayout1 = new QHBoxLayout;
	pLayout1->setMargin(0);
	pLayout1->setSpacing(2);
	pLayout1->addWidget(pItem11);
	pLayout1->addWidget(pItem12);
	pLayout1->addWidget(pItem13);
	pLayout1->addWidget(pItem14);
	pLayout1->addWidget(pItem15);
	pLayout1->addWidget(pItem16);
	pLayout1->addWidget(pItem17);
	pLayout1->addWidget(pItem18);

    color = readColor(1);
    BasicColorItem *pItem21 = new BasicColorItem(color);
    color = readColor(3);
    BasicColorItem *pItem22 = new BasicColorItem(color);
    color = readColor(5);
    BasicColorItem *pItem23 = new BasicColorItem(color);
    color = readColor(7);
    BasicColorItem *pItem24 = new BasicColorItem(color);
    color = readColor(9);
    BasicColorItem *pItem25 = new BasicColorItem(color);
    color = readColor(11);
    BasicColorItem *pItem26 = new BasicColorItem(color);
    color = readColor(13);
    BasicColorItem *pItem27 = new BasicColorItem(color);
    color = readColor(15);
    BasicColorItem *pItem28 = new BasicColorItem(color);
	QHBoxLayout *pLayout2 = new QHBoxLayout;
	pLayout2->setMargin(0);
	pLayout2->setSpacing(2);
	pLayout2->addWidget(pItem21);
	pLayout2->addWidget(pItem22);
	pLayout2->addWidget(pItem23);
	pLayout2->addWidget(pItem24);
	pLayout2->addWidget(pItem25);
	pLayout2->addWidget(pItem26);
	pLayout2->addWidget(pItem27);
	pLayout2->addWidget(pItem28);

	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->setMargin(0);
	pVLayout->setSpacing(2);
	pVLayout->addLayout(pLayout1);
	pVLayout->addLayout(pLayout2);

	m_mapIndexToItem.insert(0, pItem11);
	m_mapIndexToItem.insert(1, pItem21);
	m_mapIndexToItem.insert(2, pItem12);
	m_mapIndexToItem.insert(3, pItem22);
	m_mapIndexToItem.insert(4, pItem13);
	m_mapIndexToItem.insert(5, pItem23);
	m_mapIndexToItem.insert(6, pItem14);
	m_mapIndexToItem.insert(7, pItem24);
	m_mapIndexToItem.insert(8, pItem15);
	m_mapIndexToItem.insert(9, pItem25);
	m_mapIndexToItem.insert(10, pItem16);
	m_mapIndexToItem.insert(11, pItem26);
	m_mapIndexToItem.insert(12, pItem17);
	m_mapIndexToItem.insert(13, pItem27);
	m_mapIndexToItem.insert(14, pItem18);
	m_mapIndexToItem.insert(15, pItem28);

    connect(pItem11, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem11Slot(const QColor &)));
    connect(pItem12, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem12Slot(const QColor &)));
    connect(pItem13, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem13Slot(const QColor &)));
    connect(pItem14, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem14Slot(const QColor &)));
    connect(pItem15, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem15Slot(const QColor &)));
    connect(pItem16, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem16Slot(const QColor &)));
    connect(pItem17, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem17Slot(const QColor &)));
    connect(pItem18, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem18Slot(const QColor &)));

    connect(pItem21, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem21Slot(const QColor &)));
    connect(pItem22, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem22Slot(const QColor &)));
    connect(pItem23, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem23Slot(const QColor &)));
    connect(pItem24, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem24Slot(const QColor &)));
    connect(pItem25, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem25Slot(const QColor &)));
    connect(pItem26, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem26Slot(const QColor &)));
    connect(pItem27, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem27Slot(const QColor &)));
    connect(pItem28, SIGNAL(itemClickedSignal(const QColor &)), this, SLOT(colorItem28Slot(const QColor &)));
}

void CustomColorArea::colorItem11Slot(const QColor &color)
{
    m_iCurIndex = 0;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem21Slot(const QColor &color)
{
    m_iCurIndex = 1;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem12Slot(const QColor &color)
{
    m_iCurIndex = 2;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem22Slot(const QColor &color)
{
    m_iCurIndex = 3;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem13Slot(const QColor &color)
{
    m_iCurIndex = 4;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem23Slot(const QColor &color)
{
    m_iCurIndex = 5;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem14Slot(const QColor &color)
{
    m_iCurIndex = 6;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem24Slot(const QColor &color)
{
    m_iCurIndex = 7;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem15Slot(const QColor &color)
{
    m_iCurIndex = 8;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem25Slot(const QColor &color)
{
    m_iCurIndex = 9;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem16Slot(const QColor &color)
{
    m_iCurIndex = 10;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem26Slot(const QColor &color)
{
    m_iCurIndex = 11;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem17Slot(const QColor &color)
{
    m_iCurIndex = 12;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem27Slot(const QColor &color)
{
    m_iCurIndex = 13;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem18Slot(const QColor &color)
{
    m_iCurIndex = 14;
    emit colorItemSelcSignal(color);
}
void CustomColorArea::colorItem28Slot(const QColor &color)
{
    m_iCurIndex = 15;
    emit colorItemSelcSignal(color);
}

void CustomColorArea::setGivenItemColor(int iIndex, const QColor &c)
{
	BasicColorItem *pCurItem = m_mapIndexToItem[iIndex];
    pCurItem->setColor(c);
}

QColor CustomColorArea::readColor(int index)
{
    QSettings settings(COLORPATH,QSettings::IniFormat);

    if(settings.contains(QString("Color/")+"color"+QString::number(index)))
    {
       return settings.value("Color/color"+QString::number(index)).value<QColor>();
    }else
    {
        return QColor(255,255,255);
    }
}

void CustomColorArea::saveColor(int index, QColor color)
{
     QSettings settings(COLORPATH,QSettings::IniFormat);

     settings.beginGroup("Color");
     settings.setValue("color"+QString::number(index),color);
     settings.endGroup();
}

void CustomColorArea::resetColor()
{
    QSettings settings(COLORPATH,QSettings::IniFormat);
    QString index;

    settings.beginGroup("Color");

    for(int i=0;i<16;i++)
    {
      index ="color" + QString::number(i);
      settings.setValue(index,QColor(255,255,255));
    }
    settings.endGroup();
}




