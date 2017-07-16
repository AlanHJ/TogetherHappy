#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
class HappyConnection : public QTcpSocket
{
    Q_OBJECT
public:
    HappyConnection(QObject *parent = 0);
    virtual~HappyConnection();
};

#endif // CONNECTION_H
