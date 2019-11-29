/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   ctasks_pool.h
/// Description:
/// Data       :   2019.11.29
///
/////////////////////////////////////////////////////////////////

#ifndef CTASKS_POOL_H
#define CTASKS_POOL_H

#include "cwebparser_types.h"

#include <QtCore/QAtomicInt>

class CTasksPool
{
public:
    explicit CTasksPool(int size);

    bool append(const QUrl&);
    void resize(int size);
    QString state(int) const;
    void setState(const QUrl&,int);
    int  countService() const { return m_headTake.fetchAndAddRelaxed(0); }
    QUrl takeFirst();
    QUrl url(int i) const;
    bool isEmpty() const { return m_headService.fetchAndAddOrdered(0) == m_tail.fetchAndAddOrdered(0); }
    bool isFull() const { return m_table.size() == m_list.size(); }
    bool hasTake() const { return m_table.size() > m_headTake.fetchAndAddOrdered(0); }

private:

    WPt::QVectorUrl m_list;
    WPt::QHashUrl m_table;

    mutable QAtomicInt m_headTake;
    mutable QAtomicInt m_headService;
    mutable QAtomicInt m_tail;

    static const QHash<int,QString> C_STATE;
};

#endif // CTASKS_POOL_H
