#include "filesend.h"
#include "../clientopt.h"
#include "../../data/filedatacontent.h"
#include "../../data/messagedata.h"
#include "../../data/messagechardata.h"
#include "../../broadcast/existbroadcast.h"
#include "../clientthread.h"
#include <QTime>
#include <QDir>
#include <QThread>
#include <QSemaphore>
#include <QAbstractEventDispatcher>

class FileSend::PrivData
{
public:
    PrivData() : semaphore(1)
    {
        thread = new QThread();
        thread->setObjectName("send thread");
        isEnabled = false;
        client = nullptr;
        key = ExistBroadcast::getInstance()->localIp().toString() +
                ":" + QString::number(HAPPY_LISTEN_PORT);
    }
    ~PrivData()
    {
        fileLst.clear();
        pathLst.clear();
    }

public:
    bool isEnabled;
    qint16 port;
    QString ip;
    QString key;
    ClientOpt *client;

    QStringList fileLst;

    QStringList pathLst;

    QThread *thread;

    QSemaphore semaphore;
};

FileSend::FileSend(QObject *parent) : QObject(parent)
{
    mData = new PrivData();
    this->moveToThread(mData->thread);
    connect(this, SIGNAL(signalStartSend(QStringList,QStringList)),
            this, SLOT(slotStartSend(QStringList,QStringList)));
    connect(this, SIGNAL(signalQuit()),
            this, SLOT(slotQuit()));
    connect(mData->thread, SIGNAL(finished()),
            this, SLOT(deleteLater()));
    connect(mData->thread, SIGNAL(finished()),
            mData->thread, SLOT(deleteLater()));
    mData->thread->start();
}

FileSend::~FileSend()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void FileSend::setConnectInfo(QString ip, qint16 port)
{
    mData->ip = ip;
    mData->port = port;
}

void FileSend::setClientOpt(ClientOpt *opt)
{
    mData->client = opt;
}

void FileSend::slotQuit()
{
    thread()->quit();
//    thread()->wait();
//    ClientThread *t = mData->client->connectThread();
//    t->quit();
//    t->wait();
}

void FileSend::nextData()
{
    mData->semaphore.release(1);
    if (mData->semaphore.available() > 1)
    {
        mData->semaphore.acquire(mData->semaphore.available() - 1);
    }
}

