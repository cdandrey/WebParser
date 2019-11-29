/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cwidget_main.h
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#ifndef CWIDGETMAIN_H
#define CWIDGETMAIN_H

#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
class CManagerParsers;
class CModelTasks;
class CWidgetControl;
QT_END_NAMESPACE

class CWidgetMain : public QWidget
{
    Q_OBJECT

public:

    explicit CWidgetMain(QWidget *parent = 0);
    ~CWidgetMain();

private:

    CManagerParsers *m_manager;
    CModelTasks     *m_tasks;
    CWidgetControl  *m_control;

    int m_paramMaxThreads;
    int m_paramMaxUrls;

private slots:

};

#endif // CWIDGETMAIN_H
