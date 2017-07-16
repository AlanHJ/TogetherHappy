#ifndef CLIENTOPT_H
#define CLIENTOPT_H

#include <QObject>
#include "../data/networkdata.h"
#include "../data/messagedata.h"

class ClientThread;
class ClientOpt : public QObject
{
    Q_OBJECT
public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    explicit ClientOpt(QObject *parent = 0);
    virtual~ClientOpt();
    void setMyKey(QString key);
    QString clientKey();
    void sendGreetingMsg();
    void setThread(ClientThread *thread);
    ClientThread *connectThread();
signals:
    void sendSpeed(float mbs);
    void recvSpeed(float mbs);

    void sendFail();
    void nextSendingData();
    void disconnectToHost();
    void Clientdisconnected();
    void pingpongtimeout();
    void readyForUse(const QString &key);
    void sendMessage(NetworkData data);
    void sendMsgData(MessageData data);
    void parserMessage(NetworkData data);
    void sendUIMessage(MessageData data);
    void sendFileMessage(MessageData data);
    void connectToHost(QString host, quint16 port);
public slots:
    void sendDisconnect();
    void recvMessage(NetworkData data);
    void sendToStreamMessage(MessageData data);
    void recvMessageData(MessageData data);
    void connected();
    void disconnected();
    void sendMessageFail();
private slots:
    void pingsend();
private:
    class PrivData;
    PrivData *mData;
};

#endif // CLIENTOPT_H
