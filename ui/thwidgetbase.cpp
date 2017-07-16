#include "thwidgetbase.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include "../util/thmethod.h"
#include "happytitlebar.h"
#include <QGraphicsBlurEffect>
#include <QtWinExtras>
#include <QApplication>
#include <QHBoxLayout>
#include <QRect>
#include "../manager/widgetmanager.h"
#include "../config/configuredata.h"

THWidgetBase::THWidgetBase(QWidget *parent, bool shadow) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);

    bezierX = 0;
    bezierY = 0;
    bezierColor = QColor(30, 30, 30, 35);
    bezierEnable = false;
    showShadow = shadow;
    leftPressed = false;
    bar = new HappyTitleBar(this);
    setSizeEnable(false);

    startColor = QColor(/*47, 107, 117*/244, 184, 187, 240);
    endColor = QColor(47, 107, 147, 250);
    // windows aero effect
//    if (QtWin::isCompositionEnabled())
//    {
//        QtWin::extendFrameIntoClientArea(this, 15, 15, 15, 15);
//        setAttribute(Qt::WA_TranslucentBackground, true);
//        setAttribute(Qt::WA_NoSystemBackground, false);
//    }
    bezierTime = new QTimeLine(6000, this);
    bezierTime->setLoopCount(0);
    bezierTime->setFrameRange(0, this->width());
    bezierTime->setCurveShape(QTimeLine::LinearCurve);
    connect(bezierTime, &QTimeLine::frameChanged, this, [=] () {
        bezierX = bezierTime->currentFrame() + ShadowWidth;
        QTime time = QTime::currentTime();
        int h = this->height() - 2 * ShadowWidth;
        if (time < StartTime)
        {
            bezierY = h;
        }
        else if (time > EndTime)
        {
            bezierY = ShadowWidth;
        }
        else
        {
            float t1 = time.msecsTo(EndTime) / 1000 / 60.0;
            float t2 = StartTime.msecsTo(EndTime) / 1000 / 60.0;
            bezierY = h - (t2 - t1) / t2 * h;
        }
        update();
    });

    WidgetManager::GetInstance()->registerWidget(this);

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

}

THWidgetBase::~THWidgetBase()
{
    WidgetManager::GetInstance()->unregisterWidget(this);
}

void THWidgetBase::setTitleBarWidth(int width)
{
    bar->setBarWidth(width);
    bar->move(ShadowWidth, ShadowWidth);
//    connect(bar, SIGNAL(signalMinimize()), this, SLOT(hide()));
}

void THWidgetBase::hideTitleBar()
{
    bar->hide();
}

void THWidgetBase::setSizeEnable(bool b, QSize max)
{
    isResize = b;
    minSize = max;
    setMouseTracking(b);
}

void THWidgetBase::updateColor(QColor color1, QColor color2)
{
    startColor = color1;
    endColor = color2;
    update();
}

void THWidgetBase::setBezierEnable(bool b)
{
    if (!bezierEnable)
        return;
    if (b)
    {
        if (bezierTime->state() == QTimeLine::NotRunning)
        {
            bezierColor = QColor(30, 30, 30, 35);
            bezierTime->setFrameRange(0, this->width());
            bezierTime->start();
        }
    }
    else
    {
        bezierColor = Qt::transparent;
        if (bezierTime->state() == QTimeLine::Running)
            bezierTime->stop();
        update();
    }
}

qint64 THWidgetBase::getActivateTime()
{
    return activeTime;
}

QRect THWidgetBase::centerRect(QPoint center, QSize size)
{
    QRect rect(QPoint(0, 0), size);
    rect.moveCenter(center);
    return rect;
}



