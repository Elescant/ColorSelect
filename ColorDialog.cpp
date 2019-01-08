#include <QMouseEvent>
#include "ColorDialog.h"
#include "ui_ColorDialog.h"
#include <QDebug>

class ColorDialog::ColorSetting
{
public:
	Ui::ColorDialog ui;
	int m_iHue;
	int m_iSaturation;
	int m_iBrightness;
	bool m_bPressed;
	QPoint m_ptPress;

public:
	ColorSetting()
		: m_iHue(0)
		, m_iSaturation(0)
		, m_iBrightness(0)
		, m_bPressed(false)
	{
	}
	~ColorSetting()
	{

	}

private:

};

ColorDialog::ColorDialog(QWidget *parent)
	: QWidget(parent)
	, m_pSetting(new ColorSetting)
	, m_bNotEdit(false)
{
	m_pSetting->ui.setupUi(this);
	//标题
	m_pTitleLbl = new QLabel(this);
    m_pTitleLbl->setText(QString::fromLocal8Bit(""));
	m_pTitleLbl->setFixedSize(190, 34);

	//关闭按钮
	m_pCloseBtn = new QPushButton(this);
	m_pCloseBtn->setFixedSize(22, 34);
	m_pCloseBtn->setObjectName("m_pCloseBtn");

//	m_buttonRole = Yes;
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(this);

	QRegExp rx("(\\d?[a-f]?){0,6}");
	m_pSetting->ui.colorEdit->setValidator(new QRegExpValidator(rx, this));
	m_pSetting->ui.colorEdit->setText("000000");

    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(m_pSetting->ui.radioBtn_off,0);
    m_btnGroup->addButton(m_pSetting->ui.radioBtn_on,1);
    m_btnGroup->addButton(m_pSetting->ui.radioBtn_slow,2);
    m_btnGroup->addButton(m_pSetting->ui.radioBtn_fast,3);
    m_btnGroup->addButton(m_pSetting->ui.radioBtn_gradual,4);

	this->setFocusPolicy(Qt::ClickFocus);

    autoUpdateTimer = new QTimer(this);
    connect(autoUpdateTimer,&QTimer::timeout,this,&ColorDialog::autoUpdateTimerout);

	initSignalAndSlotConn();
}

ColorDialog::~ColorDialog()
{
	delete m_pSetting;
}

void ColorDialog::ColorDialog::showDialog()
{
	show();
//	QEventLoop evtLoop;
//	m_pEvtLoop = &evtLoop;
//	evtLoop.exec();

//	m_pEvtLoop = NULL;
//	hide();
//	return m_buttonRole;
}

void ColorDialog::setCurColor(const QColor &c)
{
	colorItemSelcSlot(c);
}

QColor ColorDialog::getColor()
{
	QColor color;
	color.setHsv(m_pSetting->m_iHue, m_pSetting->m_iSaturation, m_pSetting->m_iBrightness);
	return color;
}

QRgb ColorDialog::getRgb()
{
	QColor color;
	color.setHsv(m_pSetting->m_iHue, m_pSetting->m_iSaturation, m_pSetting->m_iBrightness);
	return color.rgb();
}

bool ColorDialog::eventFilter(QObject *obj, QEvent *ev)
{
	switch (ev->type())
	{
	case QEvent::MouseButtonPress:
	{
		QMouseEvent *e = dynamic_cast<QMouseEvent *>(ev);
		if (e->pos().y() > 29)
		{
			return false;
		}
		if (e->buttons() & Qt::LeftButton)
		{
			m_pSetting->m_bPressed = true;
			m_pSetting->m_ptPress = e->globalPos() - this->frameGeometry().topLeft();
		}
	}
		break;
	case QEvent::MouseButtonRelease:
	{
		m_pSetting->m_bPressed = false;
	}
		break;
	case QEvent::MouseMove:
    {
		QMouseEvent *e = dynamic_cast<QMouseEvent *>(ev);
		if (m_pSetting->m_bPressed)
		{
			move(this->mapToGlobal(e->pos() - m_pSetting->m_ptPress));
		}
	}
		break;
	case QEvent::Resize:
	{
		m_pTitleLbl->move(8, 0);
		m_pCloseBtn->move(width() - 24, -8);
	}
		break;
	default:
		break;
	}
    return  QWidget::eventFilter(obj, ev);
}

