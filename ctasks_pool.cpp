/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   ctasks_pool.cpp
/// Description:
/// Data       :   2019.11.29
///
/////////////////////////////////////////////////////////////////

#include "ctasks_pool.h"

const QHash<int,QString> CTasksPool::C_STATE {
    {WPt::C_WAITING          , ("Waiting for service ..."          )},
    {WPt::C_DOWNLOADING      , ("Downloading ..."                  )},
    {WPt::C_DOWNLOADING_ERROR, ("Download error."                  )},
    {WPt::C_FINDING          , ("Finding text ..."                 )},
    {WPt::C_FOUND            , ("Finding completed: text found."   )},
    {WPt::C_NOT_FOUND        , ("Finding completed: text not found")}
};
//---------------------------------------------------------------


CTasksPool::CTasksPool(int size)
    : m_headTake{QAtomicInt{0}}
    , m_headService{QAtomicInt{0}}
    , m_tail{QAtomicInt{size}}
{
    m_list = WPt::QVectorUrl(size);
    m_table.reserve(2*size);
}
//---------------------------------------------------------------


bool CTasksPool::append(const QUrl &u)
{
    if (isFull())
        return false;

    if (m_table.find(u) == m_table.end())
    {
        m_list[m_table.size()] = u;
        m_table.insert(u,WPt::C_WAITING);
        m_tail.fetchAndStoreOrdered(m_table.size());
    }

    return true;
}
//---------------------------------------------------------------


void CTasksPool::resize(int size)
{
    m_headTake.fetchAndStoreRelaxed(0);
    m_headService.fetchAndStoreRelaxed(0);
    m_tail.fetchAndAndRelaxed(size);

    m_list = WPt::QVectorUrl(size);
    m_table.clear();
    m_table.reserve(2*size);
}
//---------------------------------------------------------------


QString CTasksPool::state(int i) const
{
    if (i < m_list.size())
        return C_STATE[m_table.value(m_list.at(i))];

    return QString{""};
}
//---------------------------------------------------------------


void CTasksPool::setState(const QUrl &u, int state)
{
    if (m_table.find(u) != m_table.end())
    {
        m_table.operator [](u) = state;

        if (state == WPt::C_DOWNLOADING_ERROR ||
            state == WPt::C_NOT_FOUND         ||
            state == WPt::C_FOUND               )
        {
            m_headService.fetchAndAddOrdered(1);
        }
    }
}
//---------------------------------------------------------------


QUrl CTasksPool::url(int i) const
{
    if (i < m_list.size())
        return m_list.at(i);

    return QUrl{};
}
//---------------------------------------------------------------


QUrl CTasksPool::takeFirst()
{
    if (hasTake())
        return m_list.at(m_headTake.fetchAndAddOrdered(1));

    return QUrl{};
}
//---------------------------------------------------------------


