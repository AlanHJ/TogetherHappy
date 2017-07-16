#include "emojimanagerui.h"
#include "../util/thmethod.h"
#include "../thwidgets/emoji/emojitable.h"
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

EmojiManagerUI::EmojiManagerUI()
{
    init();
    setWindowFlags(Qt::Tool);
    setWindowTitle(cns("哈啤表情管理"));
}

EmojiManagerUI::~EmojiManagerUI()
{

}

void EmojiManagerUI::tabClicked()
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    if (nullptr == pb)
    {
        return;
    }
    int index = pb->objectName().toInt();
    tab->setCurrentIndex(index);
}

void EmojiManagerUI::editClicked()
{
    QMessageBox::information(this, cns("提示"), cns("开发中..."), cns("吼啊"));
}

void EmojiManagerUI::init()
{
    // 初始化表情管理
    QFrame *frame = new QFrame(this);
    tab = new QTabWidget(this);
    tab->tabBar()->hide();
    tab->setTabPosition(QTabWidget::South);
    tab->setMovable(true);
    // 增加自定义表情栏
    EmojiTable *emoji1 = new EmojiTable(this);
    QStringList lst1;
    QString path = "D:/Program Files/happy/emoji.data";
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        QString s = QString::fromLocal8Bit(ba);
        QStringList lst = s.split(";");
        for (int i = 0; i < lst.size(); i++)
        {
            QString imagepath = lst.at(i);
            QFile testfile(imagepath);
            if (testfile.exists())
            {
                lst1 << imagepath;
            }
        }
    }
    file.close();
//    emoji1->insertEmoji(lst1);

    // tieba
    EmojiTable *emoji4 = new EmojiTable(this);
    QStringList lst4;
    QString tieba = ":/res/baidu/%1.png";
    for (int i = 0; i < 72; i++)
    {
        lst4 << tieba.arg(i + 1);
    }
//    emoji4->insertEmoji(lst4);
    // huaji
    EmojiTable *emoji5 = new EmojiTable(this);
    QStringList lst5;
    QString huaji = ":/res/huaji/%1.png";
    for (int i = 0; i < 64; i++)
    {
        lst5 << huaji.arg(i + 1);
    }
//    emoji5->insertEmoji(lst5);
    // newqq
    EmojiTable *emoji6 = new EmojiTable(this);
    QStringList lst6;
    QString newqq = ":/res/newqq/%1.png";
    for (int i = 0; i < 109; i++)
    {
        lst6 << newqq.arg(i + 1);
    }
//    emoji6->insertEmoji(lst6);
    // money
    EmojiTable *emoji7 = new EmojiTable(this);
    QStringList lst7;
    QString money = ":/res/money/%1.png";
    for (int i = 0; i < 20; i++)
    {
        lst7 << money.arg(i + 1);
    }
//    emoji7->insertEmoji(lst7);

    // QQ表情
    EmojiTable *emoji2 = new EmojiTable(this);
    QStringList lst2;
    QString qqpath = ":/res/qqemoji/%1.gif";
    for (int i = 0; i < 132; i++)
    {
        lst2 << qqpath.arg(i + 1);
    }
//    emoji2->insertEmoji(lst2);
    // 梦幻表情
    EmojiTable *emoji3 = new EmojiTable(this);
    QStringList lst3;
    QString mhpath = ":/res/mhxy/%1.gif";
    for (int i = 0; i < 210; i++)
    {
        lst3 << mhpath.arg(i + 1);
    }
//    emoji3->insertEmoji(lst3);

    tab->insertTab(0, emoji1, cns("收藏"));
    tab->insertTab(1, emoji4, cns("微博"));
    tab->insertTab(2, emoji5, cns("滑稽"));
    tab->insertTab(3, emoji6, cns("扣扣"));
    tab->insertTab(4, emoji7, cns("打赏"));
    tab->insertTab(5, emoji2, cns("企鹅"));
    tab->insertTab(6, emoji3, cns("梦幻"));

    QPushButton *pb1 = new QPushButton(cns("收藏"), frame);
    pb1->setObjectName(QString::number(0));
    connect(pb1, SIGNAL(clicked()), this, SLOT(tabClicked()));

    QPushButton *pb8 = new QPushButton(cns("微博"), frame);
    pb8->setObjectName(QString::number(1));
    connect(pb8, SIGNAL(clicked()), this, SLOT(tabClicked()));
    QPushButton *pb5 = new QPushButton(cns("滑稽"), frame);
    pb5->setObjectName(QString::number(2));
    connect(pb5, SIGNAL(clicked()), this, SLOT(tabClicked()));
    QPushButton *pb6 = new QPushButton(cns("扣扣"), frame);
    pb6->setObjectName(QString::number(3));
    connect(pb6, SIGNAL(clicked()), this, SLOT(tabClicked()));
    QPushButton *pb7 = new QPushButton(cns("打赏"), frame);
    pb7->setObjectName(QString::number(4));
    connect(pb7, SIGNAL(clicked()), this, SLOT(tabClicked()));

    QPushButton *pb2 = new QPushButton(cns("企鹅"), frame);
    pb2->setObjectName(QString::number(5));
    connect(pb2, SIGNAL(clicked()), this, SLOT(tabClicked()));
    QPushButton *pb3 = new QPushButton(cns("梦幻"), frame);
    pb3->setObjectName(QString::number(6));
    connect(pb3, SIGNAL(clicked()), this, SLOT(tabClicked()));
    QPushButton *pb4 = new QPushButton(cns("管理"), this);
    connect(pb4, SIGNAL(clicked()), this, SLOT(editClicked()));

    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    v->addWidget(tab);
    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(pb1, 0, Qt::AlignLeft);
    h->addWidget(pb8, 0, Qt::AlignLeft);
    h->addWidget(pb5, 0, Qt::AlignLeft);
    h->addWidget(pb6, 0, Qt::AlignLeft);
    h->addWidget(pb7, 0, Qt::AlignLeft);
    h->addWidget(pb2, 0, Qt::AlignLeft);
    h->addWidget(pb3, 0, Qt::AlignLeft);
    frame->setLayout(h);
    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    h2->addWidget(frame, 0, Qt::AlignLeft);
    h2->addWidget(pb4, 0, Qt::AlignRight);
    v->addLayout(h2);
    setLayout(v);

    setFixedSize(emoji1->width(), emoji1->height() + 25);

    connect(emoji1, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji1, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji1, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));

    connect(this, SIGNAL(updateEmoji(QString)),
            emoji1, SLOT(updateEmoji(QString)));

    connect(emoji2, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji2, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji2, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));

    connect(emoji3, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji3, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji3, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));

    connect(emoji5, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji5, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji5, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));

    connect(emoji6, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji6, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji6, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));

    connect(emoji7, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji7, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji7, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));

    connect(emoji4, SIGNAL(currentEmojiPath(QString)),
            this, SIGNAL(currentEmojiPath(QString)));
    connect(emoji4, SIGNAL(enterEmoji(QString)),
            this, SIGNAL(enterEmoji(QString)));
    connect(emoji4, SIGNAL(leaveEmoji()),
            this, SIGNAL(leaveEmoji()));
}

