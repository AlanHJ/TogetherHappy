#include "broadcastsender.h"
#include <QThread>
#include <QUdpSocket>
#include <QTime>

BroadcastSender *BroadcastSender::sender = nullptr;
BroadcastSender *BroadcastSender::Sender()
{
    if (!sender)
        sender = new BroadcastSender();
    return sender;
}

void BroadcastSender::Destruct()
{
    if (sender)
    {
        sender->thread->quit();
    }
}

void BroadcastSender::start()
{
    thread->start();
    emit createSender();
}

BroadcastSender::BroadcastSender(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    thread = new QThread();
    udp = nullptr;
    thread->setObjectName("BroadcastSender Thread");
    this->moveToThread(thread);
    connect(this, &BroadcastSender::signalSendData,
            this, &BroadcastSender::slotSendData);

    connect(this, &BroadcastSender::signalMsgData,
            this, &BroadcastSender::slotMsgData);

    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(this, &BroadcastSender::createSender, this, [=] () {
        udp = new QUdpSocket(this);
        udp->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);
    });
}

BroadcastSender::~BroadcastSender()
{

}

void BroadcastSender::slotSendData(NetworkData data,
                                   QHostAddress addr,
                                   quint16 port,
                                   QString uid)
{
    if (!udp)
        return;
    char *d = data.data();
    int len = data.length();
    // 每次最多发送UDP_SEND_DATA_SIZE长度
    int pos = 0;
    int error = 0;
    while (pos < len)
    {
        int rlt = udp->writeDatagram(d + pos, len - pos > UDP_SEND_DATA_SIZE ? UDP_SEND_DATA_SIZE : len - pos, addr, port);

        if (rlt == -1)
        {
             error++;

        }
        else
        {
            pos += rlt;
        }
    }
}

void BroadcastSender::slotMsgData(MessageData msg,
                                  QHostAddress addr,
                                  quint16 port,
                                  QString uid)
{
    NetworkData data;
    msg.toStream(data);
    slotSendData(data, addr, port, uid);
}

