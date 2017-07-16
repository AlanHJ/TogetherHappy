#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class TcpServer : public QTcpServer
{
public:
    TcpServer(QObject *parent = 0);
    ~TcpServer();

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);
};

#endif // TCPSERVER_H
