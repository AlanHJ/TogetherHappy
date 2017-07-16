#include "qtoast.h"
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include <QPainter>
#include <QPainterPath>

QToast::QToast(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    anima1 = new QPropertyAnimation(this, "aniHeight");
    anima2 = new QPropertyAnimation(this, "aniSize");
    QString css = cns("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;");
    setStyleSheet(css);
}

QToast::~QToast()
{

}

void QToast::ShowToast(QString txt)
{
//    QToast *toast = new QToast();
//    toast->setText(txt);
//    QPoint p = qApp->desktop()->rect().center();
//    toast->move(p.x() - this->width() / 2, p.y() + 200);
}

int QToast::getHeight()
{
    return 0;
}

void QToast::setHeight(int /*h*/)
{

}

QSize QToast::getSize()
{
    return QSize();
}

void QToast::setSize(QSize /*s*/)
{

}

void QToast::showAnimation()
{

}

void QToast::hideAnimation()
{

}

void QToast::stressAnimation()
{

}

void QToast::recoverAnimation()
{

}



void QToast::paintEvent(QPaintEvent *e)
{
    // brush background
//    QPainter painter(this);
//    QPainterPath path;
//    float h = this->height();
//    float w = this->width();
//    path.setFillRule(Qt::WindingFill);
//    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    // LinearGradient
//    QLinearGradient liner(QPoint(0, 0), QPoint(w, h));
//    ConfigureData *conf = ConfigureData::getInstance();
//    c1 = conf->getColorIni("color1");
//    c2 = conf->getColorIni("color2");
//    liner.setColorAt(0, c1);
//    liner.setColorAt(1, c2);
//    painter.fillPath(path, QBrush(liner));
//    painter.setPen(Qt::transparent);
//    painter.drawPath(path);
    return QLabel::paintEvent(e);

}
