#include "clientopt.h"
#include "tcpdatadeal.h"
#include "messagedatadeal.h"
#include "../data/messageping.h"
#include "../data/messagepong.h"
#include "../data/messagegreeting.h"
#include "clientthread.h"
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QThread>
#include "datathread.h"
#include <QStringList>
#include <QAbstractEventDispatcher>

static const int PongTimeout = 60 * 1000;
static const int PingInterval = 5 * 1000;
class ClientOpt::PrivData
{
public:
    PrivData()
    {
        datathread = nullptr;
        thread = nullptr;
        type = undefine;
        state = ClientOpt::WaitingForGreeting;
    }
    ~PrivData()
    {
        datathread->quit();
        datathread->wait();
    }
public:
    DataThread *datathread;
    ClientThread *thread;
    QString myKey;
    QString clientKey;
    MessageType type;
    ClientOpt::ConnectionState state;
};

ClientOpt::ClientOpt(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<MessageData>("MessageData");
    mData = new PrivData();
    mData->datathread = new DataThread(this);
    connect(this, SIGNAL(sendMsgData(MessageData)), this, SLOT(sendToStreamMessage(MessageData)));
}

ClientOpt::~ClientOpt()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void ClientOpt::setMyKey(QString key)
{
    mData->myKey = key;
}

QString ClientOpt::clientKey()
{
    return mData->clientKey;
}

void ClientOpt::sendGreetingMsg()
{
    MessageData data;
    MessageGreeting *greeting = new MessageGreeting();
    greeting->setGreetingKey(mData->myKey);
    data.setDatatype(greeting->datatype());
    data.setContent(greeting);
    NetworkData netdata;
    data.toStream(netdata);
    emit sendMessage(netdata);
}

void ClientOpt::setThread(ClientThread *thread)
{
    mData->thread = thread;
    connect(thread, SIGNAL(finished()), mData->datathread, SLOT(quit()));
}

ClientThread *ClientOpt::connectThread()
{
    return mData->thread;
}

void ClientOpt::sendDisconnect()
{
    qDebug() << thread()->isRunning() << thread()->isFinished();
    emit disconnectToHost();
}

void ClientOpt::recvMessage(NetworkData data)
{
    // 收到数据进行解析
    if (mData->state == WaitingForGreeting)
    {
        emit parserMessage(data);
        mData->state = ReadingGreeting;
    }

    if (mData->state == ReadingGreeting)
    {
        QTime time;
        time.start();
        while (time.elapsed() < 5000)
        {
            if (mData->state == ReadyForUse)
            {
                break;
            }
            QThread::currentThread()->eventDispatcher()->processEvents (QEventLoop::AllEvents);
        }
        if (mData->state != ReadyForUse)
        {
            // 断开连接
            qDebug() << cns("验证失败");
        }
        return;
    }

    if (mData->state != ReadyForUse)
    {
        return;
    }

    emit parserMessage(data);
}

void ClientOpt::sendToStreamMessage(MessageData data)
{
    NetworkData netdata;
    data.toStream(netdata);
    emit sendMessage(netdata);
}

void ClientOpt::connected()
{
    qDebug() << "socket connected";
    // 连接时发送验证消息
    sendGreetingMsg();
}

void ClientOpt::disconnected()
{
    // 连接断开口进行重新连接
    emit Clientdisconnected();
    thread()->quit();
}

void ClientOpt::sendMessageFail()
{
    emit sendFail();
}

void ClientOpt::pingsend()
{
    MessageData msg;
    MessagePing *ping = new MessagePing();
    msg.setDatatype(ping->datatype());
    msg.setContent(ping);
    NetworkData data;
    msg.toStream(data);
    emit sendMessage(data);
    qDebug() << cns("send ping");
}

void ClientOpt::recvMessageData(MessageData data)
{
    mData->type = data.datatype();
    switch (mData->type) {
    case Ping:
    {
        qDebug() << cns("recv ping");
        MessageData msg;
        MessagePong *pong = new MessagePong();
        msg.setDatatype(pong->datatype());
        msg.setContent(pong);
        NetworkData data;
        msg.toStream(data);
        emit sendMessage(data);
        qDebug() << cns("send pong");
    }
        break;
    case Pong:
        qDebug() << cns("recv pong");
        break;
    case Greeting:
    {
        mData->state = ReadyForUse;
        MessageGreeting *greeting = dynamic_cast<MessageGreeting *>(data.content());
        if (nullptr != greeting)
        {
            mData->clientKey = greeting->greetingKey();
            emit readyForUse(mData->clientKey);
        }
    }
        break;
    case RichText:
        emit sendUIMessage(data);
        break;
    case FileSendType:
        emit sendFileMessage(data);
        break;
    case FileHead:
        emit sendFileMessage(data);
        break;
    case undefine:
        break;
    default:
        break;
    }
}
