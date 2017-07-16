#include "coloralphapicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

static int pWidth = 270;
static int pHeight = 12;
ColorAlphaPicker::ColorAlphaPicker(QWidget *parent) : QAbstractButton(parent)
{
    a = 4;
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(pWidth, pHeight);
    setCheckable(true);
    connect(this, &QAbstractButton::clicked, this, [=] () {
        update();
    });
}

ColorAlphaPicker::~ColorAlphaPicker()
{

}

void ColorAlphaPicker::setRgb(int r, int g, int b, int a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    update();
    emit newColor(QColor(r, g, b, a));
}

void ColorAlphaPicker::setColor(QColor color)
{
    setRgb(color.red(), color.green(), color.blue(), color.alpha());
}

QColor ColorAlphaPicker::currentColor()
{
    return QColor(r, g, b, a);
}

void ColorAlphaPicker::mousePressEvent(QMouseEvent *m)
{
    setAlp(x2alp(m->x()));
    return QAbstractButton::mousePressEvent(m);
}

void ColorAlphaPicker::mouseMoveEvent(QMouseEvent *m)
{
    setAlp(x2alp(m->x()));
    return QAbstractButton::mouseMoveEvent(m);
}

void ColorAlphaPicker::enterEvent(QEvent *e)
{
//    repaint();
    return QAbstractButton::enterEvent(e);
}

void ColorAlphaPicker::leaveEvent(QEvent *e)
{
//    repaint();
    return QAbstractButton::leaveEvent(e);
}

void ColorAlphaPicker::paintEvent(QPaintEvent *)
{
    int w = width();
    int h = height();
    QPoint p1(2, 2);
    QPoint p2(w - 4, h - 4);
    QPainter p(this);
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(QRect(p1, p2), 3, 3);
    QLinearGradient liner(p1, p2);
    liner.setColorAt(0, QColor(r, g, b, 255));
    liner.setColorAt(1, QColor(r, g, b, 0));
    p.fillPath(path, QBrush(liner));

    QPoint pt = QPoint(alp2x(a), h / 2);
    QPainterPath path1;
    path1.addEllipse(QPoint(pt.x(), pt.y()), 2, 2);
    path1.addEllipse(QPoint(pt.x(), pt.y()), 4, 4);
    p.fillPath(path1, Qt::white);
    QColor color(230, 230, 230, 0);
    if (isChecked())
    {
        color.setAlpha(80);
    }
    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
    p.fillPath(path, QBrush(color));
}

int ColorAlphaPicker::alp2x(int a)
{
    return (255 - a) * (width() - 4) / 255;
}

int ColorAlphaPicker::x2alp(int x)
{
    if (x >= width() -4)
        x = width() - 4;
    else if (x <= 4)
        x = 4;
    return 255 - x * 255 / (width() - 4);
}

void ColorAlphaPicker::setAlp(int a)
{
    if (this->a == a)
        return;
    this->a = qMax(0, qMin(a,255));
    repaint();
    emit newColor(QColor(r, g, b, a));
}
