#include "rotatebutton.h"
#include <QTimer>
#include <QTimeLine>
#include <QPainter>
#include <QPainterPath>

RotateButton::RotateButton(QWidget *parent)
    : QAbstractButton(parent)
{
    alpha = 0;
    degree = 0;
    loop = 5;
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_TranslucentBackground);
    QTimeLine *tl = new QTimeLine(1000, this);
    connect(this, &QAbstractButton::clicked, this, [=] () {
        update();
        this->setEnabled(false);
        QTimer::singleShot(500, this, [=] () {
            tl->setDuration(1000);  // 持续时间
            tl->setLoopCount(loop);    // 3次循环
            tl->setFrameRange(0, 360); // 0~360度
            tl->setCurveShape(QTimeLine::LinearCurve);
            tl->setUpdateInterval(25); // 25ms更新一次
            connect(tl, &QTimeLine::finished, tl, [=] () {
                this->setEnabled(true);
            });
            connect(tl, &QTimeLine::frameChanged, this, [=] () {
                // 旋转
                degree = tl->currentFrame();
                update();
            });
            connect(this, &RotateButton::stopAnimation, this, [=] () {
                if (tl->state() == QTimeLine::Running)
                {
                    tl->stop();
                    tl->setLoopCount(1);
                    tl->start();
                }
            });
            tl->start();
        });
    });

    setStyleSheet("QToolTip{border:0px solid transparent;"
                  "background-color:rgba(47, 107, 117, 100);"
                  "border-radius:3px;"
                  "color:white;}"
                  "QWidget:focus{outline: none;}");
}

RotateButton::~RotateButton()
{

}

void RotateButton::setImagePath(QString path)
{
    imagePath = path;
}

void RotateButton::setLoop(int times)
{
    loop = times;
}

void RotateButton::paintEvent(QPaintEvent *)
{
    int w = this->width();
    int h = this->height();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
    p.fillPath(path, QBrush(QColor(225, 225, 225, alpha)));
    if (isDown())
    {
        p.fillPath(path, QBrush(QColor(50, 50, 50, alpha)));
    }
    QImage image(imagePath);
    p.translate(w / 2, h / 2);
    p.rotate(degree);
    p.translate(-w / 2, -h / 2);
    p.drawImage(QRect(0, 0, w, h), image, image.rect());
}

void RotateButton::enterEvent(QEvent *)
{
    alpha = 45;
    update();
}

void RotateButton::leaveEvent(QEvent *)
{
    alpha = 0;
    update();
}