void FileSend::readyForSend(QString key)
{
    QString ks = mData->ip + ":" + QString::number(mData->port);
    if (ks != key)
    {
        // 出现问题
        emit signalSendInfo(cns("错误"), cns("连接验证失败"));
        emit signalQuit();
        return;
    }
    // 开始准备
    emit signalSendStart();

    
    // 发送预览数据
    MessageData msgdata;
    msgdata.setDatatype(FileHead);
    MessageCharData *stringdata = new MessageCharData();
    QString strs;
    for (QString str : mData->fileLst)
    {
        if (strs.isEmpty() == false)
        {
            strs.append(";");
        }
        strs.append(str);
    }
    for (QString str : mData->pathLst)
    {
        if (strs.isEmpty() == false)
        {
            strs.append(";");
        }
        strs.append(str);
    }
    stringdata->setContent(strs.toLocal8Bit());
    msgdata.setContent(stringdata);
    if (nullptr != mData->client)
    {
        emit mData->client->sendMsgData(msgdata);
        mData->semaphore.release(1);
    }
    
    emit signalSendInfo(cns("信息"), cns("开始检索需要发送的文件"));
    // 获取所有需要发送的数据
    for (QString path : mData->pathLst)
    {
        QDir dir(path);
        dir.cdUp();
        QString cdpath = dir.absolutePath();
        getAllFileLst(path, cdpath, &mData->fileLst);
    }
    emit signalSendInfo(cns("信息"), cns("发送文件检索完毕，准备发送"));
    // 开始发送文件数据

    // 开启发送计数

    for (int i = 0; i < mData->fileLst.size(); i++)
    {
        QStringList lst = mData->fileLst.at(i).split("*:*");
        QString path;
        QString name;
        name = lst.last();
        path = lst.first();
        if (lst.size() > 2)
        {
            name.clear();
            for (int j = 1; j < lst.size(); j++)
            {
                name.append(lst.at(j));
            }
        }
        QFile file(name);
        emit signalSendInfo(cns("信息"), /*cns("当前文件:") +*/ QFileInfo(name).fileName());
        if (file.exists() == false)
        {
            // 该文件名的文件不存在
            emit signalSendInfo(cns("警告"), cns("当前文件不存在:") + name);
            continue;
        }
        if (file.open(QIODevice::ReadOnly) == false)
        {
            // 该文件无法被读取
            emit signalSendInfo(cns("警告"), cns("当前文件无法读取:") + name);
            continue;
        }
        QDataStream stream(&file);
        qint64 filesize = file.size();
        qint64 sendSize = 0;

        if (filesize == 0)
        {
            // 空文件
            MessageData msg;
            FileDataContent *content = new FileDataContent();
            FileDataContent::FileHead filehead;
            filehead.total = mData->fileLst.size();
            filehead.currentIndex = i + 1;
            float f1 = filehead.currentIndex;
            float f2 = filehead.total;
            filehead.totalProgress = f1 / f2  * 100;
            float f3 = sendSize + 1;
            float f4 = filesize + 1;
            filehead.currentProgress = f3 / f4 * 100;
            filehead.fileCurrentSize = 0;
            filehead.fileToalSize = 0;
            emit signalSendTotalProgress(filehead.totalProgress);
            emit signalSendCurrentProgress(filehead.currentProgress);
            content->setSendInfo(filehead);
            if (lst.size() == 1)
            {
                path = "replace";
                QFileInfo info(name);
                name = path + info.fileName();
            }
            content->setFileName(path, name);
            msg.setDatatype(content->datatype());
            msg.setContent(content);
            if (nullptr == mData->client)
            {
                // 抛出异常
                emit signalSendInfo(cns("错误"), cns("连接操作异常"));
                break;
            }
            emit mData->client->sendMsgData(msg);
        }

        for (qint64 k = 0; k < filesize;)
        {
            while (mData->semaphore.tryAcquire(1) == false)
            {
                thread()->msleep(1);
                QThread::currentThread()->eventDispatcher()->processEvents (QEventLoop::AllEvents);
                QTime time;
                time.start();
                if (time.elapsed() > 3000)
                {
                    mData->semaphore.release(1);
                    break;
                }
            }
            char *temp = new char[MAX_SEND_SIZE];
            int ret = stream.readRawData(temp, MAX_SEND_SIZE);
            sendSize += ret;
            // 打包数据
            MessageData msg;
            FileDataContent *content = new FileDataContent();
            FileDataContent::FileHead filehead;
            filehead.total = mData->fileLst.size();
            filehead.currentIndex = i + 1;
            float f1 = filehead.currentIndex;
            float f2 = filehead.total;
            filehead.totalProgress = f1 / f2  * 100;
            float f3 = sendSize;
            float f4 = filesize;
            filehead.currentProgress = f3 / f4 * 100;
            filehead.fileCurrentSize = ret;
            filehead.fileToalSize = filesize;
            emit signalSendTotalProgress(filehead.totalProgress);
            emit signalSendCurrentProgress(filehead.currentProgress);
            content->setSendInfo(filehead);
            if (lst.size() == 1)
            {
                path = "replace";
                QFileInfo info(name);
                name = path + info.fileName();
            }
            content->setFileName(path, name);
            content->setData(temp);
            msg.setDatatype(content->datatype());
            msg.setContent(content);
            delete[] temp;
            k += ret;
            if (nullptr == mData->client)
            {
                // 抛出异常
                emit signalSendInfo(cns("错误"), cns("连接操作异常"));
                break;
            }
            emit mData->client->sendMsgData(msg);
            if (mData->isEnabled == false)
            {
                break;
            }
        }
        file.close();
        if (mData->isEnabled == false)
        {
            break;
        }
    }
    // 发送结束了
    emit signalSendInfo(cns("信息"), cns("文件发送结束"));
    emit signalSendSucess();
//    emit signalQuit();
}

void FileSend::slotStartSend(QStringList fileLst, QStringList pathLst)
{
    // 重置标志位
    mData->isEnabled = true;
    // 建立连接
    emit signalSendInfo(cns("信息"), cns("建立连接"));
    emit mData->client->connectToHost(mData->ip, mData->port);
    connect(mData->client, SIGNAL(readyForUse(QString)),
            this, SLOT(readyForSend(QString)));
    connect(mData->client, SIGNAL(nextSendingData()),
            this, SLOT(nextData()), Qt::DirectConnection);
    mData->fileLst = fileLst;
    mData->pathLst = pathLst;
}

void FileSend::getAllFileLst(QString path, QString cdPath, QStringList *list)
{
    QDir dir(path);
    QFileInfoList lst = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    auto it = lst.begin();
    while (it != lst.end())
    {
        if (mData->isEnabled == false)
        {
            break;
        }
        QFileInfo info = *it;
        if (info.isDir())
        {
            getAllFileLst(info.absoluteFilePath(), cdPath, list);
        }
        else if (info.isFile())
        {
            list->append(cdPath + "*:*" + info.absoluteFilePath());
            emit signalSendInfo(cns("信息"), cns("检索到") +
                                QString::number(list->size()) +
                                cns("个文件"));
        }
        ++it;
    }
}

void FileSend::stopSend()
{
    mData->isEnabled = false;
    emit signalQuit();
}