void ColorDialog::show()
{
    QWidget::show();
}

void ColorDialog::initSignalAndSlotConn()
{
    connect(m_pSetting->ui.listWdt_id,&QListWidget::itemPressed,this,&ColorDialog::listWdtItemPressSlot,Qt::QueuedConnection);

	connect(m_pSetting->ui.hColorWgt, SIGNAL(hueChangedSignal(int)), m_pSetting->ui.svColorWgt, SLOT(hueChangedSlot(int)));
	connect(m_pSetting->ui.svColorWgt, SIGNAL(svChangedSignal(int, int, int)), m_pSetting->ui.previewWgt, SLOT(svChangedSlot(int, int, int)));
	connect(m_pSetting->ui.previewWgt, SIGNAL(svChangedSignal(int, int, int)), this, SLOT(updateEditData(int, int, int)));
	connect(m_pSetting->ui.basicColorWgt, SIGNAL(colorItemSelcSignal(const QColor &)), this, SLOT(colorItemSelcSlot(const QColor &)));
	connect(m_pSetting->ui.customColorWgt, SIGNAL(colorItemSelcSignal(const QColor &)), this, SLOT(colorItemSelcSlot(const QColor &)));
	connect(m_pSetting->ui.colorEdit, SIGNAL(textEdited(const QString &)), this, SLOT(editChangedSlot(const QString &)));
	connect(m_pSetting->ui.colorEdit, SIGNAL(editingFinished()), this, SLOT(editFinishedSlot()));

	connect(m_pSetting->ui.addCustomColorBtn, SIGNAL(clicked()), this, SLOT(addCustomColorSlot()));
	connect(m_pSetting->ui.okBtn, SIGNAL(clicked()), this, SLOT(okBtnClickedSlot()));
	connect(m_pSetting->ui.cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtnClickedSlot()));
    connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(closeBtnClickedSlot()));

	connect(m_pSetting->ui.hSpinBox, SIGNAL(valueChanged(int)), this, SLOT(hValueChangedSlot(int)));
	connect(m_pSetting->ui.sSpinBox, SIGNAL(valueChanged(int)), this, SLOT(sValueChangedSlot(int)));
	connect(m_pSetting->ui.vSpinBox, SIGNAL(valueChanged(int)), this, SLOT(vValueChangedSlot(int)));
	connect(m_pSetting->ui.rSpinBox, SIGNAL(valueChanged(int)), this, SLOT(rValueChangedSlot(int)));
	connect(m_pSetting->ui.gSpinBox, SIGNAL(valueChanged(int)), this, SLOT(gValueChangedSlot(int)));
	connect(m_pSetting->ui.bSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bValueChangedSlot(int)));

    //    typedef  void (QSpinBox::*pfun)(int);
    //    pfun fun = &QSpinBox::valueChanged;

    //    void (QSpinBox::*fun)(int);
    //    fun = &QSpinBox::valueChanged;

    connect(m_pSetting->ui.hSpinBox,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,m_pSetting->ui.hSlider_h,&QSlider::setValue);
    connect(m_pSetting->ui.hSlider_h,&QSlider::valueChanged,m_pSetting->ui.hSpinBox,&QSpinBox::setValue);

    connect(m_pSetting->ui.sSpinBox,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,m_pSetting->ui.hSlider_s,&QSlider::setValue);
    connect(m_pSetting->ui.hSlider_s,&QSlider::valueChanged,m_pSetting->ui.sSpinBox,&QSpinBox::setValue);

    connect(m_pSetting->ui.vSpinBox,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,m_pSetting->ui.hSlider_v,&QSlider::setValue);
    connect(m_pSetting->ui.hSlider_v,&QSlider::valueChanged,m_pSetting->ui.vSpinBox,&QSpinBox::setValue);

    connect(m_pSetting->ui.rSpinBox,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,m_pSetting->ui.hSlider_red,&QSlider::setValue);
    connect(m_pSetting->ui.hSlider_red,&QSlider::valueChanged,m_pSetting->ui.rSpinBox,&QSpinBox::setValue);

    connect(m_pSetting->ui.gSpinBox,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,m_pSetting->ui.hSlider_green,&QSlider::setValue);
    connect(m_pSetting->ui.hSlider_green,&QSlider::valueChanged,m_pSetting->ui.gSpinBox,&QSpinBox::setValue);

    connect(m_pSetting->ui.bSpinBox,(void (QSpinBox::*)(int))&QSpinBox::valueChanged,m_pSetting->ui.hSlider_blue,&QSlider::setValue);
    connect(m_pSetting->ui.hSlider_blue,&QSlider::valueChanged,m_pSetting->ui.bSpinBox,&QSpinBox::setValue);

    connect(m_pSetting->ui.checkBox,&QCheckBox::stateChanged,this,&ColorDialog::checkBoxstateChanged);
}

