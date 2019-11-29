/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   main.cpp
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#include "cwidget_main.h"

#include <QtCore/QFile>
#include <QtWidgets/QApplication>
//------------------------------------------------------


int main(int argc, char *argv[])
{
    QApplication a(argc,argv);

    QFile file("://stylesheet.qss");

    if (file.open(QIODevice::ReadOnly))
        a.setStyleSheet(file.readAll());

    CWidgetMain w;
    w.showMaximized();

    return a.exec();
}
//------------------------------------------------------
