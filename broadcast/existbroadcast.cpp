#include "existbroadcast.h"
#include "broadcastopt.h"
#include "../util/thmethod.h"
#include "../ui/mainui.h"
#include "../data/messagechardata.h"
#include "../config/configuredata.h"
#include <QtNetwork>

ExistBroadcast *ExistBroadcast::mBroadcast = nullptr;
ExistBroadcast::ExistBroadcast()
{
    mServerPort = HAPPY_LISTEN_PORT;
    updateAddress();
    mBroadcastSocket.bind(QHostAddress::Any, BroadcastPort,
                          QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint);
    connect(&mBroadcastSocket, SIGNAL(readyRead()),
            this, SLOT(readBroadcastDatagram()));

}

ExistBroadcast::~ExistBroadcast()
{

}

ExistBroadcast *ExistBroadcast::getInstance()
{
    if (nullptr == mBroadcast)
    {
        mBroadcast = new ExistBroadcast();
    }
    return mBroadcast;
}

void ExistBroadcast::unInstance()
{
    if (nullptr != mBroadcast)
    {
        delete mBroadcast;
        mBroadcast = nullptr;
    }
}

QHostAddress ExistBroadcast::localIp()
{
    return mLocalIp;
}

QHostAddress ExistBroadcast::localBroadcast()
{
    return mBroadcastIp;
}

quint16 ExistBroadcast::serverport()
{
    return mServerPort;
}

QString ExistBroadcast::myselfKey()
{
    QString key = mLocalIp.toString() + ":" + QString::number(mServerPort);
    return key;
}

void ExistBroadcast::setServerPort(quint16 port)
{
    mServerPort = port;
}

void ExistBroadcast::setMain(MainUI *main)
{
    mMain = main;
}

void ExistBroadcast::startBroadcasting(const QString &username)
{
    mUserName = username;
    mNewName   = mUserName;
#ifdef QT_NO_DEBUG
#endif
    mBroadcastTimer.start();
}

void ExistBroadcast::updateUsername(const QString &newname)
{
    mNewName = newname;
    sendBroadcastDatagram();
    mUserName = mNewName;
}

QString ExistBroadcast::myName()
{
    return mUserName;
}

void ExistBroadcast::sendLeftMsg()
{
    mUserName = "";
    mNewName = "";
    sendBroadcastDatagram();
}

void ExistBroadcast::sendBroadcast(NetworkData data)
{
//    emit timerStop();
//    char *buff = data.data();
//    int length = data.length();
//    QByteArray ba(buff, length);
//    QDataStream stream(ba);
//    foreach (QHostAddress address, broadcastAddresses) {
//        bool b = false;
//        for (int size = 0; size < length;)
//        {
//            char *temp = new char[8000];
//            int ret1 = stream.readRawData(temp, 8000);
//            int ret2 = mBroadcastSocket.writeDatagram(temp, ret1, address,
//                                                     BroadcastPort);
//            thread()->msleep(10);
//            if (ret1 != ret2)
//            {
//                b = false;
//                qDebug() << mBroadcastSocket.errorString();
//            }
//            else
//            {
//                size += ret2;
//                b = true;
//            }
//            delete temp;
//        }
//        if (b)
//        {
//            break;
//        }
//    }
//    emit timerStart();
}

void ExistBroadcast::verify(QString content)
{
//    QStringList lst = content.split(":");
//    if (lst.size() != 4)
//    {
//        return;
//    }
//    QString username = lst.first();
//    QString newname = lst.at(1);
//    QHostAddress address = QHostAddress(lst.at(2));
//    qint16 port = lst.last().toShort();
//    if (address == mLocalIp && mServerPort == port)
//    {
//        return;
//    }
//    QString key = address.toString() + ":" + QString::number(port);
////        qDebug() << key << username;
//    if (username == "" && newname == "")
//    {
//        emit participantLeft(key);
//        return;
//    }
//    if (!mMain->isExistUser(key))
//    {
//        emit newParticipant(username, key);
//    }
//    else
//    {
//        if (username != newname)
//        {
//            emit modifParticipant(username, newname, key);
//        }
//    }
}

void ExistBroadcast::sendBroadcastDatagram()
{
    QByteArray datagram;
    datagram.append(mUserName/* + "V" + QString::number(VERSION)*/);
    datagram.append(":");
    datagram.append(mNewName/* + "V" + QString::number(VERSION)*/);
    datagram.append(":");
    datagram.append(mLocalIp.toString());
    datagram.append(":");
    datagram.append(QString::number(mServerPort));

//    MessageData msg;
//    MessageCharData chardata;
//    chardata.setContent(datagram);
//    msg.setContent(chardata.copy());
//    msg.setDatatype(chardata.datatype());
//    NetworkData netdata;
//    msg.toStream(netdata);

//    emit signalSendBroadcast(netdata);

    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, broadcastAddresses) {
        if (mBroadcastSocket.writeDatagram(datagram, address,
                                          BroadcastPort) == -1)
            validBroadcastAddresses = false;
    }

    if (!validBroadcastAddresses)
        updateAddress();
}

void ExistBroadcast::readBroadcastDatagram()
{
    while (mBroadcastSocket.hasPendingDatagrams())
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(mBroadcastSocket.pendingDatagramSize());
        if (mBroadcastSocket.readDatagram(datagram.data(), datagram.size(),
                                          &senderIp, &senderPort) == -1)
        {
            continue;
        }
//        emit recvBroadcastData(senderIp.toString(), datagram);

        QStringList lst = QString::fromUtf8(datagram).split(":");
        if (lst.size() != 4)
        {
            continue;
        }
        QString username = lst.first();
        QString newname = lst.at(1);
        QHostAddress address = QHostAddress(lst.at(2));
        qint16 port = lst.last().toShort();
        if (address == mLocalIp && mServerPort == port)
        {
            continue;
        }
        QString key = address.toString() + ":" + QString::number(port);
//        qDebug() << key << username;
        if (username == "" && newname == "")
        {
            emit participantLeft(key);
            continue;
        }
        if (1)
        {
            emit newParticipant(username, key);
        }
        else
        {
            if (username != newname)
            {
                emit modifParticipant(username, newname, key);
            }
        }
    }
}

void ExistBroadcast::updateAddress()
{
    broadcastAddresses.clear();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
                broadcastAddresses << broadcastAddress;
            }
        }
    }

    QList<QNetworkInterface> interfacelst = QNetworkInterface::allInterfaces();
    for (QNetworkInterface face : interfacelst)
    {
        if (face.flags() == (QNetworkInterface::IsUp |
                             QNetworkInterface::IsRunning |
                             QNetworkInterface::CanBroadcast |
                             QNetworkInterface::CanMulticast))
        {
            if (!face.humanReadableName().contains("VMw", Qt::CaseInsensitive)
                    && !face.humanReadableName().contains("Vir", Qt::CaseInsensitive))
            {
                for (QNetworkAddressEntry entry : face.addressEntries())
                {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    {
                        qDebug() << face.hardwareAddress(); // MAC address
                        mBroadcastIp = entry.broadcast();
                        mLocalIp = entry.ip();
                        break;
                    }
                }
                break;
            }
        }
    }
    // 是否使用手动设置
    int enable = 0;
//    ConfigureData::getInstance()->getIni("ipset", enable);
    if (enable == 1)
    {
        QString ip;
//        ConfigureData::getInstance()->getIni("ip", ip);
        mLocalIp = ip;
    }

}

