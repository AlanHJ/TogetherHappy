#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QEventLoop>
#include "../thwidgetbase.h"

class QPushButton;
class QLineEdit;
class InputBox : public THWidgetBase
{
    Q_OBJECT
public:
    explicit InputBox(QWidget *parent = 0);
    ~InputBox();
    void setEchoMode(QLineEdit::EchoMode mode = QLineEdit::Normal);
    void setTitle(QString title);
    void setPlaceHoldTxt(QString txt);
    QString exec();
    static QString GetInput(QString title, bool *ok = false,
                            QLineEdit::EchoMode mode = QLineEdit::Normal,
                            QString holdtxt = QString(""));

private slots:
    void buttonsClicked(int id);

private:
    QLineEdit *edit;
    QPushButton *pb1;
    QPushButton *pb2;
    bool isOk;
    QEventLoop loop;
};

#endif // INPUTBOX_H
