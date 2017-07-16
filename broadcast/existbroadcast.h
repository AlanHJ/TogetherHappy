#ifndef EXISTBROADCAST_H
#define EXISTBROADCAST_H

#include <QTimer>
#include <QObject>
#include <QUdpSocket>
#include "../data/networkdata.h"

static const unsigned BroadcastPort = 54321;
class MainUI;
class BroadcastOpt;
class ExistBroadcast : public QObject
{
    Q_OBJECT
public:
    ExistBroadcast();
    virtual~ExistBroadcast();
    static ExistBroadcast *getInstance();
    static void unInstance();
    QHostAddress localIp();
    QHostAddress localBroadcast();
    quint16 serverport();
    QString myselfKey();
    void setServerPort(quint16 port);
    void setMain(MainUI *main);
    void startBroadcasting(const QString &username);
    void updateUsername(const QString &newname);
    QString myName();
    void sendLeftMsg();
public slots:
    void sendBroadcast(NetworkData data);
    void verify(QString content);
private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();
signals:
    void startBroadcast();
    void timerStop();
    void timerStart();
    void signalSendBroadcast(NetworkData data);
    void recvBroadcastData(QString ip, QByteArray ba);
    void participantLeft(const QString &key);
    void newParticipant(const QString &nick, const QString &key);
    void modifParticipant(const QString &oldnick, const QString &newnick, const QString &key);
private:
    void updateAddress();
private:
    static ExistBroadcast *mBroadcast;
    MainUI *mMain;

    QUdpSocket mBroadcastSocket;
    QTimer mBroadcastTimer;

    QString mUserName;
    QString mNewName;
    QHostAddress mLocalIp;
    QList<QHostAddress> broadcastAddresses;
    QHostAddress mBroadcastIp;
    quint16 mServerPort;
};

#endif // EXISTBROADCAST_H
