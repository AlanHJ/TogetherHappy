#include "calcrectopt.h"
#include <QLine>
#include <QDebug>
#include <QColor>
#include <QBitmap>
#include <QThread>
#include <QPainterPath>
#include <QApplication>

class CalcRectOpt::PrivData
{
public:
    PrivData()
    {
        thread = nullptr;
    }
    ~PrivData()
    {

    }
    QLine calcdPoint(QPoint pos, QPoint dp)
    {
        int ct1 = 0;
        int ct2 = 0;
        int len1 = 0;
        dp.x() == 0 ? len1 = image.height() : len1 = image.width();
        int len2 = 0;
        dp.x() == 0 ? len2 = pos.y() : len2 = pos.x();
        QColor color = image.pixel(pos);
        QPoint p1 = pos;
        QPoint p2 = pos;
        bool b1 = false;
        bool b2 = false;
        do
        {
            if (ct1 <= (len1 - len2) && !b1)
            {
                p1 += dp;
                ct1++;
                QColor c1 = image.pixel(p1);
                if (c1 != color)
                {
                    b1 = true;
                }
            }
            if (ct2 <= len2 && !b2)
            {
                p2 -= dp;
                ct2++;
                QColor c2 = image.pixel(p2);
                if (c2 != color)
                {
                    b2 = true;
                }
            }
        }
        while ((!b1 || !b2) && (ct1 + ct2 - 1) <= len1);
        return QLine(p1, p2);
    }
    int calcLength(QPoint pos, QPoint dp)
    {
        QPoint p = pos;
        QColor color = image.pixel(pos);
        int len = 0;
        int length = 0;
        dp.x() == 0 ? length = image.height() : length = image.width();
        do
        {
            p += dp;
            len++;
            QColor c = image.pixel(p);
            if (c != color)
            {
                break;
            }
        }
        while(len <= length);
        return len;
    }

public:
    QImage image;
    QThread *thread;
};

CalcRectOpt::CalcRectOpt(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QPainterPath>("QPainterPath");
    mData = new PrivData();
    mData->thread = new QThread();
    mData->thread->setObjectName("Calc Thread");
    this->moveToThread(mData->thread);
    connect(this, SIGNAL(signalSetPoint(QPoint)),
            this, SLOT(slotCalcRect(QPoint)));
    connect(mData->thread, SIGNAL(finished()),
            mData->thread, SLOT(deleteLater()));
    connect(mData->thread, SIGNAL(finished()),
            this, SLOT(deleteLater()));
    mData->thread->start();
}

CalcRectOpt::~CalcRectOpt()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void CalcRectOpt::deleteOpt()
{
    mData->thread->quit();
    mData->thread->wait();
}

void CalcRectOpt::setImage(QImage image)
{
    mData->image = image;
}

void CalcRectOpt::slotCalcRect(QPoint pos)
{
    EdgeEnum edge = undefine;
    QPoint seedp = pos;
    // standard color
    QColor standardc = mData->image.pixel(pos);
    // calc seed point
    int width = mData->image.width();
    int heigth = mData->image.height();
    int left = pos.x();
    int top = pos.y();
    int right = width - left;
    int bottom = heigth - top;
    int l = 1;
    int t = 1;
    int r = 1;
    int b = 1;
    do
    {
        QColor leftc = standardc;
        QColor rightc = standardc;
        QColor topc = standardc;
        QColor bottomc = standardc;
        // limit
        if (l <= left)
        {
            QPoint leftp = seedp + QPoint(-l, 0); // move left 1
            leftc = mData->image.pixel(leftp); // get left color
            l++;
            if (standardc != leftc)
            {
                edge = leftEdge;
                seedp = leftp + QPoint(1, 0);
                break;
            }
        }
        if (r <= right)
        {
            QPoint rightp = seedp + QPoint(r, 0); // move right 1
            rightc = mData->image.pixel(rightp); // get right color
            r++;
            if (standardc != rightc)
            {
                edge = rightEdge;
                seedp = rightp - QPoint(1, 0);
                break;
            }
        }
        if (t <= top)
        {
            QPoint topp = seedp + QPoint(0, -t); // move top 1
            topc = mData->image.pixel(topp); // get top color
            t++;
            if (standardc != topc)
            {
                edge = topEdge;
                seedp = topp + QPoint(0, 1);
                break;
            }
        }
        if (b <= bottom)
        {
            QPoint bottomp = seedp + QPoint(0, b); // move bottom 1
            bottomc = mData->image.pixel(bottomp); // get bottom color
            b++;
            if (standardc != bottomc)
            {
                edge = bottomEdge;
                seedp = bottomp - QPoint(0, 1);
                break;
            }
        }
    }
    while(l <= left || r <= right || t <= top || b <= bottom);

    QPoint topleft(0, 0);
    QSize size(0, 0);

    // recalc rect
    if (edge == leftEdge)
    {
        QLine line = mData->calcdPoint(seedp, QPoint(0, 1));
        int len = mData->calcLength(line.p1() - QPoint(1, 0), QPoint(1, 0));
        topleft = line.p1();
        size = QSize(len, line.dy());
#ifdef Calc_Debug
//        QPointF f(line.p1());
//        QPainterPath path(f);
//        path.lineTo(line.p2());
//        emit signalDebugPath(path);
#endif
    }
    else if (edge == rightEdge)
    {
        QLine line = mData->calcdPoint(seedp, QPoint(0, 1));
        int len = mData->calcLength(line.p2() - QPoint(1, 0), QPoint(1, 0));
        topleft = QPoint(line.x1() - len, line.y1());
        size = QSize(len, line.dy());
#ifdef Calc_Debug
//        QPointF f(line.p1());
//        QPainterPath path(f);
//        path.lineTo(line.p2());
//        emit signalDebugPath(path);
#endif
    }
    else if (edge == topEdge)
    {
        QLine line = mData->calcdPoint(seedp, QPoint(1, 0));
        int len = mData->calcLength(line.p1() + QPoint(0, 1), QPoint(0, 1));
        topleft = line.p1();
        size = QSize(line.dx(), len);
#ifdef Calc_Debug
//        QPointF f(line.p1());
//        QPainterPath path(f);
//        path.lineTo(line.p2());
//        emit signalDebugPath(path);
#endif
    }
    else if (edge == bottomEdge)
    {
        QLine line = mData->calcdPoint(seedp, QPoint(1, 0));
        int len = mData->calcLength(line.p1() - QPoint(0, 1), QPoint(0, 1));
        topleft = QPoint(line.x1(), line.y1() - len);
        size = QSize(line.dx(), len);
#ifdef Calc_Debug
//        QPointF f(line.p1());
//        QPainterPath path(f);
//        path.lineTo(line.p2());
//        emit signalDebugPath(path);
#endif
    }
    else
    {
        return;
    }
    QRect rect(topleft, size);
//    emit signalCalcRect(rect);
}

