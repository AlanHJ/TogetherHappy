#include "broadcastrecever.h"
#include <QThread>
#include <QDateTime>
#include <QUdpSocket>
#include "../util/thmethod.h"

BroadcastRecever::BroadcastRecever(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<NetworkData>("NetworkData");
    qRegisterMetaType<MessageData>("MessageData");
    thread = new QThread();
    udp = nullptr;
    thread->setObjectName("BroadcastRecever Thread" +
                          QTime::currentTime().toString("hh:mm:ss"));
    this->moveToThread(thread);
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(this, &BroadcastRecever::createRecever,
            this, [=] (QHostAddress addr, quint16 port) {
        initRecever(addr, port);
    });
}

BroadcastRecever::~BroadcastRecever()
{

}

void BroadcastRecever::start()
{
    thread->start();
}

void BroadcastRecever::stop()
{
    thread->quit();
}

void BroadcastRecever::initRecever(QHostAddress addr, quint16 port)
{
    QMutexLocker lock(&mutex);
    if (udp)
    {
        udp->close();
        udp->abort();
        delete udp;
        udp = nullptr;
    }
    udp = new QUdpSocket(this);
    udp->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);
    udp->bind(QHostAddress::AnyIPv4, port,
             QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint);
    bool b = udp->joinMulticastGroup(addr);
    connect(udp, &QUdpSocket::readyRead, this, [=] () {
        readyRead();
    });
    if (!b)
    {
        emit joinMulticastGroupFail();
    }
    this->addr = addr;
}
