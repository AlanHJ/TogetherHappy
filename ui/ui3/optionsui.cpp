#include "optionsui.h"
#include "../happytitlebar.h"
#include <QtWidgets>
#include "../../config/configuredata.h"
#include "optioncommon.h"
#include "optionnetwork.h"
#include "optionsecurity.h"
#include "optionkeys.h"
#include "optionpersonalization.h"
#include "../notifyui.h"

OptionsUI::OptionsUI(QWidget *parent) : THWidgetBase(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
//    setWindowModality(Qt::ApplicationModal);
    setFixedSize(500, 300);
    setTitleBarWidth(500);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(QStringLiteral("哈啤详细设置"));

    // add list
    list = new QListWidget(this);
    list->setDragEnabled(true);
    list->setDragDropOverwriteMode(true);
    QStringList items;
    items << QStringLiteral("通用")
          << QStringLiteral("网络")
          << QStringLiteral("安全")
          << QStringLiteral("快捷键")
          << QStringLiteral("个性化");
    list->addItems(items);
    for (int i = 0; i < list->count(); i++)
    {
        QListWidgetItem *item = list->item(i);
        item->setTextAlignment(Qt::AlignCenter);
    }
    // add stack
    stack = new QStackedWidget(this);
    OptionCommon *common = new OptionCommon(this);
    OptionNetwork *network = new OptionNetwork(this);
    OptionSecurity *security = new OptionSecurity(this);
    OptionKeys *keys = new OptionKeys(this);
    OptionPersonalization *personal = new OptionPersonalization(this);
    common->setIndex(stack->addWidget(common));
    network->setIndex(stack->addWidget(network));
    security->setIndex(stack->addWidget(security));
    keys->setIndex(stack->addWidget(keys));
    personal->setIndex(stack->addWidget(personal));

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(5, 35, 5, 5);
    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(list, 1);
    h->addWidget(stack, 4);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 5);
    QPushButton *defaultPb = new QPushButton(QStringLiteral("恢复默认"), this);
    QPushButton *applyPb = new QPushButton(QStringLiteral("应用"), this);
    QPushButton *confirmPb = new QPushButton(QStringLiteral("确定"), this);
    QPushButton *cancelPb = new QPushButton(QStringLiteral("取消"), this);
    h2->addSpacing(5);
    h2->addWidget(defaultPb, 2);
    h2->addStretch(5);
    h2->addWidget(applyPb, 1);
    h2->addWidget(confirmPb, 1);
    h2->addWidget(cancelPb, 1);
    h2->addSpacing(5);

    v->addLayout(h, 9);
    v->addLayout(h2, 1);


    QFile file;
    file.setFileName(":res/css/list.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        list->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        defaultPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        applyPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        confirmPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        cancelPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        emit notify();
        this->close();
        this->deleteLater();
    });

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

    connect(list, &QListWidget::currentRowChanged, [=] (int row) {
        stack->setCurrentIndex(row);
    });
    list->setCurrentRow(0);

    connect(applyPb, &QPushButton::clicked, this, [=] () {
        emit apply(stack->currentIndex());
        emit notify();
    });
    connect(confirmPb, &QPushButton::clicked, this, [=] () {
        emit confirm(stack->currentIndex());
        emit bar->signalClose();
    });
    connect(cancelPb, &QPushButton::clicked, this, [=] () {
        emit bar->signalClose();
    });
    connect(defaultPb, &QPushButton::clicked, this, [=] () {
        NotifyUI::ShowNotify(QStringLiteral("暂未启用"));
    });
}

OptionsUI::~OptionsUI()
{

}


void OptionsUI::paintEvent(QPaintEvent *e)
{
    THWidgetBase::paintEvent(e);
    int w = list->width();
    int h = this->height();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::white);
    QPoint p1(w + 6, 35);
    QPoint p2(w + 6, h / 2);
    QLinearGradient linecolor(p1, p2);
    linecolor.setColorAt(0, QColor(255, 255, 255, 80));
    linecolor.setColorAt(0.25, QColor(255, 255, 255, 255));
    linecolor.setColorAt(0.75, QColor(255, 255, 255, 255));
    linecolor.setColorAt(1, QColor(255, 255, 255, 80));
    p.setPen(QPen(QBrush(linecolor), 1));
    p.drawLine(p1, p2);
}