void ColorDialog::updateRGBColor(const QColor &color)
{
    //qDebug()<<"updateRGBColor";
	m_pSetting->ui.previewWgt->setNewColor(color);

	m_pSetting->ui.rSpinBox->setValue(color.red());
	m_pSetting->ui.gSpinBox->setValue(color.green());
	m_pSetting->ui.bSpinBox->setValue(color.blue());

	QString strR = QString::number(color.red(), 16);
	QString strG = QString::number(color.green(), 16);
	QString strB = QString::number(color.blue(), 16);
	QString strRgb = QString("%1%2%3").arg(QString("%1").arg(strR.size() == 1 ? strR.prepend("0") : strR),
		QString("%1").arg(strG.size() == 1 ? strG.prepend("0") : strG),
		QString("%1").arg(strB.size() == 1 ? strB.prepend("0") : strB));
	m_pSetting->ui.colorEdit->setText(strRgb);
}


void ColorDialog::colorItemSelcSlot(const QColor &c)
{
    //qDebug()<<"colorItemSelecSlot";
	m_bNotEdit = true;

	m_pSetting->ui.hColorWgt->setHue(c.hue());
	m_pSetting->ui.svColorWgt->setColor(c);

	m_bNotEdit = false;
}

void ColorDialog::addCustomColorSlot()
{
	QColor color;
	color.setHsv(m_pSetting->m_iHue, m_pSetting->m_iSaturation, m_pSetting->m_iBrightness);
	m_pSetting->ui.customColorWgt->setGivenColor(color);
}

void ColorDialog::okBtnClickedSlot()
{
	QColor color;
    uint8_t fun = 0;
    uint8_t bright = 0;
    uint8_t fun_bright=0;
    uint32_t id = 0;

	color.setHsv(m_pSetting->m_iHue, m_pSetting->m_iSaturation, m_pSetting->m_iBrightness);
	m_pSetting->ui.previewWgt->setCurColor(color);

    fun = m_btnGroup->checkedId();
    bright = m_pSetting->ui.hSlider_level->value();

    fun_bright = (fun << 5)| bright;

    QListWidget *lsw = m_pSetting->ui.listWdt_id;
    if(lsw->item(0)->isSelected())
    {
        id = 0xFFFFFF;
    }else
    {
        for(int i=1;i < lsw->count();i++)
        {
            if(lsw->item(i)->isSelected())
            {
                id |= (((uint32_t)1)<<(i-1));
            }
        }
    }
    qDebug()<<"fun: "<<fun<<"bright: "<<bright;
    qDebug("id %x",id);

    emit colorSelect(color,fun_bright,id);

//	m_buttonRole = Yes;
//	if (m_pEvtLoop != NULL)
//	{
//		m_pEvtLoop->exit();
//	}
}

void ColorDialog::cancelBtnClickedSlot()
{
//    m_buttonRole = No;
    emit cancelBtnsignal();
//	if (m_pEvtLoop != NULL)
//	{
//		m_pEvtLoop->exit();
//    }
}

void ColorDialog::closeBtnClickedSlot()
{
  close();
}

