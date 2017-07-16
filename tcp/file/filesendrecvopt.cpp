#include "filesendrecvopt.h"
#include "filesend.h"
#include "filerecv.h"
#include "../clientthread.h"
#include "../client.h"
#include "../clientopt.h"
#include "../../broadcast/existbroadcast.h"
#include "../../config/configuredata.h"
#include <QStringList>

class FileSendRecvOpt::PrivData
{
public:
    PrivData()
    {
        myKey = ExistBroadcast::getInstance()->localIp().toString() +
                ":" + QString::number(HAPPY_LISTEN_PORT);

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
    ~PrivData()
    {
        fileLst.clear();
        pathLst.clear();
    }
public:
    QStringList fileLst;
    QStringList pathLst;
    QString key;
    QString myKey;
    QString path;
};

FileSendRecvOpt::FileSendRecvOpt()
{
    mData = new PrivData();
    Client *client = Client::GetInstance();
    connect(client, SIGNAL(clientConnetion(QString,ClientOpt*)),
            this, SLOT(slotRecvClientOpt(QString,ClientOpt*)));
}

FileSendRecvOpt::~FileSendRecvOpt()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void FileSendRecvOpt::setKey(QString key)
{
    mData->key = key;
}

bool FileSendRecvOpt::isEmpty()
{
    return mData->pathLst.isEmpty() && mData->fileLst.isEmpty();
}

void FileSendRecvOpt::addFilePath(QString path)
{
    mData->pathLst.append(path);
}

void FileSendRecvOpt::addFileName(QString file)
{
    mData->fileLst.append(file);
}

void FileSendRecvOpt::clearAllPrepareFile()
{
    mData->fileLst.clear();
    mData->pathLst.clear();
}

void FileSendRecvOpt::clearPrepareFileByName(QString name)
{
    mData->fileLst.removeOne(name);
    mData->pathLst.removeOne(name);
}

void FileSendRecvOpt::stopSend()
{
    emit signalStopSend();
}

void FileSendRecvOpt::startSend()
{
    ClientThread *client = new ClientThread();
    client->clientopt()->setMyKey(mData->myKey);
    FileSend *send = client->createFileSend();
    QStringList lst = mData->key.split(":");
    send->setConnectInfo(lst.first(), lst.last().toShort());
    connect(this, SIGNAL(signalStopSend()),
            send, SLOT(stopSend()), Qt::DirectConnection);
    connect(send, SIGNAL(signalSendInfo(QString,QString)),
            this, SIGNAL(sendInformation(QString,QString)));
    connect(send, SIGNAL(signalSendTotalProgress(int)),
            this, SIGNAL(sendFilesProgress(int)));
    connect(send, SIGNAL(signalSendCurrentProgress(int)),
            this, SIGNAL(sendCurrentFileProgress(int)));
    connect(send, SIGNAL(signalSendStart()),
            this, SIGNAL(signalSendStart()));
    connect(send, SIGNAL(signalSendSucess()),
            this, SIGNAL(signalSendSuccess()));

    connect(client->clientopt(), SIGNAL(sendSpeed(float)),
            this, SIGNAL(sendFileSpeed(float)));
    emit send->signalStartSend(mData->fileLst, mData->pathLst);
}

//void FileSendRecvOpt::recvVerify()
//{

//}

void FileSendRecvOpt::fileSavePath(QString path)
{
    mData->path = path;
}

QString FileSendRecvOpt::getSavePath()
{
    return mData->path;
}

void FileSendRecvOpt::stopRecv()
{
    emit signalStopRecv();
}

void FileSendRecvOpt::slotRecvClientOpt(const QString &key, ClientOpt *client)
{
    // 接收线程类
    ClientThread *thread = client->connectThread();
    FileRecv *recv = thread->createFileRecv();
    recv->setNewPath(mData->path);
    if (mData->key != key)
    {
        recv->signalStopRecv();
        return;
    }
    connect(this, SIGNAL(signalStopRecv()),
            recv, SLOT(slotStopRecv()), Qt::DirectConnection);
    connect(recv, SIGNAL(signalRecvInfo(QString,QString)),
            this, SIGNAL(recvInformation(QString,QString)));
    connect(recv, SIGNAL(signalRecvTotalProgress(int)),
            this, SIGNAL(recvFilesProgress(int)));
    connect(recv, SIGNAL(signalRecvCurrentProgress(int)),
            this, SIGNAL(recvCurrentFileProgress(int)));

    connect(recv, SIGNAL(signalRecvStart()),
            this, SIGNAL(signalRecvStart()));
    connect(recv, SIGNAL(signalRecvSucess()),
            this, SIGNAL(signalRecvSuccess()));
    connect(recv, SIGNAL(signalRecvPreInfo(QStringList)),
            this, SIGNAL(signalRecvPreInfo(QStringList)));

    connect(thread->clientopt(), SIGNAL(recvSpeed(float)),
            this, SIGNAL(recvFileSpeed(float)));
    // 预处理完成，发送开始接收信号
    emit signalRecvStart();
}
