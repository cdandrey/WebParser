/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cmodel_tasks.h
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#ifndef CMODELTASKS_H
#define CMODELTASKS_H

#include "ctasks_pool.h"

#include <QtCore/QAbstractTableModel>
#include <QtCore/QReadWriteLock>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QUrl>

//---------------------------------------------------------------


class CModelTasks : public QAbstractTableModel
{
    Q_OBJECT
public:

    explicit CModelTasks(QObject *parent = 0);
    ~CModelTasks();

    int           rowCount(const QModelIndex &parent = QModelIndex()) const;
    int           columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant      data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &) const ;
    QVariant      headerData(int section, Qt::Orientation orientation, int role) const;

signals:

    void out_completed();
    void out_send(Qt::HANDLE,QUrl,bool);

public slots:

    void in_add(const QVector<QUrl>&);
    void in_set_param(const WPt::CParam *p);
    void in_start(QUrl);
    void in_take(Qt::HANDLE);
    void in_update(QUrl, int);

private:

    CTasksPool *m_pool;

    mutable QReadWriteLock m_lock;

    static const int                C_COLUMN_COUNT;
    static const QVector<QString>   C_TITLE_HEADER;

};
//---------------------------------------------------------------


#endif // CMODELTASKS_H