void ColorDialog::editChangedSlot(const QString &strColor)
{
    qDebug()<<"editChangeSlot";
	m_bNotEdit = true;

	int r, g, b;
	switch (strColor.length())
	{
	case 0:
	{
		r = g = b = 0;
	}
		break;
	case 1:
	case 2:
	{
		r = g = 0;
		bool ok;
		b = strColor.toInt(&ok, 16);
	}
		break;
	case 3:
	{
		QString strR = QString("%1%2").arg(strColor.left(1), strColor.left(1));
		QString strG = QString("%1%2").arg(strColor.mid(1, 1), strColor.mid(1, 1));
		QString strB = QString("%1%2").arg(strColor.right(1), strColor.right(1));
		bool ok;
		r = strR.toInt(&ok, 16);
		g = strG.toInt(&ok, 16);
		b = strB.toInt(&ok, 16);
	}
		break;
	case 4:
	{
		r = 0;
		QString strG = QString("%1").arg(strColor.left(2));
		QString strB = QString("%1").arg(strColor.right(2));
		bool ok;
		g = strG.toInt(&ok, 16);
		b = strB.toInt(&ok, 16);
	}
		break;
	case 5:
	{
		QString strR = QString("%1").arg(strColor.left(1));
		QString strG = QString("%1").arg(strColor.mid(1, 2));
		QString strB = QString("%1").arg(strColor.right(2));
		bool ok;
		r = strR.toInt(&ok, 16);
		g = strG.toInt(&ok, 16);
		b = strB.toInt(&ok, 16);
	}
		break;
	case 6:
	{
		QString strR = QString("%1").arg(strColor.left(2));
		QString strG = QString("%1").arg(strColor.mid(2, 2));
		QString strB = QString("%1").arg(strColor.right(2));
		bool ok;
		r = strR.toInt(&ok, 16);
		g = strG.toInt(&ok, 16);
		b = strB.toInt(&ok, 16);
	}
		break;
	default:
		break;
	}

	QColor color;
	color.setRgb(r, g, b);
	int h, s, v;
	h = m_pSetting->m_iHue = color.hue();
	s = m_pSetting->m_iSaturation = color.saturation();
	v = m_pSetting->m_iBrightness = color.value();
	m_pSetting->ui.hColorWgt->setHue(h);
	m_pSetting->ui.svColorWgt->setHsv(h, s, v);
	m_pSetting->ui.previewWgt->setNewColor(color);

	m_pSetting->ui.hSpinBox->setValue(h);
	m_pSetting->ui.sSpinBox->setValue(s);
	m_pSetting->ui.vSpinBox->setValue(v);
	m_pSetting->ui.rSpinBox->setValue(r);
	m_pSetting->ui.gSpinBox->setValue(g);
	m_pSetting->ui.bSpinBox->setValue(b);

	m_bNotEdit = false;
}

void ColorDialog::editFinishedSlot()
{
	QString strColor = m_pSetting->ui.colorEdit->text();
	switch (strColor.length())
	{
	case 0:
	{
		m_pSetting->ui.colorEdit->setText("000000");
	}
		break;
	case 1:
	{
		m_pSetting->ui.colorEdit->setText(strColor.prepend("00000"));
	}
		break;
	case 2:
	{
		m_pSetting->ui.colorEdit->setText(strColor.prepend("0000"));
	}
		break;
	case 3:
	{
		QString strR = QString("%1%2").arg(strColor.left(1), strColor.left(1));
		QString strG = QString("%1%2").arg(strColor.mid(1, 1), strColor.mid(1, 1));
		QString strB = QString("%1%2").arg(strColor.right(1), strColor.right(1));
		m_pSetting->ui.colorEdit->setText(strR.append(strG).append(strB));
	}
		break;
	case 4:
	{
		QString strG = QString("%1").arg(strColor.left(2));
		QString strB = QString("%1").arg(strColor.right(2));
		m_pSetting->ui.colorEdit->setText(strG.append(strB).prepend("00"));
	}
		break;
	case 5:
	{
		QString strR = QString("%1").arg(strColor.left(1));
		QString strG = QString("%1").arg(strColor.mid(1, 2));
		QString strB = QString("%1").arg(strColor.right(2));
		m_pSetting->ui.colorEdit->setText(strR.append(strG).append(strB).prepend("0"));
	}
		break;
	case 6:
	{
		return;
	}
		break;
	default:
		break;
	}
}

