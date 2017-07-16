#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "happyserver.h"

class ClientThread;
class ClientOpt;
class Client :public QObject
{
    Q_OBJECT
public:
    Client();
    virtual~Client();
    HappyServer *server();
    static Client *GetInstance();
    static void DesInstance();
signals:
    void clientConnetion(const QString &key, ClientOpt *client);
private slots:
    void readyToUse(const QString &key);
    void newHappyConnection(ClientThread *clientthread);
private:
    HappyServer mServer;
    static Client *ClientInstance;
};

#endif // CLIENT_H
