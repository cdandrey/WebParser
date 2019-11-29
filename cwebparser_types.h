/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   ctypes.h
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#ifndef CWEBPARSER_TYPES_H
#define CWEBPARSER_TYPES_H

#include <QtCore/QVector>
#include <QtCore/QHash>
#include <QtCore/QScopedPointer>
#include <QtCore/QUrl>

namespace WPt
{
    struct CParam
    {
        int maxThreads;
        int maxUrls;
    };

    using QScopedPtrParam = QScopedPointer<CParam>;
    using QVectorUrl = QVector<QUrl>;
    using QHashUrl   = QHash<QUrl,int>;

    enum TaskState
    {

    };

    static const int C_WAITING           {0};
    static const int C_DOWNLOADING       {1};
    static const int C_DOWNLOADING_ERROR {2};
    static const int C_FINDING           {3};
    static const int C_FOUND             {4};
    static const int C_NOT_FOUND         {5};

    static const int C_DEFAULT_NUM_THRDS {1};
    static const int C_DEFAULT_NUM_URLS  {100};
}


#endif // CWEBPARSER_TYPES_H

