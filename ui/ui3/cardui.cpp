#include "cardui.h"
#include <QtWidgets>
#include <QPropertyAnimation>
#include "../../config/configuredata.h"
#include "../../thwidgets/emoji/emojilabel.h"
CardUI::CardUI(QWidget *parent)
    : THWidgetBase(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    hideTitleBar();
    // 创建界面
    QHBoxLayout *h = new QHBoxLayout(this);
    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    QImage image(":res/ui3/show_default.gif");
    emoji = new EmojiLabel(this);
    emoji->setMovieable(true);
    emoji->setMoiveRes(":res/ui3/show_default.gif",
                       image.rect().contains(QRect(0, 0, 150, 220)) ? QSize(150, 220) : image.size());
    emoji->setStyleSheet("QLabel{border-radius:3px;"
                         "border-width:1px;border-style:solid;"
                         "border-color:transparent;}");
    name1 = new QLabel(QStringLiteral("昵称名"), this);
    name1->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:bold 14px;color:white;"));
    name2 = new QLabel(QStringLiteral("备注名"), this);
    name2->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    version = new QLabel(QStringLiteral("版本号"), this);
    version->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    ip = new QLabel(QStringLiteral("192.168.155.1"), this);
    ip->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    say = new QLabel(QStringLiteral("葬爱家族的荣耀，将由我们来守护！！！<a style='color: white;' href = www.baidu.com>百度</a>"), this);
    say->setOpenExternalLinks(true);
//    connect(say, &QLabel::linkActivated, this, &CardUI::linkUrl);
    say->adjustSize();
    say->setWordWrap(true);
    say->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    v->addSpacing(1);
    v->addWidget(name1, 0, Qt::AlignCenter);
    v->addWidget(name2, 0, Qt::AlignCenter);
    v->addWidget(version, 0, Qt::AlignCenter);
    v->addWidget(ip, 0, Qt::AlignCenter);
    v->addWidget(say, 8, Qt::AlignTop);
    v->addSpacing(1);

    h->addWidget(emoji, 1, Qt::AlignCenter);
    h->addLayout(v, 1);
    setFixedSize(350, 220);
    animation = new QPropertyAnimation(this, "opacity");

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
}

CardUI::~CardUI()
{
    delete animation;
//    qDebug() << "delete CardUI";
}

qreal CardUI::getopacity()
{
    return this->windowOpacity();
}

void CardUI::setopacity(qreal opacity)
{
    setWindowOpacity(opacity);
}

void CardUI::setPicture(QString path)
{
    QImage image(path);
    emoji->setMoiveRes(path,
                       image.rect().contains(QRect(0, 0, 150, 220)) ? QSize(150, 220) : image.size());
}

void CardUI::setFirstTxt(QString str)
{
    name1->setText(str);
}

void CardUI::setSecondTxt(QString str)
{
    name2->setText(str);
}

void CardUI::setThirdTxt(QString str)
{
    ip->setText(str);
}

void CardUI::setIntroduceTxt(QString str)
{
    say->setText(str);
}

void CardUI::setVersion(float v)
{
    version->setText(QString(QStringLiteral("哈啤%1")).arg(v));
}

void CardUI::setGsoapGet()
{
    // by gsoap get resource
}

void CardUI::showAnimation()
{
    this->show();
}

void CardUI::hideAnimation()
{
    this->close();
    this->deleteLater();
}

void CardUI::linkUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void CardUI::leaveEvent(QEvent *e)
{
    QTimer::singleShot(50, this, [=] () {
        hideAnimation();
    });
    return THWidgetBase::leaveEvent(e);
}