void ColorDialog::hValueChangedSlot(int h)
{
	if (m_bNotEdit)
	{
		return;
	}
	m_bNotEdit = true;

	m_pSetting->m_iHue = h;
	m_pSetting->ui.hColorWgt->setHue(h);
	QColor color;
	color.setHsv(h, m_pSetting->m_iSaturation, m_pSetting->m_iBrightness);
	m_pSetting->ui.svColorWgt->setHue(h);
	updateRGBColor(color);

	m_bNotEdit = false;
}

void ColorDialog::sValueChangedSlot(int s)
{
	if (m_bNotEdit)
	{
		return;
	}
	m_bNotEdit = true;

	m_pSetting->m_iSaturation = s;
	QColor color;
	color.setHsv(m_pSetting->m_iHue, s, m_pSetting->m_iBrightness);
	m_pSetting->ui.svColorWgt->setSaturation(s);
	updateRGBColor(color);

	m_bNotEdit = false;
}

void ColorDialog::vValueChangedSlot(int v)
{
	if (m_bNotEdit)
	{
		return;
	}
	m_bNotEdit = true;

	m_pSetting->m_iBrightness = v;
	QColor color;
	color.setHsv(m_pSetting->m_iHue, m_pSetting->m_iSaturation, v);
	m_pSetting->ui.svColorWgt->setBrightness(v);
	updateRGBColor(color);

	m_bNotEdit = false;
}

void ColorDialog::rValueChangedSlot(int r)
{
	if (m_bNotEdit)
	{
		return;
	}
	m_bNotEdit = true;

	int iGreen = m_pSetting->ui.gSpinBox->value();
	int iBlue = m_pSetting->ui.bSpinBox->value();
	QColor color;
	color.setRgb(r, iGreen, iBlue);
	int h = m_pSetting->m_iHue = color.hue();
	int s = m_pSetting->m_iSaturation = color.saturation();
	int v = m_pSetting->m_iBrightness = color.value();

	m_pSetting->ui.hColorWgt->setHue(h);
	m_pSetting->ui.svColorWgt->setHsv(h, s, v);
	m_pSetting->ui.previewWgt->setNewColor(color);

	m_pSetting->ui.hSpinBox->setValue(h);
	m_pSetting->ui.sSpinBox->setValue(s);
	m_pSetting->ui.vSpinBox->setValue(v);

	QString strR = QString::number(r, 16);
	QString strG = QString::number(iGreen, 16);
	QString strB = QString::number(iBlue, 16);
	QString strRgb = QString("%1%2%3").arg(QString("%1").arg(strR.size() == 1 ? strR.prepend("0") : strR),
		QString("%1").arg(strG.size() == 1 ? strG.prepend("0") : strG),
		QString("%1").arg(strB.size() == 1 ? strB.prepend("0") : strB));
	m_pSetting->ui.colorEdit->setText(strRgb);

	m_bNotEdit = false;
}

void ColorDialog::gValueChangedSlot(int g)
{
	if (m_bNotEdit)
	{
		return;
	}
	m_bNotEdit = true;

	int iRed = m_pSetting->ui.rSpinBox->value();
	int iBlue = m_pSetting->ui.bSpinBox->value();
	QColor color;
	color.setRgb(iRed, g, iBlue);
	int h = m_pSetting->m_iHue = color.hue();
	int s = m_pSetting->m_iSaturation = color.saturation();
	int v = m_pSetting->m_iBrightness = color.value();

	m_pSetting->ui.hColorWgt->setHue(h);
	m_pSetting->ui.svColorWgt->setHsv(h, s, v);
	m_pSetting->ui.previewWgt->setNewColor(color);

	m_pSetting->ui.hSpinBox->setValue(h);
	m_pSetting->ui.sSpinBox->setValue(s);
	m_pSetting->ui.vSpinBox->setValue(v);

	QString strR = QString::number(iRed, 16);
	QString strG = QString::number(g, 16);
	QString strB = QString::number(iBlue, 16);
	QString strRgb = QString("%1%2%3").arg(QString("%1").arg(strR.size() == 1 ? strR.prepend("0") : strR),
		QString("%1").arg(strG.size() == 1 ? strG.prepend("0") : strG),
		QString("%1").arg(strB.size() == 1 ? strB.prepend("0") : strB));
	m_pSetting->ui.colorEdit->setText(strRgb);

	m_bNotEdit = false;
}

