#ifndef MUILTDATAOPT_H
#define MUILTDATAOPT_H

#include <QObject>
#include "../data/networkdata.h"
#include "../data/db/groupdata.h"
#include <QHash>

class TcpDataDeal;
class MessageDataDeal;
class MuiltDataOpt : public QObject
{
    Q_OBJECT
public:
    explicit MuiltDataOpt(QThread *thread, QObject *parent = 0);
    ~MuiltDataOpt();

signals:
    void recvMsg(QString uid, QString html);
    void updateGroupData(QString uid, GroupData gd);

public slots:
    void recvMuiltData(QString key, NetworkData data);
    void resetQueue();
private:
    QHash<QString, TcpDataDeal*> tcps;
    MessageDataDeal *msg;
};

#endif // MUILTDATAOPT_H
