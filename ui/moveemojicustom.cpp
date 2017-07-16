#include "moveemojicustom.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QDir>
#include <QThread>
#include <QFileInfo>

static MoveEmojiCustom *MoveCustom = nullptr;
static QMutex MoveMutex;
class MoveEmojiCustom::PrivData
{
public:
    PrivData()
    {
        thread = new QThread();
        thread->setObjectName("move thread");
    }
    ~PrivData()
    {
        thread->quit();
    }
public:
    QThread *thread;
};

MoveEmojiCustom::MoveEmojiCustom()
{
    mData = new PrivData();
    this->moveToThread(mData->thread);
    connect(mData->thread, SIGNAL(finished()), mData->thread, SLOT(deleteLater()));
    connect(this, SIGNAL(signalStartMove(QString)), this, SLOT(slotStartMove(QString)));
    connect(this, SIGNAL(signalStartMoves(QStringList)), this, SLOT(slotStartMoves(QStringList)));
    mData->thread->start();
}

MoveEmojiCustom::~MoveEmojiCustom()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

MoveEmojiCustom *MoveEmojiCustom::GetInstance()
{
    QMutexLocker lock(&MoveMutex);
    if (nullptr == MoveCustom)
    {
        MoveCustom = new MoveEmojiCustom();
    }
    return MoveCustom;
}

void MoveEmojiCustom::DesInstance()
{
    QMutexLocker lock(&MoveMutex);

    if (nullptr != MoveCustom)
    {
        delete MoveCustom;
        MoveCustom = nullptr;
    }
}

void MoveEmojiCustom::slotStartMove(QString res)
{
    QMutexLocker lock(&MoveMutex);
    QString oldres = res;
    QString newres = res;
    // 开始迁移表情包
    // 是否存在文件夹
    QString path = GetWorkPath();
//    ConfigureData::getInstance()->getIni("path", path);
    QDir dir;
    dir.mkpath(path + "/custom");
    path = path + "/custom";
    QStringList lst = res.split(";");
    if (lst.isEmpty())
    {
        emit signalMoveInfo(cns("没有资源文件"));
        emit signalProgress(100);
        return;
    }
    for (int i = 0; i < lst.size(); i++)
    {
        QString str = lst.at(i);
        QString num = QString::number(i) + ".";
        emit signalMoveInfo(cns("当前迁移:") + num + str);
        QFile file(str);
        if (file.exists())
        {
            QFileInfo info(file.fileName());
            QString name = info.fileName();
            QFile ofile(path + name);
            ofile.remove();
            if (file.copy(path + name))
            {
                newres = newres.replace(str, path + name);
                emit signalMoveInfo(name + cns(" 迁移成功"));
            }
            else
            {
                emit signalMoveInfo(name + cns(" 迁移失败"));
            }
        }
        else
        {
            emit signalMoveInfo(cns("当前:") + str + cns(" 不存在"));
        }
        file.close();
        float f1 = i + 1;
        float f2 = lst.size();
        int value = f1 / f2 * 100;
        emit signalProgress(value);
    }

    // 重新写入emoji.data文件
    QString content;
    QFile datafile("D:/Program Files/happy/emoji.data");
    if (datafile.open(QIODevice::ReadOnly))
    {
        QByteArray ba = datafile.readAll();
        content = QString::fromLocal8Bit(ba);
    }
    datafile.close();
    datafile.remove();
    content = content.replace(oldres, newres);
    if (datafile.open(QIODevice::WriteOnly))
    {
        datafile.write(content.toLocal8Bit());
    }
    datafile.close();

    emit signalProgress(101);
}

void MoveEmojiCustom::slotStartMoves(QStringList lst)
{
    QString temp;
    for (QString str : lst)
    {
        if (temp.isEmpty() == false)
        {
            temp.append(";");
        }
        temp.append(str);
    }
    slotStartMove(temp);
}

