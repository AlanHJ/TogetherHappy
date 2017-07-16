#ifndef FILERECV_H
#define FILERECV_H

#include <QObject>
#include "../../data/messagedata.h"
class ClientOpt;
class FileRecv : public QObject
{
    Q_OBJECT
public:
    explicit FileRecv(QObject *parent = 0);
    ~FileRecv();
    void setNewPath(QString path);
    void setClientOpt(ClientOpt *opt);
signals:
    void signalRecvStart();
    void signalRecvSucess();
    void signalRecvPreInfo(QStringList list);

    void signalQuit();
    void signalStopRecv();
    void signalRecvInfo(QString type, QString txt);
    void signalRecvTotalProgress(int value);
    void signalRecvCurrentProgress(int value);
private slots:
    void slotQuit();
    void getFileConntent(MessageData msg);
public slots:
    void slotStopRecv();
private:
    class PrivData;
    PrivData *mData;
};

#endif // FILERECV_H
