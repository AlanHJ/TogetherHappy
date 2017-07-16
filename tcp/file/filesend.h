#ifndef FILESEND_H
#define FILESEND_H

#include <QObject>
#include <QStringList>

#define MAX_SEND_SIZE 1024 * 1024
class ClientOpt;
class FileSend : public QObject
{
    Q_OBJECT
public:
    explicit FileSend(QObject *parent = 0);
    ~FileSend();
    void setConnectInfo(QString ip, qint16 port);
    void setClientOpt(ClientOpt *opt);
signals:
    void signalSendStart();
    void signalSendSucess();

    void signalQuit();
    void signalStartSend(QStringList fileLst, QStringList pathLst);
    void signalSendInfo(QString type, QString txt);
    void signalSendTotalProgress(int value);
    void signalSendCurrentProgress(int value);
private slots:
    void slotQuit();
    void nextData();
    void readyForSend(QString key);
    void slotStartSend(QStringList fileLst, QStringList pathLst);

    void getAllFileLst(QString path, QString cdPath, QStringList *list);
public slots:
    void stopSend();
private:
    class PrivData;
    PrivData *mData;
};

#endif // FILESEND_H
