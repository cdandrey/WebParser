/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cmanager_parsers.cpp
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#include "cmanager_parsers.h"
#include "cparser.h"

#include <QtCore/QThread>
//---------------------------------------------------------------


CManagerParsers::CManagerParsers(QObject *parent)
    : QObject(parent)
    , m_paramMaxThrds{WPt::C_DEFAULT_NUM_THRDS}
    , m_paramMaxUrls{WPt::C_DEFAULT_NUM_URLS}
    , m_cntFinished{QAtomicInt{0}}
{
    qRegisterMetaType<Qt::HANDLE>("Qt::HANDLE");
}
//---------------------------------------------------------------


void CManagerParsers::in_set_param(const WPt::CParam *p)
{
    m_paramMaxThrds = p->maxThreads;
    m_paramMaxUrls  = p->maxUrls;
}
//---------------------------------------------------------------


void CManagerParsers::in_start(QUrl url,QByteArray txt)
{
    emit out_start(url);

    m_cntFinished.store(0);

    m_parsers = QVector<CParser*>(m_paramMaxThrds);
    m_thrds = QVector<QThread*>(m_paramMaxThrds);

    for (int i = 0; i < m_paramMaxThrds; ++i)
    {
        // create parser
        m_parsers[i] = new CParser(txt,m_paramMaxUrls);

        connect(m_parsers.at(i),&CParser::out_take,
                this,&CManagerParsers::out_take,Qt::QueuedConnection);

        connect(m_parsers.at(i),&CParser::out_update,
                this,&CManagerParsers::out_update,Qt::QueuedConnection);

        connect(m_parsers.at(i),&CParser::out_add,
                this,&CManagerParsers::out_add,Qt::QueuedConnection);

        connect(this,&CManagerParsers::out_send,
                m_parsers.at(i),&CParser::in_recive,Qt::QueuedConnection);

        connect(this,&CManagerParsers::out_pause,
                m_parsers.at(i),&CParser::in_pause,Qt::QueuedConnection);

        // create thread

        m_thrds[i] = new QThread(this);

        m_parsers.at(i)->moveToThread(m_thrds.at(i));

        connect(m_thrds.at(i),&QThread::started,
                m_parsers.at(i),&CParser::in_start,Qt::QueuedConnection);

        connect(this,&CManagerParsers::out_completed,
                m_thrds.at(i),&QThread::quit,Qt::QueuedConnection);

        connect(this,&CManagerParsers::out_stop,
                m_thrds.at(i),&QThread::quit,Qt::QueuedConnection);

        connect(m_thrds.at(i),&QThread::finished,
                m_parsers.at(i),&CParser::deleteLater,Qt::QueuedConnection);

        connect(m_thrds.at(i),&QThread::finished,
                m_thrds.at(i),&QThread::deleteLater,Qt::QueuedConnection);

        connect(m_thrds.at(i),&QThread::destroyed,
                this,&CManagerParsers::in_finished,Qt::QueuedConnection);

        m_thrds.at(i)->start();
    }
}
//---------------------------------------------------------------


void CManagerParsers::in_finished()
{
    m_cntFinished.fetchAndAddOrdered(1);

    if (m_cntFinished.fetchAndAddOrdered(0) == m_paramMaxThrds)
        emit out_finished();
}
//---------------------------------------------------------------


