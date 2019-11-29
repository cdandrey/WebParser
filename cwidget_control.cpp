/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cwidget_control.cpp
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#include "cwidget_control.h"

#include <QtCore/QThread>
#include <QtWidgets/QAction>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
//---------------------------------------------------------------


const QString CWidgetControl::C_CAPTION_LABEL_TITLE_URL {QObject::tr("Url:")                        };
const QString CWidgetControl::C_CAPTION_ACTION_FIND     {QObject::tr("Find source test on the url.")};
const QString CWidgetControl::C_CAPTION_ACTION_PAUSE    {QObject::tr("To do pause of find.")        };
const QString CWidgetControl::C_CAPTION_ACTION_STOP     {QObject::tr("To do stop of find.")         };
const QString CWidgetControl::C_CAPTION_ACTION_SETTINGS {QObject::tr("Set settings of find.")       };
const QString CWidgetControl::C_HELP_EDIT_TEXT          {QObject::tr("Source text for search")      };
const int     CWidgetControl::C_MARGIN_EDIT_TEXT        {10                                         };
const int     CWidgetControl::C_MAXBLOCK_EDIT_TEXT      {1000                                       };
//---------------------------------------------------------------


CWidgetControl::CWidgetControl(QWidget *parent)
    : QWidget(parent)
    , m_pause{false}
{
    qRegisterMetaType<WPt::CParam>("WPt::CParam");

    m_toolCommand = new QToolBar(this);
    m_toolCommand->setMovable(false);

    QLabel *labelTitleUrl = new QLabel(C_CAPTION_LABEL_TITLE_URL,this);
    QLabel *labelSpcUrl = new QLabel(this);
    QLabel *labelSpcOption = new QLabel(this);

    m_lineUrl = new QLineEdit(this);
    m_lineUrl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_lineUrl->setText("https://stackoverflow.com");

    m_actFind     = new QAction(QIcon(":/ico/ctrl_play.png"    ),C_CAPTION_ACTION_FIND,this);
    m_actPause    = new QAction(QIcon(":/ico/ctrl_pause.png"   ),C_CAPTION_ACTION_PAUSE,this);
    m_actStop     = new QAction(QIcon(":/ico/ctrl_stop.png"    ),C_CAPTION_ACTION_STOP,this);
    m_actSettings = new QAction(QIcon(":/ico/ctrl_settings.png"),C_CAPTION_ACTION_SETTINGS,this);

    m_actFind->setVisible(true);
    m_actPause->setVisible(false);
    m_actStop->setVisible(true);
    m_actSettings->setVisible(true);

    m_actFind->setEnabled(true);
    m_actPause->setEnabled(false);
    m_actStop->setEnabled(false);
    m_actSettings->setEnabled(true);
    m_actSettings->setCheckable(true);

    m_toolCommand->addWidget(labelTitleUrl);
    m_toolCommand->addWidget(m_lineUrl);
    m_toolCommand->addWidget(labelSpcUrl);
    m_toolCommand->addAction(m_actFind);
    m_toolCommand->addAction(m_actPause);
    m_toolCommand->addAction(m_actStop);
    m_toolCommand->addAction(m_actSettings);
    m_toolCommand->addWidget(labelSpcOption);

    m_editText = new QPlainTextEdit(this);
    m_editText->document()->setDocumentMargin(C_MARGIN_EDIT_TEXT);
    m_editText->setPlainText(C_HELP_EDIT_TEXT);
    m_editText->setMaximumBlockCount(C_MAXBLOCK_EDIT_TEXT);

    // create settings widget
    QWidget *wdgtSettings = new QWidget(this);

    QLabel *lblMaxThrds = new QLabel(wdgtSettings);
    lblMaxThrds->setText(QObject::tr("Maximum number of threads for scan"));

    QLabel *lblMaxUrls = new QLabel(wdgtSettings);
    lblMaxUrls->setText(QObject::tr("Maximum number of URLs scanned"));

    m_lineMaxThrds = new QLineEdit(wdgtSettings);
    m_lineMaxThrds->setText(QString::number(QThread::idealThreadCount()));
    m_lineMaxThrds->setMaximumWidth(100);

    m_lineMaxUrls = new QLineEdit(wdgtSettings);
    m_lineMaxUrls->setText("100");
    m_lineMaxUrls->setMaximumWidth(100);

    QVBoxLayout *vboxSettings = new QVBoxLayout(wdgtSettings);
    vboxSettings->addWidget(lblMaxThrds);
    vboxSettings->addWidget(m_lineMaxThrds);
    vboxSettings->addWidget(lblMaxUrls);
    vboxSettings->addWidget(m_lineMaxUrls);
    wdgtSettings->setLayout(vboxSettings);
    wdgtSettings->setVisible(false);

    // main compaund
    QSplitter *spl = new QSplitter(Qt::Horizontal,this);
    spl->addWidget(m_editText);
    spl->addWidget(wdgtSettings);
    spl->setStretchFactor(0,100);

    QVBoxLayout *vbox = new QVBoxLayout(this);

    vbox->setMenuBar(m_toolCommand);
    vbox->addWidget(spl);
    vbox->setMargin(0);

    m_param.reset(new WPt::CParam{m_lineMaxThrds->text().toInt(),m_lineMaxUrls->text().toInt()});

    connect(m_actFind,&QAction::triggered,this,&CWidgetControl::in_find);
    connect(m_actPause,&QAction::triggered,this,&CWidgetControl::in_pause);
    connect(m_actStop,&QAction::triggered,this,&CWidgetControl::in_stop);
    connect(m_actSettings,&QAction::toggled,wdgtSettings,&QWidget::setVisible);
}
//---------------------------------------------------------------