void ColorDialog::bValueChangedSlot(int b)
{
	if (m_bNotEdit)
	{
		return;
	}
	m_bNotEdit = true;

	int iRed = m_pSetting->ui.rSpinBox->value();
	int iGreen = m_pSetting->ui.gSpinBox->value();
	QColor color;
	color.setRgb(iRed, iGreen, b);
	int h = m_pSetting->m_iHue = color.hue();
	int s = m_pSetting->m_iSaturation = color.saturation();
	int v = m_pSetting->m_iBrightness = color.value();

	m_pSetting->ui.hColorWgt->setHue(h);
	m_pSetting->ui.svColorWgt->setHsv(h, s, v);
	m_pSetting->ui.previewWgt->setNewColor(color);

    m_pSetting->ui.hSpinBox->setValue(h);
	m_pSetting->ui.sSpinBox->setValue(s);
	m_pSetting->ui.vSpinBox->setValue(v);

	QString strR = QString::number(iRed, 16);
	QString strG = QString::number(iGreen, 16);
	QString strB = QString::number(b, 16);
	QString strRgb = QString("%1%2%3").arg(QString("%1").arg(strR.size() == 1 ? strR.prepend("0") : strR),
		QString("%1").arg(strG.size() == 1 ? strG.prepend("0") : strG),
		QString("%1").arg(strB.size() == 1 ? strB.prepend("0") : strB));
	m_pSetting->ui.colorEdit->setText(strRgb);

	m_bNotEdit = false;
}

void ColorDialog::updateEditData(int h, int s, int v)
{
    //qDebug()<<"updateEditData";
	m_bNotEdit = true;

	m_pSetting->m_iHue = h;
	m_pSetting->m_iSaturation = s;
	m_pSetting->m_iBrightness = v;
	QColor color;
	color.setHsv(h, s, v);
	m_pSetting->ui.hSpinBox->setValue(h);
	m_pSetting->ui.sSpinBox->setValue(s);
	m_pSetting->ui.vSpinBox->setValue(v);
	m_pSetting->ui.rSpinBox->setValue(color.red());
	m_pSetting->ui.gSpinBox->setValue(color.green());
	m_pSetting->ui.bSpinBox->setValue(color.blue());

	QString strR = QString::number(color.red(), 16);
	QString strG = QString::number(color.green(), 16);
	QString strB = QString::number(color.blue(), 16);
	QString strRgb = QString("%1%2%3").arg(QString("%1").arg(strR.size() == 1 ? strR.prepend("0") : strR),
		QString("%1").arg(strG.size() == 1 ? strG.prepend("0") : strG),
		QString("%1").arg(strB.size() == 1 ? strB.prepend("0") : strB));
	m_pSetting->ui.colorEdit->setText(strRgb);

    m_bNotEdit = false;
}

void ColorDialog::listWdtItemPressSlot(QListWidgetItem *item)
{
    (void)item;
    QListWidget *plw = m_pSetting->ui.listWdt_id;
    int count = plw->count();

    if(plw->currentRow() == 0)
    {
        bool sel = plw->item(0)->isSelected();
        for(int i = 1;i < count;i++)
        {
            plw->item(i)->setSelected(sel);
        }
    }else
    {
        int j = 1;
        for(j = 1;j < count;j++)
        {
            if(!plw->item(j)->isSelected())
            {
                break;
            }
        }
        if(j == count)
        {
            plw->item(0)->setSelected(true);
        }else
        {
            plw->item(0)->setSelected(false);
        }
    }
}


void ColorDialog::checkBoxstateChanged(int arg1)
{
    if(arg1 == 2)
    {
        autoUpdateTimer->start(10);
        m_pSetting->ui.okBtn->setEnabled(false);
    }else if(arg1 == 0)
    {
        m_pSetting->ui.okBtn->setEnabled(true);
        autoUpdateTimer->stop();
    }
}

void ColorDialog::autoUpdateTimerout()
{
    static int hue=0,saturation=0,bright=0;

    if(hue != m_pSetting->m_iHue || saturation != m_pSetting->m_iBrightness
            || bright != m_pSetting->m_iSaturation)
    {
        hue = m_pSetting->m_iHue;
        saturation = m_pSetting->m_iBrightness;
        bright = m_pSetting->m_iSaturation;
        okBtnClickedSlot();
    }
}