void THWidgetBase::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    //    painte
    QPainterPath path;
    int shadowW = ShadowWidth;
    if (!showShadow)
        shadowW = 0;
    float h = this->height();
    float w = this->width();
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(QRect(shadowW, shadowW, w - 2 * shadowW, h - 2 * shadowW), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // LinearGradient
    QLinearGradient liner(QPoint(shadowW, shadowW), QPoint(w - 2 * shadowW, h - 2 * shadowW));
    // 132, 216, 209 À¶
    // 47, 107, 117 Ä¬ÈÏ  47£¬107£¬147
    // 128,203,196 ÂÌ1
    // 0,121,107
    liner.setColorAt(0, startColor);
    {
//        float alp = abs(endColor.alpha() - startColor.alpha());
//        QColor color(QColor::colorNames().at(alp));
//        color.setAlpha(255 - alp);
//        liner.setColorAt(alp / 255, color);
    }
    liner.setColorAt(1, endColor);
    painter.fillPath(path, QBrush(liner));
    // draw shadow
    if (showShadow)
    {
        QColor color(0, 0, 0, 50);
        for(int i = 0; i < ShadowWidth; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRoundedRect(QRect(ShadowWidth - i, ShadowWidth - i, w - (ShadowWidth - i) * 2, h- (ShadowWidth - i) * 2), 3, 3);
            color.setAlpha(120 - qSqrt(i) * 50);
            painter.setPen(color);
            painter.drawPath(path);
        }
        painter.setPen(Qt::transparent);
        painter.drawPath(path);
    }
    // draw some lines
    QList<QPoint> line1;
    QList<QPoint> line2;
    {
        // calc start, final point
        float k = h / w;
        int b[7];
        int x1[7], x2[7];
        b[0] = h /3;
        b[1] = h /2;
        b[2] = 2 * h / 3;
        b[3] = 4 * h / 3;
        b[4] = 3 * h / 2;
        b[5] = 5 * h / 3;
        b[6] = h;
        x1[0] = w / 6;
        x2[0] = w / 4;
        x1[1] = w / 4;
        x2[1] = 2 * w / 5;
        x1[2] = w / 4;
        x2[2] = 4 * w / 7;
        x1[3] = w / 2;
        x2[3] = 5 * w / 6;
        x1[4] = 2 * w / 3;
        x2[4] = 5 * w / 6;
        x1[5] = 3 * w / 4;
        x2[5] = 4 * w / 5;
        x1[6] = w / 4;
        x2[6] = 3 * w / 4;

        for (int i = 0; i < 7; i++)
        {
            line1 << QPoint(x1[i], - k * x1[i] + b[i]);
            line2 << QPoint(x2[i], - k * x2[i] + b[i]);
        }
    }
//    srand(QDateTime::currentDateTime().toTime_t());
//    int counts = qrand() % 4 + 3;
    for (int i = 0; i < 7; i++)
    {
        QPoint p1 = line1.at(i);
        QPoint p2 = line2.at(i);
        QLinearGradient linecolor(p1, p2);
        linecolor.setColorAt(0, QColor(220, 220, 220, 50));
        linecolor.setColorAt(0.45, QColor(220, 220, 220, 150));
        linecolor.setColorAt(0.55, QColor(220, 220, 220, 150));
        linecolor.setColorAt(1, QColor(220, 220, 220, 50));
        painter.setPen(QPen(QBrush(linecolor), 1));
        painter.drawLine(p1, p2);
    }

    if (!bezierEnable)
        return;
    if (bezierTime->state() == QTimeLine::Running)
        bezierColor = QColor(30, 30, 30, 35);
    else
        bezierColor = Qt::transparent;
    QPainterPath bezierpath;
    bezierpath.addRect(QRect(shadowW, bezierY,
                             w - 2 * shadowW, h - bezierY));

//    bezierX += shadowW;
//    bezierY += shadowW;
    int x11 = -5 * w / 6 + bezierX;
    int x22 = -2 * w / 3 + bezierX;
    bezierpath.cubicTo(QPoint(x11, bezierY - 50),
                     QPoint(x22, bezierY + 50),
                     QPoint(-w / 2 + bezierX, bezierY));


    int x33 = -w / 3 + bezierX;
    int x44 = -w / 6 + bezierX;
    bezierpath.cubicTo(QPoint(x33, bezierY - 50),
                       QPoint(x44, bezierY + 50),
                       QPoint(bezierX, bezierY));


    int x1 = w / 6 + bezierX;
    int x2 = w / 3 + bezierX;
    bezierpath.cubicTo(QPoint(x1, bezierY - 50),
                       QPoint(x2, bezierY + 50),
                       QPoint(w / 2 + bezierX, bezierY));


    int x3 = 2 * w / 3 + bezierX;
    int x4 = 5 * w / 6 + bezierX;
    bezierpath.cubicTo(QPoint(x3, bezierY - 50),
                       QPoint(x4, bezierY + 50),
                       QPoint(w + bezierX, bezierY));

//    QLinearGradient bezierliner(QPoint(shadowW, bezierY), QPoint(w - 2 * shadowW, h - bezierY));

    painter.fillPath(bezierpath, bezierColor);

    return QWidget::paintEvent(e);
}

void THWidgetBase::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
        return;
    leftPressed = true;
    position = e->globalPos();
    e->ignore();
//    return QWidget::mousePressEvent(e);
}

void THWidgetBase::mouseMoveEvent(QMouseEvent *e)
{
    if (isResize && !leftPressed)
    {
        QPoint p = this->rect().bottomRight();
        p = QPoint(p.x() - 5, p.y() - 5);
        QRect rct(0, 0, 10, 10);
        rct.moveCenter(p);
        if (rct.contains(e->pos()))
        {
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else
        {
            setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    if (leftPressed)
    {
        if (cursor().shape() == Qt::SizeFDiagCursor)
        {
            QPoint pos = this->pos();
            QPoint dp = e->globalPos() - position;
            QSize size(this->width() + dp.x(), this->height() + dp.y());
            position = e->globalPos();
            if (size.width() <= minSize.width() || size.height() <= minSize.height())
                return;
            setFixedSize(size);
            this->move(pos);
        }
        else
        {
            QPoint dp = e->globalPos() - position + this->pos();
            this->move(dp);
            position = e->globalPos();
            e->ignore();
        }
    }

//    return QWidget::mouseMoveEvent(e);
}

void THWidgetBase::mouseReleaseEvent(QMouseEvent *e)
{
    leftPressed = false;
    setCursor(QCursor(Qt::ArrowCursor));
    QApplication::restoreOverrideCursor();
    e->ignore();
//    return QWidget::mouseReleaseEvent(e);
}


void THWidgetBase::resizeEvent(QResizeEvent *)
{
    QTimer::singleShot(20, this, [=] () {
        bar->setMaxIcon();
        bezierTime->setFrameRange(0, this->width());
    });
}