void CWidgetControl::in_reset()
{
    m_actFind->setVisible(true);
    m_actFind->setEnabled(true);

    m_actPause->setVisible(false);
    m_actPause->setEnabled(false);

    m_actStop->setEnabled(true);

    m_lineMaxThrds->setEnabled(true);
    m_lineMaxUrls->setEnabled(true);


    QMessageBox::information(this,"WebParser","Search is completed.",QMessageBox::Ok);
}
//---------------------------------------------------------------


void CWidgetControl::in_find(bool)
{
     QUrl url{m_lineUrl->text()};
     QByteArray text{m_editText->document()->toPlainText().toUtf8()};

    if (url.isValid() && !text.isEmpty())
    {
        m_actFind->setVisible(false);
        m_actFind->setEnabled(false);

        m_actPause->setVisible(true);
        m_actPause->setEnabled(true);

        m_actStop->setEnabled(true);

        if (m_lineMaxThrds->isEnabled())
        {
            bool ok;
            m_param.data()->maxThreads = m_lineMaxThrds->text().toInt(&ok);

            if (!ok)
            {
                m_lineMaxThrds->setText(QString::number(WPt::C_DEFAULT_NUM_THRDS));
                m_param.data()->maxThreads = WPt::C_DEFAULT_NUM_THRDS;
            }

            m_param.data()->maxUrls = m_lineMaxUrls->text().toInt(&ok);
            if (!ok)
            {
                m_lineMaxUrls->setText(QString::number(WPt::C_DEFAULT_NUM_URLS));
                m_param.data()->maxUrls = WPt::C_DEFAULT_NUM_URLS;
            }

            emit out_set_param(m_param.data());
            emit out_find(url,text);

            m_lineMaxThrds->setEnabled(false);
            m_lineMaxUrls->setEnabled(false);
        }
        else
        {
            emit out_pause(false);
        }
    }
    else
    {
        QMessageBox::information(this,"WebParser","Set URLs and text for search.",QMessageBox::Ok);
    }
}
//---------------------------------------------------------------


void CWidgetControl::in_pause(bool)
{
    m_actFind->setVisible(true);
    m_actFind->setEnabled(true);

    m_actPause->setVisible(false);
    m_actPause->setEnabled(false);

    m_actStop->setEnabled(true);

    emit out_pause(true);
}
//---------------------------------------------------------------


void CWidgetControl::in_stop(bool)
{
    m_actPause->setEnabled(false);
    m_actStop->setEnabled(false);

    m_lineMaxThrds->setEnabled(true);
    m_lineMaxUrls->setEnabled(true);

    emit out_stop();
}
//---------------------------------------------------------------


