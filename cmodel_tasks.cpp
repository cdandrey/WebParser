/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cmodel_tasks.cpp
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#include "cmodel_tasks.h"

#include <QtCore/QModelIndex>
#include <QtCore/QReadWriteLock>

//---------------------------------------------------------------

const int CModelTasks::C_COLUMN_COUNT {3};

const QVector<QString> CModelTasks::C_TITLE_HEADER {
    QObject::tr(""),
    QObject::tr("Url"),
    QObject::tr("State")
};
//---------------------------------------------------------------


CModelTasks::CModelTasks(QObject *parent)
    : QAbstractTableModel{parent}
    , m_pool{new CTasksPool{WPt::C_DEFAULT_NUM_URLS}}
{
}
//---------------------------------------------------------------


CModelTasks::~CModelTasks()
{
    delete m_pool;
}
//---------------------------------------------------------------


int CModelTasks::rowCount(const QModelIndex &/*parent*/) const
{
    return m_pool->countService();
}
//---------------------------------------------------------------


int CModelTasks::columnCount(const QModelIndex &/*parent*/) const
{
    return C_COLUMN_COUNT;
}
//---------------------------------------------------------------


QVariant CModelTasks::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case 0: return index.row() + 1;
            case 1: return m_pool->url(index.row());
            case 2: return m_pool->state(index.row());
            default: return QVariant{};
        }
    }

    return QVariant{};
}
//---------------------------------------------------------------


Qt::ItemFlags CModelTasks::flags(const QModelIndex &) const
{
    return  Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
//---------------------------------------------------------------


QVariant CModelTasks::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::DisplayRole )
    {
        if( orientation == Qt::Vertical )
            return section;

        if (orientation == Qt::Horizontal && section < C_TITLE_HEADER.size())
            return C_TITLE_HEADER.at(section);
    }

    return QVariant{};
}
//---------------------------------------------------------------


void CModelTasks::in_add(const QVector<QUrl> &urls)
{
    QWriteLocker lock(&m_lock);

    if (!m_pool->isFull())
    {
        foreach (const QUrl &u, urls)
            if (!m_pool->append(u))
                break;
    }
}
//---------------------------------------------------------------


void CModelTasks::in_set_param(const WPt::CParam *p)
{
    if (m_pool->isFull())
    {
        beginRemoveRows(QModelIndex(),0,m_pool->countService() - 1);
        m_pool->resize(p->maxUrls);
        endRemoveRows();
    }
    else
    {
        m_pool->resize(p->maxUrls);
    }
}
//---------------------------------------------------------------


void CModelTasks::in_start(QUrl u)
{
    m_pool->append(u);
}
//---------------------------------------------------------------


void CModelTasks::in_take(Qt::HANDLE hndl)
{
    if (m_pool->isEmpty() || !m_pool->hasTake())
    {
        emit out_send(hndl,QUrl{},!m_pool->isFull());
    }
    else
    {
        int row { m_pool->countService() };
        beginInsertRows(QModelIndex(),row, row);
        endInsertRows();

        emit out_send(hndl,m_pool->takeFirst(),!m_pool->isFull());
    }
}
//---------------------------------------------------------------


void CModelTasks::in_update(QUrl u, int state)
{
    QReadLocker lock(&m_lock);

    m_pool->setState(u,state);

    if (m_pool->isEmpty())
        emit out_completed();
}
//---------------------------------------------------------------


