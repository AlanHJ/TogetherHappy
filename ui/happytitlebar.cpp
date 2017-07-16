#include "happytitlebar.h"
#include <QtWidgets>
#include "../util/thmethod.h"


HappyTitleBar::HappyTitleBar(QWidget *parent) :
    QWidget(parent),
    timerId(-1),
    color(47, 106, 147),
    isScroll(false),
    position(0, 0)
{
    init();
}

HappyTitleBar::~HappyTitleBar()
{

}

void HappyTitleBar::setBarColor(int r, int g, int b)
{
    color.setRgb(r, g, b);
}

void HappyTitleBar::setBarColor(QColor color)
{
    this->color = color;
}

void HappyTitleBar::setBarIcon(QString path, QSize size)
{
    QPixmap pix(path);
    icon->setPixmap(pix.scaled(size));
}

void HappyTitleBar::setBarIcon(QPixmap pix)
{
    icon->setPixmap(pix);
}

void HappyTitleBar::setBarContent(QString content)
{
    this->content->setText(content);
}

void HappyTitleBar::setBarContentScroll(bool b)
{
    isScroll = b;
}

void HappyTitleBar::setBarButtons(HappyTitleBar::BarButtonType type)
{
    setBarButtons(int(type));
}

void HappyTitleBar::setBarButtons(int type)
{
    if (type & MinButtonHint)
    {
        maxButton->setVisible(false);
    }
    if (type & MaxButtonHint)
    {
        minButton->setVisible(false);
    }
    if (type & MinMaxButtonHint)
    {
        minButton->setVisible(true);
        maxButton->setVisible(true);
    }
}

void HappyTitleBar::setBarWidth(int width)
{
    this->setFixedWidth(width - 2 * ShadowWidth);
}

void HappyTitleBar::setExtraButtonStyleSheet(QString css)
{
    this->extraButton->setStyleSheet(css);
}

void HappyTitleBar::setExtraButtonVisible(bool visible)
{
    this->extraButton->setVisible(visible);
}

void HappyTitleBar::setCloseVisible(bool b)
{
    closeButton->setVisible(b);
}

void HappyTitleBar::setMaxIcon()
{
    if (!parentWidget()->isMaximized())
    {
        // max
        maxButton->setStyleSheet("QPushButton{border-image:url(:res/max.png) 0 81 0 0;}"
                             "QPushButton:hover{border-image:url(:res/max.png) 0 54 0 27;}"
                             "QPushButton:pressed{border-image:url(:res/max.png) 0 27 0 54;}");
    }
    else
    {
        maxButton->setStyleSheet("QPushButton{border-image:url(:res/restore.png) 0 81 0 0;}"
                             "QPushButton:hover{border-image:url(:res/restore.png) 0 54 0 27;}"
                             "QPushButton:pressed{border-image:url(:res/restore.png) 0 27 0 54;}");
    }
}

void HappyTitleBar::slotMinButtonClicked()
{
    emit signalMinimize();
}

void HappyTitleBar::slotMaxButtonClicked()
{

    emit signalMaximize();
}

void HappyTitleBar::slotCloseButtonClicked()
{
    emit signalClose();
}

void HappyTitleBar::slotExtraButtonClicked()
{
    emit signalExtra();
}

void HappyTitleBar::init()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    // create ui
    QHBoxLayout *h = new QHBoxLayout();

    icon = new QLabel(this);
    content = new QLabel(this);
    content->setObjectName("QLabelContent");

    minButton = new QPushButton(this);
    minButton->setFixedSize(ButtonWidth, ButtonHeight);
    minButton->setObjectName("ButtonMin");
    connect(minButton, SIGNAL(clicked()),
            this, SLOT(slotMinButtonClicked()));
    maxButton = new QPushButton(this);
    maxButton->setFixedSize(ButtonWidth, ButtonHeight);
    maxButton->setObjectName("ButtonMax");
    connect(maxButton, SIGNAL(clicked()),
            this, SLOT(slotMaxButtonClicked()));
    closeButton = new QPushButton(this);
    closeButton->setObjectName("ButtonClose");
    closeButton->setFixedSize(ButtonWidth, ButtonHeight);
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(slotCloseButtonClicked()));

    extraButton = new QPushButton(this);
    extraButton->setFixedSize(ButtonWidth, ButtonHeight);
    connect(extraButton, SIGNAL(clicked()),
            this, SLOT(slotExtraButtonClicked()));
    // default css
    extraButton->setStyleSheet(
                "QPushButton{border-image:url(:res/ui3/skin.png);background:rgba(245,245,245,0);"
                "border-width:1px;border-style:solid;border-color:transparent;}"
                "QPushButton:hover{border-image:url(:res/ui3/skin.png);background:rgba(225,225,225,45);"
                "border-width:1px;border-style:solid;border-color:transparent;}"
                "QPushButton:pressed{border-image:url(:res/ui3/skin.png);background:rgba(50,50,50,45);"
                "border-width:1px;border-style:solid;border-color:transparent;}"
    );
    // extra button default invisible
//    extraButton->setVisible(false);

    h->addWidget(icon);
    h->addWidget(content);
    h->addWidget(extraButton);
    h->addWidget(minButton);
    h->addWidget(maxButton);
    h->addWidget(closeButton);

    h->setContentsMargins(5, 0, 0, 0);
    h->setSpacing(0);
    setLayout(h);

    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setFixedHeight(BarHeight);

    loadStyleCss(":res/css/BarCss.css");
}

void HappyTitleBar::loadStyleCss(QString css)
{
    QFile file(css);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        setStyleSheet(QString::fromLocal8Bit(ba));
    }
}

void HappyTitleBar::timerEvent(QTimerEvent *)
{
    // content scroll control
}

void HappyTitleBar::mousePressEvent(QMouseEvent *e)
{
    position = e->globalPos();
//    return QWidget::mousePressEvent(e);
}

void HappyTitleBar::mouseReleaseEvent(QMouseEvent *e)
{
    return QWidget::mouseReleaseEvent(e);
}

void HappyTitleBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        emit signalMaximize();
    }
    return QWidget::mouseDoubleClickEvent(e);
}

void HappyTitleBar::mouseMoveEvent(QMouseEvent *e)
{
    QPoint dp = e->globalPos() - position + this->parentWidget()->pos();
    this->parentWidget()->move(dp);
    position = e->globalPos();
//    return QWidget::mouseMoveEvent(e);
}

void HappyTitleBar::paintEvent(QPaintEvent *e)
{
//    QPainter painter(this);
//    painter.setPen(QColor(Qt::transparent));
//    QPainterPath path;
//    path.setFillRule(Qt::WindingFill);
//    QRect crect(0, 0, width(), height());
//    path.addRoundedRect(crect, 3, 3);
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.fillPath(path, QBrush(QColor(47, 106, 147)));
    QWidget::paintEvent(e);
}

