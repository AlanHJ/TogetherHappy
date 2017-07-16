#include "happytabwidget.h"
#include "happytabbar.h"
#include <QPainter>

HappyTabWidget::HappyTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
//    setWindowFlags(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);
    bar = new HappyTabBar(this);
    bar->setFixedWidth(230);
    setTabBar(bar);
    bar->hide();
    setStyleSheet("QTabWidget::pane{background-color:transparent;"
                  "border-width:1px;"
                  "border-style:solid;"
                  "border-color: transparent;"
                  "border-radius: 3px;}");
}

HappyTabWidget::~HappyTabWidget()
{

}



void HappyTabWidget::paintEvent(QPaintEvent *e)
{
//    QPainter painter(this);
//    QPainterPath path;
//    path.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
//    painter.fillPath(path, QBrush(QColor(230, 230, 230, 0)));
    return QTabWidget::paintEvent(e);
}
