#include "colorluminancepicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <qdrawutil.h>

static int pWidth = 270;
static int pHeight = 8;
int QColorLuminancePicker::x2val(int x)
{
    return 255 - x * 255 / width();
}

int QColorLuminancePicker::val2x(int v)
{
    return (255 - v) * width() / 255;
}

QColorLuminancePicker::QColorLuminancePicker(QWidget* parent)
    :QWidget(parent)
{
    hue = 100; val = 100; sat = 100;
    setFixedSize(pWidth, pHeight);
}

QColorLuminancePicker::~QColorLuminancePicker()
{

}

void QColorLuminancePicker::mouseMoveEvent(QMouseEvent *m)
{
    setVal(x2val(m->x()));
}
void QColorLuminancePicker::mousePressEvent(QMouseEvent *m)
{
    setVal(x2val(m->x()));
}

void QColorLuminancePicker::setVal(int v)
{
    if (val == v)
        return;
    val = qMax(0, qMin(v,255));
    repaint();
    emit newHsv(hue, sat, val);
}

void QColorLuminancePicker::setCol(int h, int s)
{
    setCol(h, s, val);
    emit newHsv(h, s, val);
}

void QColorLuminancePicker::setColor(QColor color)
{
    QColor c;
    c.setRgb(color.rgb());
    int h, s, v;
    c.getHsv(&h, &s, &v);
    setCol(h, s, v);
}

void QColorLuminancePicker::paintEvent(QPaintEvent *)
{
    int w = width();
    int h = height();
    QImage img(w, h, QImage::Format_RGB32);
    int x, y;
    uint *pixel = (uint *) img.scanLine(0);
    for (y = 0; y < h; y++) {
        const uint *end = pixel + w;
        x = 0;
        while (pixel < end) {
            QColor c;
            c.setHsv(hue, sat, x2val(x));
            *pixel = c.rgb();
            ++pixel;
            ++x;
        }
    }
    pix = QPixmap::fromImage(img);
    QPainter p(this);
    p.drawPixmap(0, 0, pix);
    QPoint pt = QPoint(val2x(val), h / 2);
    QPainterPath path;
    path.addEllipse(QPoint(pt.x(), pt.y()), 2, 2);
    path.addEllipse(QPoint(pt.x(), pt.y()), 4, 4);
    p.fillPath(path, Qt::white);
}

void QColorLuminancePicker::setCol(int h, int s , int v)
{
    val = v;
    val = 255;
    hue = h;
    sat = s;
    repaint();
}
