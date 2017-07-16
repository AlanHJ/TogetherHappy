#include "topwidget.h"
#include "../../thwidgets/happytreewidget.h"
#include <QtWidgets>

TopWidget::TopWidget(QTreeWidgetItem *item,
                     HappyTreeWidget *tree) : QWidget(tree)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    arrow = new QLabel(this);
    arrow->setPixmap(QPixmap(":res/ui3/arrow_right.png").scaled(25, 25));
    arrow->setFixedSize(25, 25);
    txt = new QLabel(QStringLiteral("Ä¬ÈÏ·Ö×é"), this);
    txt->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:14px;color:white;"));
    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(arrow, 0, Qt::AlignLeft | Qt::AlignVCenter);
    h->addWidget(txt, 9, Qt::AlignLeft | Qt::AlignVCenter);
    setLayout(h);
//    setFixedSize(250, 10);
    connect(tree, &HappyTreeWidget::itemDoubleClicked, [=] (QTreeWidgetItem *it, int /*column*/) {
        if (it != item)
            return;
        slotExpanded(!item->isExpanded());
    });
    connect(tree, &HappyTreeWidget::itemExpanded, [=] (QTreeWidgetItem *it) {
        if (it != item)
            return;
        slotExpanded(true);
    });
}

TopWidget::~TopWidget()
{

}

void TopWidget::setText(QString str)
{
    txt->setText(str);
}

void TopWidget::slotExpanded(bool b)
{
    if (b)
    {
        arrow->setPixmap(QPixmap(":res/ui3/arrow_down.png").scaled(25, 25));
    }
    else
    {
        arrow->setPixmap(QPixmap(":res/ui3/arrow_right.png").scaled(25, 25));
    }
}

