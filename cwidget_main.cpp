/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cwidget_main.cpp
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#include "cmanager_parsers.h"
#include "cmodel_tasks.h"
#include "cwidget_control.h"
#include "cwidget_main.h"

#include <QtWidgets/QSplitter>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

//-----------------------------------------------------------------------------------------


CWidgetMain::CWidgetMain(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<QVector<QUrl>>("QVectorUrl");

    m_manager = new CManagerParsers(this);
    m_tasks = new CModelTasks(this);
    m_control = new CWidgetControl(this);

    QTableView *tableView = new QTableView(this);
    tableView->setModel(m_tasks);
    tableView->setColumnWidth(0,100);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);

    QSplitter *splitter = new QSplitter(Qt::Vertical,this);
    splitter->addWidget(m_control);
    splitter->addWidget(tableView);
    splitter->setStretchFactor(1,80);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(splitter);
    vbox->setMargin(0);

    this->setLayout(vbox);

    connect(m_control,&CWidgetControl::out_find,
            m_manager,&CManagerParsers::in_start);

    connect(m_control,&CWidgetControl::out_pause,
            m_manager,&CManagerParsers::out_pause);

    connect(m_control,&CWidgetControl::out_stop,
            m_manager,&CManagerParsers::out_stop);

    connect(m_control,&CWidgetControl::out_set_param,
            m_manager,&CManagerParsers::in_set_param);

    connect(m_control,&CWidgetControl::out_set_param,
            m_tasks,&CModelTasks::in_set_param);

    // manager to tasks
    connect(m_manager,&CManagerParsers::out_start,
            m_tasks,&CModelTasks::in_start);

    connect(m_manager,&CManagerParsers::out_add,
            m_tasks,&CModelTasks::in_add);

    connect(m_manager,&CManagerParsers::out_update,
            m_tasks,&CModelTasks::in_update);

    connect(m_manager,&CManagerParsers::out_take,
            m_tasks,&CModelTasks::in_take);

    connect(m_manager,&CManagerParsers::out_finished,
            m_control,&CWidgetControl::in_reset);

    // tasks to manager
    connect(m_tasks  , &CModelTasks::rowsInserted,
            tableView, &QTableView::scrollToBottom);

    connect(m_tasks  , &CModelTasks::out_completed,
            tableView, &QTableView::reset);

    connect(m_tasks,&CModelTasks::out_send,
            m_manager,&CManagerParsers::out_send);

    connect(m_tasks,&CModelTasks::out_completed,
            m_manager,&CManagerParsers::out_completed);
}
//-----------------------------------------------------------------------------------------


CWidgetMain::~CWidgetMain()
{
}
//-----------------------------------------------------------------------------------------


