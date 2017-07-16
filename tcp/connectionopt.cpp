#include "connectionopt.h"
#include "happyconnection.h"
#include <QDebug>
#include <QTime>
#include <QThread>
#include "../util/thmethod.h"
#include <QAbstractEventDispatcher>

class ConnectionOpt::PrivData
{
public:
    PrivData()
    {
        socket = nullptr;
        timerid = 0;
        sendCounts = 0;
        recvCounts = 0;
    }
    ~PrivData()
    {
        closeSocket();
    }
    void closeSocket()
    {
        if (nullptr != socket)
        {
            qDebug() << cns("socket disconnectFromHost");
            socket->abort();
            socket->deleteLater();
        }
    }

public:

    int timerid;
    qint64 sendCounts;
    qint64 recvCounts;

    HappyConnection *socket;
};

ConnectionOpt::ConnectionOpt(QObject *parent)
    : QObject(parent)
{
    mData = new PrivData();
    qRegisterMetaType<NetworkData>("NetworkData");
}

ConnectionOpt::~ConnectionOpt()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void ConnectionOpt::disconnectToHost()
{
    qDebug() << cns("socket abort----------------------------");
    mData->socket->disconnectFromHost();
    mData->socket->abort();
}

void ConnectionOpt::connectToHost(QString host, quint16 port)
{
    // 主动连接
    qDebug() << QThread::currentThread()->objectName()
             << QThread::currentThread();
    mData->closeSocket();
    mData->socket = new HappyConnection();
    // 连接信号
    connect(mData->socket, SIGNAL(readyRead()), this, SLOT(readyToRead()));
    connect(mData->socket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(mData->socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(mData->socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    mData->socket->connectToHost(host, port);
    qDebug() << cns("socket connectToHost");
    mData->timerid = startTimer(1000);
}

void ConnectionOpt::setSocketDescriptor(qintptr handle)
{
    // 被动连接
    mData->closeSocket();
    mData->socket = new HappyConnection();
    mData->socket->setSocketDescriptor(handle);
    connect(mData->socket, SIGNAL(readyRead()), this, SLOT(readyToRead()));
    connect(mData->socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(mData->socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    emit connected();
    qDebug() << cns("socket setSocketDescriptor");
    mData->timerid = startTimer(1000);
}

void ConnectionOpt::sendMessage(NetworkData data)
{
    if (nullptr == mData->socket || mData->socket->state() != QAbstractSocket::ConnectedState)
    {
//        emit disconnected();
        return;
    }
    if (data.isEmpty())
    {
        return;
    }
    int ret = mData->socket->write(data.data(), data.length());
    mData->sendCounts += ret;
    while( QThread::currentThread() !=nullptr && QThread::currentThread()->isInterruptionRequested() == false )
    {
        QThread::msleep(1);
        qint64 datalen = mData->socket->bytesToWrite();
        mData->socket->flush();
        if( datalen <1024 )
        {
            break;
        }
    }
    emit nextSendingData();

//    if (ret != data.length())
//    {
//        emit sendMessageFail();
//        return;
//    }
//    qDebug() << cns("socket send sucess");
}

void ConnectionOpt::readyToRead()
{
    qint64 available = mData->socket->bytesAvailable();
    mData->recvCounts += available;
    if (available > 0)
    {
        QByteArray ba = mData->socket->readAll();
        available = ba.length();
        if (available > 0)
        {
            NetworkData data(ba.data(), available);
            emit recvMessage(data);
        }
    }
//    qDebug() << cns("socket read sucess");
}

void ConnectionOpt::connectionError(QAbstractSocket::SocketError /*socketError*/)
{
    QString error = mData->socket->errorString();
    emit disconnected();
    thread()->quit();
    qDebug() << "socket error" << error;
    killTimer(mData->timerid);
}


void ConnectionOpt::timerEvent(QTimerEvent *)
{
    // 计算发送速度和接收速度
    float ss = mData->sendCounts / 1024.0 / 1024.0;
    emit sendSpeed(ss);
    mData->sendCounts = 0;
    float rs = mData->recvCounts / 1024.0 / 1024.0;
    emit recvSpeed(rs);
    mData->recvCounts = 0;
}
