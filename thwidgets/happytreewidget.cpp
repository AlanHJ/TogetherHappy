#include "happytreewidget.h"
#include "../util/thmethod.h"
#include <QMouseEvent>
#include <QPainter>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QPropertyAnimation>
#include "model/itemeditor.h"

HappyTreeWidget::HappyTreeWidget(QWidget *w)
    : QTreeWidget(w)
{

    // add animation
    opac = 0;
    isHide = false;
    animation = new QPropertyAnimation(this, "opacity");
    this->verticalScrollBar()->hide();
    setStyleSheet("QTreeView{background:transparent;}"
                  "QTreeWidget{background:transparent;"
                  "border-width:1px;"
                  "border-style:solid;"
                  "border-color: transparent;}");
    QFile file(":res/css/scrollbar.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString css = file.readAll();
        this->verticalScrollBar()->setStyleSheet(css);
    }
    file.close();
    file.setFileName(":res/css/tree.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString css = file.readAll();
        setStyleSheet(css);
    }
    file.close();

    connect(this, &HappyTreeWidget::itemDoubleClicked, [=] (QTreeWidgetItem *item, int column) {
        QWidget *w = itemWidget(item, column);
        ItemEditor *ie = qobject_cast<ItemEditor *>(w);
        if (ie == nullptr)
        {
            return;
        }
        for (int i = 0; i < topLevelItemCount(); i++)
        {
            if (item == topLevelItem(i))
            {
                emit topmemberDoubleClicked(ie);
                return;
            }
        }
        emit memberDoubleClicked(ie);
    });
}

HappyTreeWidget::~HappyTreeWidget()
{

}

qreal HappyTreeWidget::getopacity()
{
    return this->verticalScrollBar()->windowOpacity();
}

void HappyTreeWidget::setopacity(qreal opacity)
{
    this->verticalScrollBar()->setWindowOpacity(opacity);
    this->verticalScrollBar()->update();
    emit opacityChanged(opacity);
}



void HappyTreeWidget::mousePressEvent(QMouseEvent *e)
{
    switch (e->button()) {
    case Qt::RightButton:
        // show menu
        break;
    default:
        break;
    }
    QTreeWidget::mousePressEvent(e);
}

void HappyTreeWidget::actionRename()
{

}

void HappyTreeWidget::showAnimation()
{
    animation->setDuration(500);
    animation->setStartValue(0);
    this->verticalScrollBar()->show();
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] () {
        disconnect(animation, 0, 0, 0);
    });
}

void HappyTreeWidget::hiedAnimation()
{
//    animation->setDuration(2000);
//    animation->setStartValue(1);
//    animation->setEndValue(0);
//    animation->start();
//    connect(animation, &QPropertyAnimation::finished, [=] () {
        this->verticalScrollBar()->hide();
//        disconnect(animation, 0, 0, 0);
//    });
}


void HappyTreeWidget::paintEvent(QPaintEvent *e)
{
    return QTreeWidget::paintEvent(e);
}

void HappyTreeWidget::enterEvent(QEvent *e)
{
//    showAnimation();
    this->verticalScrollBar()->setWindowOpacity(0.23);
    return QTreeWidget::enterEvent(e);
}

void HappyTreeWidget::leaveEvent(QEvent *e)
{
    hiedAnimation();
    return QTreeWidget::leaveEvent(e);
}

void HappyTreeWidget::wheelEvent(QWheelEvent *e)
{
    this->verticalScrollBar()->show();
    isHide = true;
//    QTimer::singleShot(5000, this, [=] () {
//        if (isHide)
//        {
//            isHide = false;
//            return;
//        }
//        this->verticalScrollBar()->hide();
//    });
    return QTreeWidget::wheelEvent(e);
}
