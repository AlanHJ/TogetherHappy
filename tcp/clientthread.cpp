#include "clientopt.h"
#include "clientthread.h"
#include "file/filesend.h"
#include "file/filerecv.h"
#include "connectionopt.h"

class ClientThread::PrivData
{
public:
    PrivData()
    {
        opt = nullptr;
        tcp = nullptr;
        fileRecv = nullptr;
        fileSend = nullptr;
    }
    ~PrivData()
    {

    }
public:
    FileRecv *fileRecv;
    FileSend *fileSend;
    ClientOpt *opt;
    ConnectionOpt *tcp;
};

ClientThread::ClientThread()
{
    mData = new PrivData();
    mData->opt = new ClientOpt();
    mData->tcp = new ConnectionOpt();
    mData->opt->moveToThread(this);
    mData->tcp->moveToThread(this);

    connect(mData->opt, SIGNAL(disconnectToHost()), mData->tcp, SLOT(disconnectToHost()));

    connect(mData->tcp, SIGNAL(recvMessage(NetworkData)), mData->opt, SLOT(recvMessage(NetworkData)));
    connect(mData->tcp, SIGNAL(nextSendingData()), mData->opt, SIGNAL(nextSendingData()));

    connect(mData->tcp, SIGNAL(sendSpeed(float)), mData->opt, SIGNAL(sendSpeed(float)));
    connect(mData->tcp, SIGNAL(recvSpeed(float)), mData->opt, SIGNAL(recvSpeed(float)));

    connect(mData->tcp, SIGNAL(connected()), mData->opt, SLOT(connected()));
    connect(mData->tcp, SIGNAL(disconnected()), mData->opt, SLOT(disconnected()));
    connect(mData->tcp, SIGNAL(sendMessageFail()), mData->opt, SLOT(sendMessageFail()));

    connect(mData->opt, SIGNAL(sendMessage(NetworkData)), mData->tcp, SLOT(sendMessage(NetworkData)));
    connect(this, SIGNAL(setSocketDescriptor(qintptr)), mData->tcp, SLOT(setSocketDescriptor(qintptr)));

    connect(mData->opt, SIGNAL(connectToHost(QString,quint16)), mData->tcp, SLOT(connectToHost(QString,quint16)));

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(finished()), mData->opt, SLOT(deleteLater()));
    connect(this, SIGNAL(finished()), mData->tcp, SLOT(deleteLater()));

    mData->opt->setThread(this);
    this->start();
}

ClientThread::~ClientThread()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

FileSend *ClientThread::createFileSend()
{
    if (nullptr != mData->fileSend)
    {
        delete mData->fileSend;
        mData->fileSend = nullptr;
    }
    mData->fileSend = new FileSend();
    mData->fileSend->setClientOpt(mData->opt);
//    mData->fileSend->moveToThread(this);
//    connect(this, SIGNAL(finished()), mData->fileSend, SLOT(deleteLater()));
    return mData->fileSend;
}

FileSend *ClientThread::fileSendOpt()
{
    return mData->fileSend;
}

FileRecv *ClientThread::createFileRecv()
{
    if (nullptr != mData->fileRecv)
    {
        delete mData->fileRecv;
        mData->fileRecv = nullptr;
    }
    mData->fileRecv = new FileRecv();
    mData->fileRecv->setClientOpt(mData->opt);
//    mData->fileRecv->moveToThread(this);
//    connect(this, SIGNAL(finished()), mData->fileRecv, SLOT(deleteLater()));
    return mData->fileRecv;
}

FileRecv *ClientThread::fileRecvOpt()
{
    return mData->fileRecv;
}

ClientOpt *ClientThread::clientopt()
{
    return mData->opt;
}

ConnectionOpt *ClientThread::socketopt()
{
    return mData->tcp;
}
