#include "muiltprogressbar.h"
#include <QFile>
#include <QPainter>
#include <QTextCodec>
#include <QPainterPath>
#include "../../util/thmethod.h"

MuiltProgressBar::MuiltProgressBar(QWidget *parent) : QProgressBar(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    percent = 0;
    info = "0MB/s";
    color = QColor(230, 230, 230, 200);
    QString style = cn("QProgressBar{font-family:Î¢ÈíÑÅºÚ;font-size:8px;color:white;"
                       "background:transparent;border: 1px solid rgba(230,230,230,230);"
                       "border-radius: 3px;text-align: center;}"
                       "QProgressBar::chunk{background-color: rgba(20,220,50,255);"
                       "width: 1px;}");
    setStyleSheet(style);
    setTextVisible(false);
}

MuiltProgressBar::~MuiltProgressBar()
{

}

void MuiltProgressBar::reset()
{
    info = "0MB/s";
    setValue(0);
    setSecondValue(0);
}

void MuiltProgressBar::paintEvent(QPaintEvent *e)
{
    QProgressBar::paintEvent(e);
    // paint muilt bar
    QPainter p(this);
    p.setPen(Qt::transparent);
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPainterPath path;
    int w = this->width();
    int h = this->height();
    path.addRoundedRect(0, 0, w, h, 3, 3);
    QLinearGradient lg(0, h / 2, w, h / 2);
    if (percent < 0 || percent > 1) {
        percent = 0;
    }
    if (percent == 1)
    {
        lg.setColorAt(0, color);
        lg.setColorAt(1, color);
    }
    else
    {
        lg.setColorAt(0, color);
        lg.setColorAt(1, Qt::transparent);
        lg.setColorAt(percent, color);
        lg.setColorAt(percent + 0.00001, Qt::transparent);
    }
    p.fillPath(path, QBrush(lg));
    QFont font(cn("Î¢ÈíÑÅºÚ"));
    font.setPixelSize(9);
    QFontMetrics fm(font);
    QString txt = QString("%1% %2").arg(value()).arg(info);
    QRect rect = fm.boundingRect(0, 0, w, h, Qt::AlignCenter, txt);
    p.setPen(QColor(50, 50, 250, 200));
    p.setFont(font);
    p.drawText(rect, txt);
}

void MuiltProgressBar::setInfo(const QString &value)
{
    info = value;
    update();
}

void MuiltProgressBar::setSecondValue(qreal value)
{
    percent = value;
    update();
}


