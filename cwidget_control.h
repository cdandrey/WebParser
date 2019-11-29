/////////////////////////////////////////////////////////////////
///
/// Project    :   WebParser
/// Author     :   Andrii Sydorenko
/// File       :   cwidget_control.h
/// Description:
/// Data       :   2019.11.24
///
/////////////////////////////////////////////////////////////////

#ifndef CWIDGETCONTROL_H
#define CWIDGETCONTROL_H

#include "cwebparser_types.h"

#include <QtCore/QUrl>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QLineEdit;
class QPlainTextEdit;
class QToolBar;
QT_END_NAMESPACE

class CWidgetControl : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetControl(QWidget *parent = 0);

signals:

    void out_find(QUrl,QByteArray);
    void out_pause(bool);
    void out_stop();
    void out_set_param(const WPt::CParam*);

public slots:

    void in_reset();

private:

    QAction *m_actFind;
    QAction *m_actPause;
    QAction *m_actStop;
    QAction *m_actSettings;

    QLineEdit *m_lineUrl;
    QLineEdit *m_lineMaxThrds;
    QLineEdit *m_lineMaxUrls;
    QPlainTextEdit *m_editText;

    QToolBar *m_toolCommand;

    WPt::QScopedPtrParam m_param;
    bool m_pause;

    static const QString C_CAPTION_LABEL_TITLE_URL;
    static const QString C_CAPTION_ACTION_FIND    ;
    static const QString C_CAPTION_ACTION_PAUSE   ;
    static const QString C_CAPTION_ACTION_STOP    ;
    static const QString C_CAPTION_ACTION_SETTINGS;
    static const QString C_HELP_EDIT_TEXT         ;
    static const int     C_MARGIN_EDIT_TEXT       ;
    static const int     C_MAXBLOCK_EDIT_TEXT     ;


private slots:

    void in_find(bool);
    void in_pause(bool);
    void in_stop(bool);
};

#endif // CWIDGETCONTROL_H
