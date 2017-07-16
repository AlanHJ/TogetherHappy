#include "broadcastrecevergroup.h"
#include <QUdpSocket>

BroadcastReceverGroup::BroadcastReceverGroup(QObject *parent)
    : BroadcastRecever(parent)
{

}

BroadcastReceverGroup::~BroadcastReceverGroup()
{

}

QThread *BroadcastReceverGroup::getThread()
{
    return thread;
}

void BroadcastReceverGroup::readyRead()
{
    QMutexLocker lock(&mutex);
    if (!udp)
        return;
    while (udp->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        quint16 port;
        int pendsize = udp->pendingDatagramSize();
        datagram.resize(pendsize);
        int  rlt = udp->readDatagram(datagram.data(),
                                    datagram.size(),
                                    &addr, &port);
        if (rlt != -1)
        {
            QString key = addr.toString() + ":" + QString::number(port);
//            qDebug() << "current udp addr:" << this->addr.toString() << " receve key:" << key;
            emit recvData(key,
                          NetworkData(datagram.data(), datagram.size()));
            continue;
        }
    }
}
