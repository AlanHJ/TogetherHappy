#include "itemeditor.h"
#include <QtWidgets>
#include "../emoji/emojilabel.h"
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"

ItemEditor::ItemEditor(QWidget *parent) : QWidget(parent)
{
    timerid = -1;
    itemType = realityBody;
    init();
}

ItemEditor::~ItemEditor()
{

}

void ItemEditor::addTimeSign()
{
    other->show();
    other->clear();
    other->setText(QDateTime::currentDateTime().toString("hh:mm"));
}

void ItemEditor::setUid(QString id)
{
    this->uid = id;
}

void ItemEditor::setName(QString name)
{
//    int len = name.length();
//    QString css = cns("QLabel{font-family:Î¢ÈíÑÅºÚ;font:%1px;color:white;"
//                      "}");
//    if (len <= 8)
//    {
//        this->name->setStyleSheet(css.arg("14"));
//    }
//    else
//    {
//        this->name->setStyleSheet(css.arg("10"));
//    }
    QFont font(cn("Î¢ÈíÑÅºÚ"));
    font.setPointSize(14);
    QFontMetrics fm(font);
    this->name->setText(fm.elidedText(name, Qt::ElideMiddle, 160));
    this->name->setToolTip(name);
}

QString ItemEditor::getName()
{
    return name->toolTip();
}

QString ItemEditor::getUid()
{
    return uid;
}

void ItemEditor::setItemTyle(ItemEditor::ItemType type)
{
    itemType = type;
}

void ItemEditor::starNotify()
{
    emit sendNotify(emoji->ImagePath());
//    stopNotify();
//    timerid = startTimer(400);
}

void ItemEditor::stopNotify()
{
//    if (timerid != -1)
//    {
//        killTimer(timerid);
//        timerid = -1;
    //    }
}

void ItemEditor::showMaxMin(bool min)
{
    if (min)
    {
        emojiSize = ItemMinHeight;
        h2->itemAt(0)->setAlignment(Qt::AlignVCenter);
        h2->itemAt(1)->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
//        h2->addStretch(1);
    }
    else
    {
        emojiSize = ItemMaxHeight;
        h2->itemAt(0)->setAlignment(Qt::AlignBottom);
        h2->itemAt(1)->setAlignment(Qt::AlignBottom | Qt::AlignRight);
//        h2->addStretch(1);
    }
    introduce->setVisible(!min);
    emoji->setMoiveRes(emoji->ImagePath(),
                       QSize(emojiSize - 10, emojiSize - 10));
    emoji->setFixedSize(emojiSize, emojiSize);
    setFixedSize(250, emojiSize);
}

void ItemEditor::init()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // create default ui
    emoji = new EmojiLabel(this);
    emoji->setMovieable(true);
    name = new QLabel(this);
    introduce = new QLabel(this);
    other = new QLabel(this);
//    other->setVisible(false);

    h1 = new QHBoxLayout();
    h1->setSpacing(1);
    h1->setContentsMargins(0, 0, 0, 0);
    h2 = new QHBoxLayout();
    h2->setSpacing(1);
    h2->setContentsMargins(0, 0, 0, 0);
    v1 = new QVBoxLayout();
    v1->setSpacing(1);
    v1->setContentsMargins(0, 0, 0, 0);

    h2->addWidget(name, 0, Qt::AlignBottom);
    h2->addWidget(other, 0, Qt::AlignBottom | Qt::AlignRight);
    h2->addStretch(1);

    v1->addLayout(h2);
    v1->addWidget(introduce, 0, Qt::AlignTop);

    h1->addWidget(emoji, 1, Qt::AlignCenter);
    h1->addLayout(v1, 5);

    setLayout(h1);

    setMinimumHeight(ItemMinHeight);
    setMaximumHeight(ItemMaxHeight);
    name->setFixedWidth(120);
    other->setFixedWidth(35);
    setFixedWidth(250);


    // add default resource
    emoji->setEnabled(false);
    emoji->setMoiveRes(":res/baidu/15.png");
    emoji->setAlignment(Qt::AlignCenter);

    QFile file;
    file.setFileName(":res/css/label.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        name->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba).arg(14));
    }
    file.close();
    name->setText(QStringLiteral(""));
    introduce->setStyleSheet(cns("QLabel{font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"
                            "}"));
    introduce->setText(QStringLiteral(""));
    other->setStyleSheet(cns("QLabel{font-family:Î¢ÈíÑÅºÚ;font:10px;color:white;"
                             "}"));
//    other->setPixmap(QPixmap(":res/money/13.png").scaled(20, 20));
    other->setText(QStringLiteral("ÔÚÏß"));
    setFocusPolicy(Qt::NoFocus);

    myuid = ConfigureData::getInstance()->getUuid();
}



void ItemEditor::timerEvent(QTimerEvent *e)
{
    if (e->timerId())
    {
        emit sendNotify(emoji->ImagePath());
    }
}
