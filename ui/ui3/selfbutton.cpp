#include "selfbutton.h"
#include <QTimer>

SelfButton::SelfButton(QWidget *w)
    : QPushButton(w)
{
    reset = true;
}

SelfButton::~SelfButton()
{

}

bool SelfButton::isReset()
{
    return reset;
}

void SelfButton::setReset(bool b)
{
    reset = b;
}


void SelfButton::enterEvent(QEvent *e)
{
    emit signalEnter();
    return QWidget::enterEvent(e);
}

void SelfButton::leaveEvent(QEvent *e)
{
    emit signalLeave();
    QTimer::singleShot(300, this, [=] () {
        if (!reset)
            reset = true;
    });
    return QWidget::leaveEvent(e);
}
