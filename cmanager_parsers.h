/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cmanager_parsers.h
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#ifndef CMANAGERPARSERS_H
#define CMANAGERPARSERS_H

#include "cwebparser_types.h"

#include <QtCore/QAtomicInt>
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE
class CParser;
QT_END_NAMESPACE

//-----------------------------------------------------------------------------------------

class CManagerParsers : public QObject
{
    Q_OBJECT
public:
    explicit CManagerParsers(QObject *parent = 0);

signals:

    void out_add(QVector<QUrl>);
    void out_completed();
    void out_finished();
    void out_start(QUrl);
    void out_stop();
    void out_pause(bool);

    void out_send(Qt::HANDLE,QUrl,bool);    // pool tasks responds on the request: URL of task
    void out_take(Qt::HANDLE);              // request take task from pool tasks
    void out_update(QUrl,int);              // request update state of task by URL

public slots:

    void in_set_param(const WPt::CParam *p);
    void in_start(QUrl,QByteArray);         // start parsing: begining URL, source TEXT

private:

    int m_paramMaxThrds;
    int m_paramMaxUrls;

    QVector<CParser*> m_parsers;
    QVector<QThread*> m_thrds;

    QAtomicInt m_cntFinished;

private slots:

    void in_finished();
};

#endif // CMANAGERPARSERS_H
