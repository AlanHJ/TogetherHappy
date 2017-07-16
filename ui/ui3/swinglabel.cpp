#include "swinglabel.h"
#include <QTimer>
#include <QTimeLine>
#include <QPainter>

SwingLabel::SwingLabel(QWidget *parent)
    : QWidget(parent)
{
   degree = 0;
   setFocusPolicy(Qt::NoFocus);
   setAttribute(Qt::WA_TranslucentBackground);
   QTimer::singleShot(2000, this, [=] () {
       QTimeLine *tl = new QTimeLine(1000, this);
       tl->setLoopCount(0);
       tl->setFrameRange(15, -15);
       tl->setCurveShape(QTimeLine::SineCurve);
//       tl->setUpdateInterval(25);
       connect(tl, &QTimeLine::frameChanged, this, [=] () {
           degree = tl->currentFrame();
           update();
       });
       tl->start();
   });
}

SwingLabel::~SwingLabel()
{

}

void SwingLabel::setImagePath(QString path)
{
    imagePath = path;
}

void SwingLabel::paintEvent(QPaintEvent *)
{
    int w = this->width();
    int h = this->height();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QImage image(imagePath);
    p.translate(w / 2, 0);
    p.rotate(degree);
    p.translate(-w / 2, 0);
    p.drawImage(QRect(0, 0, w, h), image, image.rect());
}

