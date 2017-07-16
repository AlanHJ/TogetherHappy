#include "selftoolbutton.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

SelfToolButton::SelfToolButton(QWidget *parent)
    : QWidget(parent), target1(0, 0, 0, 0), target2(0, 0, 0, 0)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    isEnter = false;
    arrowDownPaths << ":/res/ui3/down.png"
                   << ":/res/ui3/down_hover.png"
                   << ":/res/ui3/down_press.png";
    arrowImage = arrowDownPaths.first();
    alpha = 0;
}

SelfToolButton::~SelfToolButton()
{

}

void SelfToolButton::setImagePaths(QStringList list)
{
    imagePaths = list;
    mainImage = list.first();
    update();
}

void SelfToolButton::setArrowDownImage(QStringList list)
{
    arrowDownPaths = list;
}

void SelfToolButton::setSign(int id)
{
    this->id = id;
}

void SelfToolButton::selectButton(int id)
{
    if (this->id == id)
    {
        alpha = 50;
    }
    else
    {
        alpha = 0;
    }
    update();
}

void SelfToolButton::enterEvent(QEvent *e)
{
    isEnter = true;
    return QWidget::enterEvent(e);
}

void SelfToolButton::leaveEvent(QEvent *e)
{
    isEnter = false;
    arrowImage = arrowDownPaths.first();
    mainImage = imagePaths.first();
    update();
    return QWidget::leaveEvent(e);
}

void SelfToolButton::paintEvent(QPaintEvent *e)
{

    int w = this->width();
    int h = this->height();
    target1.setY(0);
    target1.setX(1 * w / 8);
    target1.setHeight(h);
    target1.setWidth(3 * w / 4);

    target2.setY(0);
    target2.setX(3 * w / 4);
    target2.setHeight(h);
    target2.setWidth(w / 4);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    if (isEnter)
    {
        // draw down.png pictrue
        QImage image(arrowImage);
        p.drawImage(target2, image, image.rect());
    }
    QImage image(mainImage);
//    image = image.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    p.drawImage(target1.marginsRemoved(QMargins(14, 5, 14, 5)), image, image.rect(), Qt::AutoColor);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    p.fillPath(path, QBrush(QColor(230, 230, 230, alpha)));
    return QWidget::paintEvent(e);
}

void SelfToolButton::mousePressEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    if (target2.contains(p))
    {
        arrowImage = arrowDownPaths.last();
        emit signalArrowClicked();
    }
    if (target1.contains(p))
    {
        mainImage = imagePaths.last();
        emit signalClicked();
    }
    update();
    return QWidget::mousePressEvent(e);
}

void SelfToolButton::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    if (target2.contains(p))
    {
        arrowImage = arrowDownPaths.at(1);
    }
    if (target1.contains(p))
    {
        mainImage = imagePaths.at(1);
    }
    update();
    return QWidget::mouseReleaseEvent(e);
}

void SelfToolButton::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    if (target2.contains(p))
    {
        arrowImage = arrowDownPaths.at(1);
    }
    else
    {
        arrowImage = arrowDownPaths.last();
    }
    if (target1.contains(p))
    {
        mainImage = imagePaths.at(1);
    }
    else
    {
        mainImage = imagePaths.last();
    }
    update();
    return QWidget::mouseMoveEvent(e);
}

