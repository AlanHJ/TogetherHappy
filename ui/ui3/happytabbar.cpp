#include "happytabbar.h"
#include <QPainter>

HappyTabBar::HappyTabBar(QWidget *parent)
    : QTabBar(parent)
{

}

HappyTabBar::~HappyTabBar()
{

}

void HappyTabBar::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.fillPath(path, QBrush(QColor(230, 230, 230, 50)));
    return QTabBar::paintEvent(e);
}

