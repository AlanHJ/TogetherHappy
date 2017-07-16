#ifndef HAPPYSERVER_H
#define HAPPYSERVER_H

#include <QTcpServer>
class ClientThread;
class HappyServer : public QTcpServer
{
    Q_OBJECT
public:
    HappyServer(QObject *parent = 0);
    virtual~HappyServer();
    // QTcpServer interface
signals:
    void newConnectionChat(ClientThread *clientthread);
protected:
    virtual void incomingConnection(qintptr handle);
};

#endif // HAPPYSERVER_H
