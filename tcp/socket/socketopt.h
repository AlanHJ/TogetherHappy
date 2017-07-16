#ifndef CONNECTIONOPT_H
#define CONNECTIONOPT_H

#include <QObject>

class QTcpSocket;
class SocketOpt : public QObject
{
    Q_OBJECT
public:

    struct FileStruct
    {
        QString root;
        QString path;
        QString name;
    };

    explicit SocketOpt(QObject *parent = 0);
    ~SocketOpt();

signals:
    void sendProgress(int value, qreal svalue);
    void sendSpeed(float speed);
    void sendInformation(QString info);
    void disconnected();
    void incomingConnect(qintptr handle);

public slots:
    void requested();

private:
    QTcpSocket *socket;
    QThread *socketThread;
};

#endif // CONNECTIONOPT_H
