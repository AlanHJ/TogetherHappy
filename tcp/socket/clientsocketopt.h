#ifndef CLIENTSOCKETOPT_H
#define CLIENTSOCKETOPT_H

#include <QObject>

class QTcpSocket;
class ClientSocketOpt : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocketOpt(QObject *parent = 0);
    ~ClientSocketOpt();

    QString getSavePath() const;
    void setSavePath(const QString &value);

public slots:
    void recvRequested();

signals:
    void connectHost(QString ip, quint16 port);
    void senddata(QByteArray ba);

    void recvProgress(int value, qreal svalue);
    void recvSpeed(float speed);
    void recvInformation(QString info);
    void disconnected();
    void connected();

    void socketRequest();
private:
    QTcpSocket *socket;
    int timerid;
    qint64 recvCounts;
    QString savePath;
    QThread *socketThread;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);
};

#endif // CLIENTSOCKETOPT_H
