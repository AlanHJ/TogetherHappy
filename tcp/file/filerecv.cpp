#include "filerecv.h"
#include "../clientopt.h"
#include "../clientthread.h"
#include "../../data/filedatacontent.h"
#include "../../data/messagechardata.h"
#include "../../config/configuredata.h"
#include <QFile>
#include <QDir>

class FileRecv::PrivData
{
public:
    PrivData()
    {
        index = 0;
        file = nullptr;
        client = nullptr;
        getDefaultPath();
        isEnabled = true;
        thread = new QThread();
        thread->setObjectName("recv thread");
    }
    ~PrivData()
    {

    }
    void getDefaultPath()
    {
        path = "D:";
//        ConfigureData::getInstance()->getIni("path", path);
        QDir dir;
        path += "/recv/";
        if (dir.exists(path) == false)
        {
            bool b = dir.mkdir(path);
            if (!b)
            {
                path = "C:/";
            }
        }
    }
public:
    int index;
    QFile *file;
    ClientOpt *client;
    QString path;
    bool isEnabled;
    QThread *thread;
};

FileRecv::FileRecv(QObject *parent) : QObject(parent)
{
    mData = new PrivData();
    this->moveToThread(mData->thread);
    connect(this, SIGNAL(signalStopRecv()),
            this, SLOT(slotStopRecv()));
    connect(this, SIGNAL(signalQuit()),
            this, SLOT(slotQuit()));
    connect(mData->thread, SIGNAL(finished()),
            this, SLOT(deleteLater()));
    connect(mData->thread, SIGNAL(finished()),
            mData->thread, SLOT(deleteLater()));
    mData->thread->start();
}

FileRecv::~FileRecv()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void FileRecv::setNewPath(QString path)
{
    mData->path = path;
}

void FileRecv::setClientOpt(ClientOpt *opt)
{
    mData->client = opt;
    connect(mData->client, SIGNAL(sendFileMessage(MessageData)),
            this, SLOT(getFileConntent(MessageData)));
}

void FileRecv::slotQuit()
{
    if (nullptr != mData->client)
    {
        emit mData->client->disconnectToHost();
//        ClientThread *t = mData->client->connectThread();
//        t->quit();
//        t->wait();
    }
    thread()->quit();
//    thread()->wait();
}

void FileRecv::slotStopRecv()
{
    // 停止接收数据
    if (nullptr != mData->client)
    {
        emit mData->client->disconnectToHost();
    }
    emit signalQuit();
}

void FileRecv::getFileConntent(MessageData msg)
{
    // 获取文件内容并生成文件
    if (msg.datatype() != FileSendType && msg.datatype() != FileHead)
    {
        return;
    }

    if (msg.datatype() == FileHead)
    {
        MessageCharData *chardata = dynamic_cast<MessageCharData *>(msg.content());
        if (nullptr != chardata)
        {
            QString str = QString::fromLocal8Bit(chardata->content());
            QStringList lst = str.split(";");
            emit signalRecvPreInfo(lst);
        }
        return;
    }


    FileDataContent *content = dynamic_cast<FileDataContent *>(msg.content());
    if (nullptr == content)
    {
        return;
    }
    // 解析数据
    FileDataContent::FileHead *filehead = content->sendInfo();

    FileDataContent::FileNameHead *namehead = content->fileName();
    char *data = content->data();
    int length = filehead->fileCurrentSize;
    QString name = namehead->name;
    QString path = namehead->path;
    if (mData->index != filehead->currentIndex)
    {
        mData->index = filehead->currentIndex;
        if (nullptr != mData->file)
        {
            mData->file->close();
            delete mData->file;
            mData->file = nullptr;
        }
        QString newName = name.replace(path, mData->path);

        // 创建路径
        QFileInfo info(newName);
        QString newPath = info.absolutePath();
        QDir dir;
        if (dir.exists(newPath) == false)
        {
            // 一级一级创建所有目录
            QString temp = newPath;
            QDir dir(temp);
            while (dir.mkpath(newPath) == false)
            {
                dir.cdUp();
                dir.mkdir(dir.absolutePath());
            }
//            dir.mkdir(newPath);
        }
        // 是否存在源文件，有则删除
        QFile file(newName);
        if (file.exists())
        {
            file.remove();
        }
        // 创建文件
        emit signalRecvInfo(cns("信息"), /*cns("当前文件:") +*/ info.fileName());
        mData->file = new QFile();
        mData->file->setFileName(newName);
        bool b = mData->file->open(QIODevice::WriteOnly | QIODevice::Append);
        if (!b)
        {
            // 文件打开错误，抛出异常
            emit signalRecvInfo(cns("警告"), cns("文件创建打开失败：") + info.fileName());
            mData->file->close();
            delete mData->file;
            mData->file = nullptr;
        }
    }
    if (nullptr == mData->file)
    {
        // 文件为空，无法写入数据
        emit signalRecvInfo(cns("警告"), cns("该文件无法被写入：") + name);
        return;
    }
    if (mData->file->isOpen() && mData->file->isWritable())
    {
//        QDataStream stream(mData->file);
//        int ret = stream.writeRawData(data, length);
        int ret = mData->file->write(data, length);
        if (ret != length)
        {
            // 写入数据不完整
            mData->file->close();
            delete mData->file;
            mData->file = nullptr;
            return;
        }
        else
        {
            emit signalRecvTotalProgress(filehead->totalProgress);
            emit signalRecvCurrentProgress(filehead->currentProgress);
        }
    }

    if (filehead->currentProgress == 100)
    {
        mData->file->close();
        delete mData->file;
        mData->file = nullptr;
    }

    if (filehead->currentIndex == filehead->total &&
            filehead->totalProgress == 100 &&
            filehead->currentProgress == 100)
    {
        // 所有文件接收完毕
//        mData->file->close();
//        delete mData->file;
//        mData->file = nullptr;
        emit signalRecvInfo(cns("信息"), cns("文件接收结束"));
        emit signalRecvSucess();
        emit signalQuit();
    }
}

