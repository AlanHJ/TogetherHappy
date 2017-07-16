#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
class FileSend;
class FileRecv;
class ClientOpt;
class ConnectionOpt;
class ClientThread : public QThread
{
    Q_OBJECT
public:
    ClientThread();
    virtual~ClientThread();
    FileSend *createFileSend();
    FileSend *fileSendOpt();
    FileRecv *createFileRecv();
    FileRecv *fileRecvOpt();
    ClientOpt *clientopt();
    ConnectionOpt *socketopt();
signals:
    void setSocketDescriptor(qintptr handle);
private:
    class PrivData;
    PrivData *mData;
};

#endif // CLIENTTHREAD_H
