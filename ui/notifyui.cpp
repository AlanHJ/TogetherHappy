#include "notifyui.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QTimer>
#include <QtWidgets>

NotifyUI::NotifyUI(QWidget *parent)
    : QWidget(parent),
      timerId(-1),
      msTime(DefalutTime),
      valid(false),
      fontsize(DefalutFontSize)
{
    init();
    zoomSize = DefalutFontSize + 10;
    time.start();
    animation = new QPropertyAnimation(this, "aniHeight");
    animationSize = new QPropertyAnimation(this, "aniSize");
}

NotifyUI::~NotifyUI()
{
    qDebug() << "delete";
    animationSize->deleteLater();
}

void NotifyUI::ShowNotify(QString &txt)
{
    NotifyManager::GetInstance()->showNotify(txt);
}

void NotifyUI::Show3sNotify(QString txt, int time)
{
    NotifyUI notfy;
    notfy.setNotifyText(txt);
    notfy.showNotify(time);
}

int NotifyUI::getHeight()
{
    return this->height();
}

void NotifyUI::setHeight(int h)
{
    this->resize(width(), h);
}

int NotifyUI::getSize()
{
    return fontsize;
}

void NotifyUI::setSize(int s)
{
    fontsize = s;
    setNotifyText(text);
}

NotifyUI *NotifyUI::setNotifyText(QString &txt)
{
//#ifdef InheritQWidget
    text = txt;
    QFont font(cns("微软雅黑"));
    font.setPixelSize(fontsize + 2);
    QFontMetrics metrics(font);
    showsize = metrics.size(Qt::TextExpandTabs , txt);
    resize(showsize);
    repaint();
//#else
//    QFont font(cns("微软雅黑"));
//    font.setPixelSize(fontsize + 2);
//    setFont(font);
//    setText(txt);
//#endif
    QPoint p = qApp->desktop()->rect().center();
    move(p.x() - this->width() / 2, p.y() + 200);
    return this;
}

int NotifyUI::showTime()
{
    return msTime;
}

void NotifyUI::showNotify(int ms, NotifyManager::NotifyOrder order)
{
    msTime = ms;
    NotifyManager::GetInstance()->addNotify(this->copy(), order);
}

void NotifyUI::slotTimeOver()
{
#ifdef InheritQWidget
    setNotifyText(cns("在3秒内改变内容，不应该对此创建使用！！！"));
#endif
}

void NotifyUI::hideNotify()
{
    disconnect(animation, 0, 0, 0);
    hide();
}

void NotifyUI::init()
{
    // create ui
    // noly word
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

NotifyUI *NotifyUI::copy()
{
    NotifyUI *ui = new NotifyUI();
    ui->setNotifyText(this->text);
    ui->timerId = this->timerId;
    ui->msTime = this->msTime;
    return ui;
}

void NotifyUI::setValid(bool b)
{
    valid = b;
    if (b)
    {
        timerId = startTimer(10);
    }
}

bool NotifyUI::isValid()
{
    return valid;
}

void NotifyUI::showAnimation()
{
    this->showNormal();
    animation->setDuration(400);
    animation->setStartValue(1);
    animation->setEndValue(showsize.height());
    animation->start();
}

void NotifyUI::closeAnimation()
{
    animation->setDuration(250);
    animation->setStartValue(showsize.height());
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SIGNAL(signalClose()));
    connect(animation, SIGNAL(finished()), this, SLOT(deleteLater()));
    valid = false;
}

void NotifyUI::hideAnimation()
{
    animation->setDuration(250);
    animation->setStartValue(showsize.height());
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(hideNotify()));
}

void NotifyUI::stressAnimation()
{
    animationSize->setDuration(60);
    animationSize->setStartValue(DefalutFontSize);
    animationSize->setEndValue(zoomSize);
    animationSize->start();
    connect(animationSize, SIGNAL(finished()), SLOT(recoverAnimation()));
}

void NotifyUI::recoverAnimation()
{
    disconnect(animationSize, 0, 0, 0);
    animationSize->setDuration(60);
    animationSize->setStartValue(zoomSize);
    animationSize->setEndValue(DefalutFontSize);
    animationSize->start();
}

void NotifyUI::paintEvent(QPaintEvent *)
{
    // paint word
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect;
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(width() - 1);
    rect.setHeight(height() - 1);
    painter.setPen(QColor(0, 0, 0, 0));
    QPainterPath path;
    path.addRoundedRect(rect, 5, 5);
    painter.drawPath(path);
    QColor backColor = ConfigureData::getInstance()->getColorIni("color1");
    painter.fillPath(path, QBrush(backColor));
    painter.setPen(Qt::white);
    QFont font(cns("微软雅黑"));
    font.setPixelSize(fontsize);
    painter.setFont(font);
    painter.drawText(QRect(QPoint(0, 0), this->size()), Qt::AlignCenter, text);

}



void NotifyUI::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerId)
    {
        if (time.elapsed() >= msTime)
        {
            killTimer(timerId);
            emit signalTimeOver();
        }
    }
}
