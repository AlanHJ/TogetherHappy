#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>
static int pWidth = 270;
static int pHeight = 100;
QPoint QColorPicker::colPt()
{
    QRect r = contentsRect();
    if (val == 255)
    {
        return QPoint(hue * r.width() / 360, sat * r.height()  / 255 / 2);
    }
    else
    {
        return QPoint(r.width() - 1 - (360 - hue) * (r.width() - 1) / 360,
                      (255 - sat) * (r.height() / 2 - 1) / 255 + r.height() / 2 + 1);
    }
//    return QPoint((360 - hue) * (r.width() - 1) / 360, (255 - sat) * (r.height() - 1) / 255);
}

int QColorPicker::huePt(const QPoint &pt)
{
    QRect r = contentsRect();
    if (pt.y() > r.height() / 2)
    {
        return 360 - (r.width() - pt.x() - 1) * 360 / (r.width() - 1);
    }
    else
    {
        return pt.x() * 360 / (r.width());
    }
//    return /*360 -*/ pt.x() * 360 / (r.width() - 1);
}

int QColorPicker::satPt(const QPoint &pt)
{
    QRect r = contentsRect();
    if (pt.y() > r.height() / 2)
    {
        return 255 - (pt.y() - r.height() / 2 - 1) * 255 / (r.height() / 2 - 1);
    }
    else
    {
        return pt.y() * 255 / (r.height() / 2);
    }
//    return /*255 -*/ pt.y() * 255 / (r.height() - 1);
}

int QColorPicker::valPt(const QPoint &pt)
{
    QRect r = contentsRect();
    int halfH = r.height() / 2;
    if (pt.y() > halfH)
    {
        return 255 - (pt.y() - halfH) * 255 / halfH;
    }
    else
    {
        return 255;
    }
}



void QColorPicker::setCol(const QPoint &pt)
{
    setCol(huePt(pt), satPt(pt), valPt(pt));
}

QColorPicker::QColorPicker(QWidget* parent)
    : QFrame(parent)
{
    hue = 0; sat = 0;
    setCol(150, 255, 255);

    setAttribute(Qt::WA_NoSystemBackground);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed) );
}

QColorPicker::~QColorPicker()
{
}

QSize QColorPicker::sizeHint() const
{
    return QSize(pWidth + 2*frameWidth(), pHeight + 2*frameWidth());
}

void QColorPicker::setCol(int h, int s, int v)
{
    int nhue = qMin(qMax(0,h), 359);
    int nsat = qMin(qMax(0,s), 255);
    int nval = qMin(qMax(0, v), 255);
    if (nhue == hue && nsat == sat && nval == val)
        return;
    hue = nhue; sat = nsat; val = nval;
    update();
}

void QColorPicker::setColor(QColor color)
{
    QColor c;
    c.setRgb(color.rgb());
    int h, s, v;
    c.getHsv(&h, &s, &v);
    setCol(h, s, v);
}

void QColorPicker::mouseMoveEvent(QMouseEvent *m)
{
    QPoint p = m->pos() - contentsRect().topLeft();
    setCol(p);
    emit newCol(hue, sat, val);
}

void QColorPicker::mousePressEvent(QMouseEvent *m)
{
    QPoint p = m->pos() - contentsRect().topLeft();
    setCol(p);
    emit newCol(hue, sat, val);
}

void QColorPicker::paintEvent(QPaintEvent* )
{
    QPainter p(this);
    drawFrame(&p);
    QRect r = contentsRect();

    p.drawPixmap(r.topLeft(), pix);
    QPoint pt = colPt() + r.topLeft();
    QPainterPath path;
    path.addEllipse(QPoint(pt.x(), pt.y()), 2, 2);
    path.addEllipse(QPoint(pt.x(), pt.y()), 4, 4);
    p.fillPath(path, Qt::white);

}

void QColorPicker::resizeEvent(QResizeEvent *ev)
{
    QFrame::resizeEvent(ev);

    int w = width() - frameWidth() * 2;
    int h = height() - frameWidth() * 2;
    QImage img(w, h, QImage::Format_RGB32);
    int x, y;
    uint *pixel = (uint *) img.scanLine(0);
    for (y = 0; y < h; y++) {
        const uint *end = pixel + w;
        x = 0;
        while (pixel < end) {
            QPoint p(x, y);
            QColor c;
            c.setHsv(huePt(p), satPt(p), valPt(p));
            *pixel = c.rgb();
            ++pixel;
            ++x;
        }
    }
    pix = QPixmap::fromImage(img);
}
