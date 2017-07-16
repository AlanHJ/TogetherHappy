#ifndef MSGBOX_H
#define MSGBOX_H

#include "../thwidgetbase.h"
#include <QEventLoop>
class QLabel;
class QTextEdit;
class QEventLoop;
class QPushButton;
class MsgBox : public THWidgetBase
{
    Q_OBJECT
public:
    explicit MsgBox(QWidget *parent = 0);
    ~MsgBox();
    void setCloseButtonVisible(bool b);
    void setTitle(QString title);
    void setContent(QString content);
    void setButtons(QString bt1 = QString(), QString bt2 = QString(), QString bt3 = QString());
    int exec();
    static int ShowMsgBox(QString t, QString c, QString bt1 = QString(),
                          QString bt2 = QString(), QString bt3 = QString());
    static void ShowBRbox(QString t, QString c);
private slots:
    void buttonsClicked(int id);

private:
    QTextEdit *edit;
    QLabel *contentLbl;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;

    int id;

    QEventLoop loop;
};

#endif // MSGBOX_H
