#ifndef CONNECTIONOPT_H
#define CONNECTIONOPT_H

#include "../data/networkdata.h"
#include <QObject>
#include <QAbstractSocket>

class ConnectionOpt : public QObject
{
    Q_OBJECT
public:
    ConnectionOpt(QObject *parent = 0);
    virtual~ConnectionOpt();
public slots:
    void disconnectToHost();
    void connectToHost(QString host, quint16 port);
    void setSocketDescriptor(qintptr handle);
    void sendMessage(NetworkData data);
private slots:
    void readyToRead();
    void connectionError(QAbstractSocket::SocketError socketError);
signals:
    void sendSpeed(float mbs);
    void recvSpeed(float mbs);

    void nextSendingData();
    void connected();
    void disconnected();
    void sendMessageFail();
    void recvMessage(NetworkData data);
private:
    class PrivData;
    PrivData *mData;

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // CONNECTIONOPT_H
