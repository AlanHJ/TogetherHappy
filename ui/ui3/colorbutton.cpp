#include "colorbutton.h"
#include <QPainter>
#include <QPainterPath>

ColorButton::ColorButton(QWidget *parent)
    : QAbstractButton(parent)
{
    alpha = 0;
    color = QColor(Qt::white);
}

ColorButton::~ColorButton()
{

}

void ColorButton::setColor(QColor color)
{
    this->color = color;
    update();
}

QColor ColorButton::CurrentColor()
{
    return color;
}

void ColorButton::enterEvent(QEvent *)
{
    alpha = 45;
    update();
}

void ColorButton::leaveEvent(QEvent *)
{
    alpha = 0;
    update();
}

void ColorButton::paintEvent(QPaintEvent *)
{
    int w = this->width();
    int h = this->height();
    QPainter p(this);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
    p.fillPath(path, QBrush(QColor(225, 225, 225, alpha)));
    if (isDown())
    {
        p.fillPath(path, QBrush(QColor(50, 50, 50, alpha)));
    }
    QPainterPath path2;
    path2.addRoundedRect(QRect(3, 3, w - 6, h - 6), 3, 3);
    p.fillPath(path2, color);

}

