#include "broadcastreceverjson.h"
#include <QUdpSocket>
#include <QJsonDocument>

BroadcastReceverJson::BroadcastReceverJson(QObject *parent)
    : BroadcastRecever(parent)
{

}

BroadcastReceverJson::~BroadcastReceverJson()
{

}

void BroadcastReceverJson::readyRead()
{
    QMutexLocker lock(&mutex);
    if (!udp)
        return;
    while (udp->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress addr;
        quint16 port;
        datagram.resize(udp->pendingDatagramSize());
        int rlt = udp->readDatagram(datagram.data(),
                                    datagram.size(),
                                    &addr, &port);
        if (rlt != -1)
        {
            // Ω‚ŒˆJson
            QByteArray base64 = QByteArray::fromBase64(datagram);
            QJsonParseError jsonerror;
            QJsonDocument doc = QJsonDocument::fromJson(base64, &jsonerror);
            if (jsonerror.error == QJsonParseError::NoError)
            {
                if (doc.isObject())
                {
                    QJsonObject obj = doc.object();
                    if (obj.contains("uid"))
                    {
                        auto it = obj.find("uid");
                        QString uid = it.value().toString();
                        emit jsonMsg(uid, obj);
                    }
                }
            }
            continue;
        }
    }
}
