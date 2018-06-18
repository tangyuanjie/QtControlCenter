#include "QGetWidget.h"
#include <QDebug>
QGetWidget::QGetWidget(QWidget *parent, WIDGET_TYPE types)
	: QDialog(parent)
{
	this->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint );
	this->setWindowIcon(QIcon(":/Image/Resources/Image/zhihuiwu.ico"));
	Init(types);

	connect(btn_back, SIGNAL(clicked()), this, SLOT(on_btnBack_clicked()));
	connect(btn_home, SIGNAL(clicked()), this, SLOT(on_btnHome_clicked()));
	
	connect(btn_NumGet, SIGNAL(clicked()), this, SLOT(on_btnNumGet_clicked()));
	connect(btn_QRGet, SIGNAL(clicked()), this, SLOT(on_btnQRGet_clicked()));
	connect(btn_NumSave, SIGNAL(clicked()), this, SLOT(on_btnNumSave_clicked()));
	connect(btn_QRSave, SIGNAL(clicked()), this, SLOT(on_btnQRSave_clicked()));

	connect(gblPortControl, SIGNAL(sig_OpendoorFinish()), this, SLOT(on_slot_OpendoorFinish()));
	

}

QGetWidget::~QGetWidget()
{
}


void QGetWidget::Init(WIDGET_TYPE types)
{
	widget_Save->hide();
	widget_get->hide();
	label_title->hide();
	if (types & MSGBOX_GET)
	{
		widget_get->show();
	}
	if (types & MSGBOX_SAVE)
	{
		widget_Save->show();
	}

}


void QGetWidget::on_btnNumGet_clicked()
{
	NumClick(WIDGET_TYPE::MSGBOX_GET);
}
void QGetWidget::on_btnQRGet_clicked()
{
	QRClick(WIDGET_TYPE::MSGBOX_GET);
}
void QGetWidget::on_btnNumSave_clicked()
{
	NumClick(WIDGET_TYPE::MSGBOX_SAVE);
}
void QGetWidget::on_btnQRSave_clicked()
{
	QRClick(WIDGET_TYPE::MSGBOX_SAVE);
}

void QGetWidget::QRClick(WIDGET_TYPE types)
{
	gblRuntimeData->m_type = types;

	m_pQRWidget.reset(new CQRWidget);
	m_pQRWidget->Init(types);
	m_pQRWidget->showMaximized();
	int res = m_pQRWidget->exec();
	if (res == WIDGET_TYPE::MSGBOX_BACK)
	{
		this->show();
	}
	else if (res == WIDGET_TYPE::MSGBOX_HOME)
	{
		done(MSGBOX_HOME);
	}
	else
	{
		int nInputNum = res;
		MessageBoxShow(nInputNum, types);
	}

}

void QGetWidget::NumClick(WIDGET_TYPE types)
{
	gblRuntimeData->m_type = types;
	m_pNumKeyWidget.reset(new NumKeyBoard);
	m_pNumKeyWidget->Init(types);
	m_pNumKeyWidget->showMaximized();
	int res = m_pNumKeyWidget->exec();
	if (res == WIDGET_TYPE::MSGBOX_BACK)
	{
		this->show();
		return;
	}
	else if (res == WIDGET_TYPE::MSGBOX_HOME)
	{
		done(MSGBOX_HOME);
		return;
	}
	else
	{
		int nInputNum = res;
		MessageBoxShow(nInputNum, types);
	}

}
void  QGetWidget::MessageBoxShow(int InputNum, WIDGET_TYPE types)
{
	m_nInputNum = InputNum;
	m_type = types;
	int nRes;
	if (types == WIDGET_TYPE::MSGBOX_SAVE)
	{
		int nRes = CMessage::WaitSave();
		if (nRes == WIDGET_TYPE::MSGBOX_BACK)
		{
			this->show();
			return;
		}
		else if (nRes == WIDGET_TYPE::MSGBOX_HOME)
		{
			done(WIDGET_TYPE::MSGBOX_HOME);
			return;
		}
		m_pProgressWidget.reset(new CProgressWidget);
		bool bres = m_pProgressWidget->setInputNum(gblRuntimeData->InputNum, gblRuntimeData->m_type);
		if (bres == false)
		{
			done(WIDGET_TYPE::MSGBOX_HOME);
			return;
		}
		m_pProgressWidget->showMaximized();
		int nResult = m_pProgressWidget->exec();
		nRes = CMessage::SaveEnd();

	}
	else
	{
		m_pProgressWidget.reset(new CProgressWidget);
		m_pProgressWidget->showMaximized();
		m_pProgressWidget->setInputNum(InputNum,types);
		int nResult = m_pProgressWidget->exec();
		nRes = CMessage::GetEnd();
		if (nRes == WIDGET_TYPE::MSGBOX_RETURN)
		{
			goto next;
		}
		else
		{		

			done(WIDGET_TYPE::MSGBOX_HOME);
			return;
		}
		
	next:
		nRes = CMessage::WaitGet();
		if (nRes == WIDGET_TYPE::MSGBOX_BACK)
		{
			this->show();
			return;
		}
		else if (nRes == WIDGET_TYPE::MSGBOX_HOME)
		{
			done(WIDGET_TYPE::MSGBOX_HOME);
			return;
		}
	}
	done(WIDGET_TYPE::MSGBOX_HOME);
}


void QGetWidget::on_slot_OpendoorFinish()
{
	
}




void QGetWidget::on_btnBack_clicked()
{
	done(MSGBOX_BACK);
}
void QGetWidget::on_btnHome_clicked()
{
	done(MSGBOX_HOME);
}