#include "emojiinstance.h"
#include "../util/thmethod.h"
#include "../thwidgets/emoji/emojilabel.h"
#include "../thwidgets/emoji/emojitable.h"
#include "emojimanagerui.h"

static EmojiInstance *EmjIntance = nullptr;
class EmojiInstance::PrivData
{
public:
    PrivData()
    {
        init();
    }
    ~PrivData()
    {
//        if (nullptr != emoji)
//        {
//            emoji->deleteLater();
//        }
        if (nullptr != label)
        {
            label->deleteLater();
        }
        if (nullptr != ui)
        {
            ui->deleteLater();
        }
    }
    void init()
    {
//        emoji = new EmojiTable();
//        emoji->hide();

        ui = new EmojiManagerUI();
        ui->hide();
    }

public:
    QString key;
    EmojiLabel *label;
//    EmojiTable *emoji;
    QString css;
    EmojiManagerUI *ui;
};

EmojiInstance::EmojiInstance(QObject *parent) : QObject(parent)
{
    mData = new PrivData();
    mData->label = new EmojiLabel();
    mData->label->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    mData->label->setAttribute(Qt::WA_TranslucentBackground);
    mData->label->setScaledContents(true);
    mData->label->setAlignment(Qt::AlignCenter);

    connect(mData->ui, SIGNAL(currentEmojiPath(QString)),
            this, SLOT(selectedEmoji(QString)));
    connect(this, SIGNAL(updateEmoji(QString)),
            mData->ui, SIGNAL(updateEmoji(QString)));

    connect(mData->ui, SIGNAL(enterEmoji(QString)),
            this, SLOT(enterEmoji(QString)));
    connect(mData->ui, SIGNAL(leaveEmoji()),
            this, SLOT(leaveEmoji()));

}

EmojiInstance::~EmojiInstance()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

EmojiInstance *EmojiInstance::GetInstance()
{
    if (nullptr == EmjIntance)
    {
        EmjIntance = new EmojiInstance();
    }
    return EmjIntance;
}

void EmojiInstance::DesInstance()
{
    if (nullptr != EmjIntance)
    {
        delete EmjIntance;
        EmjIntance = nullptr;
    }
}

void EmojiInstance::showEmoji(QPoint point, QString key)
{
	mData->key = key;
    mData->ui->move(point.x(), point.y() - mData->ui->height());
    mData->ui->setWindowModality(Qt::WindowModal);
    mData->ui->activateWindow();
    mData->ui->show();
}

void EmojiInstance::reloadEmoji()
{
    disconnect(mData->ui, SIGNAL(currentEmojiPath(QString)),
            this, SLOT(selectedEmoji(QString)));

    disconnect(this, SIGNAL(updateEmoji(QString)),
            mData->ui, SIGNAL(updateEmoji(QString)));

    disconnect(mData->ui, SIGNAL(enterEmoji(QString)),
            this, SLOT(enterEmoji(QString)));
    disconnect(mData->ui, SIGNAL(leaveEmoji()),
            this, SLOT(leaveEmoji()));
    mData->ui->close();
    delete mData->ui;
    mData->ui = nullptr;
    mData->ui = new EmojiManagerUI();
    mData->ui->setStyleSheet(mData->css);
    mData->ui->hide();

    connect(mData->ui, SIGNAL(currentEmojiPath(QString)),
            this, SLOT(selectedEmoji(QString)));

    connect(this, SIGNAL(updateEmoji(QString)),
            mData->ui, SIGNAL(updateEmoji(QString)));

    connect(mData->ui, SIGNAL(enterEmoji(QString)),
            this, SLOT(enterEmoji(QString)));
    connect(mData->ui, SIGNAL(leaveEmoji()),
            this, SLOT(leaveEmoji()));
}

void EmojiInstance::setStyleSheet(QString css)
{
    mData->css = css;
    mData->ui->setStyleSheet(css);
}

void EmojiInstance::selectedEmoji(QString url)
{
    emit signalEmojiUrl(url, mData->key);
    mData->ui->hide();
}

void EmojiInstance::enterEmoji(QString path)
{
    mData->label->setMoiveRes(path);
    mData->label->show();
    mData->label->move(mData->ui->x() + mData->ui->width() + 16, mData->ui->y());
}

void EmojiInstance::leaveEmoji()
{
    mData->label->hide();
}

