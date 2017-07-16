#include "searchwidget.h"
#include "editlabel.h"
#include "../../util/thmethod.h"
#include <QPainter>
#include <QtWidgets>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    QLabel *searchIco = new QLabel(this);
    searchIco->setFixedSize(22, 25);
    searchIco->setPixmap(QPixmap(":res/ui3/search_white.png").scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    edit = new SearchLabel(this);
    edit->setFixedSize(183, 23);
    edit->setPlaceholderText(QStringLiteral("ÊäÈë¹Ø¼ü×ÖËÑË÷"));
    QPushButton *clear = new QPushButton(this);
    connect(clear, &QPushButton::clicked, this, [=] () {edit->clear();});
    clear->setFixedSize(15, 15);
    clear->setStyleSheet("QPushButton{border-image:url(:res/ui3/clear.png);background:rgba(245,245,245,0);"
                         "border-width:1px;border-style:solid;border-color:transparent;}"
                         "QPushButton:hover{border-image:url(:res/ui3/clear_hover.png);background:rgba(225,225,225,0);"
                         "border-width:1px;border-style:solid;border-color:transparent;}"
                         "QPushButton:pressed{border-image:url(:res/ui3/clear_press.png);background:rgba(50,50,50,0);"
                         "border-width:1px;border-style:solid;border-color:transparent;}");
    h->addWidget(searchIco, 0, Qt::AlignLeft | Qt::AlignCenter);
    h->addWidget(edit, 10, Qt::AlignLeft | Qt::AlignCenter);
    h->addWidget(clear, 0 , Qt::AlignLeft | Qt::AlignCenter);
    h->addSpacerItem(new QSpacerItem(5, 23));
    setLayout(h);

    setFixedSize(230, 28);
    connect(edit, &SearchLabel::textChanged, clear, [=] () {
        clear->setVisible(!edit->text().isEmpty());
    });
    connect(edit, &SearchLabel::enterPress, this, [=] () {
        emit searchTxtChanged(edit->text());
    });
    clear->setVisible(false);
}

SearchWidget::~SearchWidget()
{

}

void SearchWidget::clear()
{
    edit->clear();
}



void SearchWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.fillPath(path, QBrush(QColor(230, 230, 230, 50)));
    return QWidget::paintEvent(e);
}
