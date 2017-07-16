#ifndef BROADCASTRECEVER_H
#define BROADCASTRECEVER_H

#include <QObject>
#include <QHostAddress>
#include "../data/messagedata.h"
#include "../data/networkdata.h"

class QUdpSocket;
class BroadcastRecever : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastRecever(QObject *parent = 0);
    virtual~BroadcastRecever();
    void start();
    void stop();
signals:
    void joinMulticastGroupFail();
    void createRecever(QHostAddress addr,
                       quint16 port);
private slots:
    virtual void readyRead() = 0;
    void initRecever(QHostAddress addr,
                     quint16 port);
protected:
    QMutex mutex;
    QUdpSocket *udp;
    QThread *thread;

    QHostAddress addr;
};

#endif // BROADCASTRECEVER_H
