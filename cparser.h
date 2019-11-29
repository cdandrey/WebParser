/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cparser.h
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#ifndef CPARSER_H
#define CPARSER_H

#include "cwebparser_types.h"

#include <QtCore/QObject>
#include <QtCore/QRegularExpression>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
QT_END_NAMESPACE

//-----------------------------------------------------------------------------------------


class CParser : public QObject
{
    Q_OBJECT
public:

    CParser(const QByteArray &,int maxUrls = WPt::C_DEFAULT_NUM_URLS, QObject *parent = 0);

signals:

    void out_add(QVector<QUrl>);
    void out_take(Qt::HANDLE);
    void out_update(QUrl,int);
    void out_wait(Qt::HANDLE);

public slots:

    void in_recive(Qt::HANDLE, QUrl,bool);

    void in_start();
    void in_pause(bool p) { m_pause = p;   }
    void in_stop()        { m_stop  = true;}


private:

    QByteArray             m_text;
    QUrl                   m_url;
    QNetworkAccessManager *m_networkManager;
    int                    m_delayRetryRequest;
    int                    m_maxUrls;

    bool m_stop;
    bool m_pause;
    bool m_urlParsed;

    void parsing(const QByteArray &page);

    static const QRegularExpression C_URL_REG_EXP              ;
    static const int                C_URL_BEGIN_POS            ;
    static const int                C_URL_CORRECT_LEN          ;
    static const int                C_FIRST_DELAY_RETRY_REQUEST;
    static const int                C_LAST_DELAY_RETRY_REQUEST ;
    static const int                C_WAIT_TASK                ;

private slots:

    void in_load(QNetworkReply*);
    void in_retry_request();

};

#endif // CPARSER_H
