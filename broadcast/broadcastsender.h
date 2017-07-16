#ifndef BROADCASTSENDER_H
#define BROADCASTSENDER_H

#include <QObject>
#include <QHostAddress>
#include "../data/messagedata.h"
#include "../data/networkdata.h"

class QUdpSocket;
class BroadcastSender : public QObject
{
    Q_OBJECT
public:
    static BroadcastSender *Sender();
    static void Destruct();
    void start();
private:
    explicit BroadcastSender(QObject *parent = 0);
    ~BroadcastSender();

    static BroadcastSender *sender;
    QThread *thread;
    QUdpSocket *udp;

signals:
    void sendFail(QString uid);
    void createSender();
    void signalMsgData(MessageData msg, QHostAddress addr,
                  quint16 port, QString uid);
    void signalSendData(NetworkData data, QHostAddress addr,
                  quint16 port, QString uid);
private slots:
    void slotSendData(NetworkData data, QHostAddress addr,
                  quint16 port, QString uid);
    void slotMsgData(MessageData msg, QHostAddress addr,
                  quint16 port, QString uid);
};

#endif // BROADCASTSENDER_H
