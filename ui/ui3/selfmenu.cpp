#include "selfmenu.h"
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include <QPainter>
#include <qmath.h>

SelfMenu::SelfMenu(QWidget *parent)
    : QMenu(parent)
{
    init();
}

SelfMenu::SelfMenu(QString title, QWidget *parent)
    : QMenu(title, parent)
{
    init();
}

SelfMenu::~SelfMenu()
{

}

void SelfMenu::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
//    painte
    QPainterPath path;
    float h = this->height();
    float w = this->width();
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // LinearGradient
    QLinearGradient liner(QPoint(0, 0), QPoint(w, h));
    // 132, 216, 209 À¶
    // 47, 107, 117 Ä¬ÈÏ
    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    c1 = conf->getColorIni("color1");
    c2 = conf->getColorIni("color2");
    liner.setColorAt(0, c1);
    liner.setColorAt(1, c2);
    painter.fillPath(path, QBrush(liner));
    painter.setPen(Qt::transparent);
    painter.drawPath(path);
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
    return QMenu::paintEvent(e);
}

void SelfMenu::init()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QFile file;
    file.setFileName(":res/css/menu.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString cssmenu = file.readAll();
        setStyleSheet(cssmenu);
    }
    file.close();
}
