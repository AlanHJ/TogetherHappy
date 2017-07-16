#include "client.h"
#include "../broadcast/existbroadcast.h"
#include "clientthread.h"
#include "clientopt.h"

Client *Client::ClientInstance = nullptr;
Client::Client()
{
    connect(&mServer, SIGNAL(newConnectionChat(ClientThread*)),
            this, SLOT(newHappyConnection(ClientThread*)));
}

Client::~Client()
{

}

HappyServer *Client::server()
{
    return &mServer;
}

Client *Client::GetInstance()
{
    if (nullptr == ClientInstance)
    {
        ClientInstance = new Client();
    }
    return ClientInstance;
}

void Client::DesInstance()
{
    if (nullptr != ClientInstance)
    {
        delete ClientInstance;
        ClientInstance = nullptr;
    }
}

void Client::readyToUse(const QString &key)
{
    ClientOpt *opt = qobject_cast<ClientOpt *>(sender());
    if (!opt)
    {
        return;
    }
    emit clientConnetion(key, opt);
    qDebug() << cns("socket ±»¶¯ ready use");
}

void Client::newHappyConnection(ClientThread *clientthread)
{
    QString key = ExistBroadcast::getInstance()->localIp().toString() +
            ":" + QString::number(mServer.serverPort());
    ClientOpt *opt = clientthread->clientopt();
    opt->setMyKey(key);
    if (nullptr == opt)
    {
        return;
    }
    connect(opt, SIGNAL(readyForUse(QString)),
            this, SLOT(readyToUse(QString)));
}

