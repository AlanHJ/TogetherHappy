#include "progressbar.h"
#include <QPropertyAnimation>

ProgressBar::ProgressBar(QWidget *parent)
    : QProgressBar(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    animation = new QPropertyAnimation(this, "opacity");
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::showAnimation()
{
        animation->setDuration(1000);
        animation->setStartValue(0);
        this->show();
        animation->setEndValue(1);
        animation->start();
}

qreal ProgressBar::getopacity()
{
    return opacity_;
}

void ProgressBar::setopacity(qreal opacity)
{
    opacity_ = opacity;
    setWindowOpacity(opacity);
    update();
}

