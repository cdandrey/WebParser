/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   parser.cpp
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#include "cparser.h"

#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QRegularExpressionMatchIterator>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>


//-----------------------------------------------------------------------------------------


const QRegularExpression CParser::C_URL_REG_EXP               {"((?:href=\"http?\\S+\"))"};
const int                CParser::C_URL_BEGIN_POS             {QString{"href=\""}.length()};
const int                CParser::C_URL_CORRECT_LEN           {C_URL_BEGIN_POS + QString{"\""}.length()};
const int                CParser::C_FIRST_DELAY_RETRY_REQUEST {1000} ;   // 1 seconds
const int                CParser::C_LAST_DELAY_RETRY_REQUEST  {1000};    // 1 seconds
const int                CParser::C_WAIT_TASK                 {5000};    // 5 seconds
//-----------------------------------------------------------------------------------------


CParser::CParser(const QByteArray& txt, int maxUrls, QObject *parent)
    : QObject{parent}
    , m_text{txt}
    , m_networkManager{new QNetworkAccessManager(this)}
    , m_delayRetryRequest{C_FIRST_DELAY_RETRY_REQUEST}
    , m_maxUrls{maxUrls}
    , m_stop{false}
    , m_pause{false}
    , m_urlParsed{true}

{
    connect(m_networkManager,&QNetworkAccessManager::finished,
            this            ,&CParser::in_load);
}
//-----------------------------------------------------------------------------------------


void CParser::in_recive(Qt::HANDLE hndl, QUrl url, bool p)
{
    if (m_stop)
        return;

    m_urlParsed = p;

    if (hndl == QThread::currentThreadId() && !url.isEmpty())
    {
        m_url = url;

        emit out_update(m_url,WPt::C_DOWNLOADING);
        m_networkManager->get(QNetworkRequest(m_url));
    }
    else if (hndl == QThread::currentThreadId() && url.isEmpty())
    {
        QTimer::singleShot(C_WAIT_TASK,this,SLOT(in_start()));
    }
}
//-----------------------------------------------------------------------------------------


void CParser::in_start()
{
    if (m_stop)
        return;

//    while (m_pause)
//    {
//        QMutex m;
//        m.lock();
//        m.tryLock(C_WAIT_TASK);
//        m.unlock();
//    }

    emit out_take(QThread::currentThreadId());
}
//-----------------------------------------------------------------------------------------


void CParser::parsing(const QByteArray& page)
{
    if (m_urlParsed)
    {
        QString str;
        QUrl u;
        QVector<QUrl> urls;
        urls.reserve(m_maxUrls);

        QRegularExpressionMatchIterator it = C_URL_REG_EXP.globalMatch(page);

        while (it.hasNext() && !m_stop)
        {
            str = it.next().captured(1);
            u = QUrl(str.mid(C_URL_BEGIN_POS,str.length() - C_URL_CORRECT_LEN));

            if (u.isValid() && urls.size() < m_maxUrls)
                urls << u;
        }

        if (m_stop)
            return;

        emit out_add(urls);

    }

    if (m_stop)
        return;

    if (page.indexOf(m_text) != -1)
        emit out_update(m_url,WPt::C_FOUND);
    else
        emit out_update(m_url,WPt::C_NOT_FOUND);

    emit out_take(QThread::currentThreadId());
}
//-----------------------------------------------------------------------------------------


void CParser::in_load(QNetworkReply *reply)
{
    if (m_stop)
    {
        reply->deleteLater();
        return;
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        if (m_delayRetryRequest < C_LAST_DELAY_RETRY_REQUEST)
        {
            QTimer::singleShot(m_delayRetryRequest,this,SLOT(in_retry_request()));

            m_delayRetryRequest <<= 1;
        }
        else
        {
            emit out_update(m_url,WPt::C_DOWNLOADING_ERROR);
            QTimer::singleShot(C_WAIT_TASK,this,SLOT(in_start()));
        }
    }
    else
    {
        emit out_update(m_url,WPt::C_FINDING);

        parsing(reply->readAll());
    }

    reply->deleteLater();
}
//-----------------------------------------------------------------------------------------


void CParser::in_retry_request()
{
    if (m_stop)
        return;

    if (m_url.isValid())
        m_networkManager->get(QNetworkRequest(m_url));
    else if (!m_url.isEmpty())
        emit out_update(m_url,WPt::C_DOWNLOADING_ERROR);
}
//-----------------------------------------------------------------------------------------

