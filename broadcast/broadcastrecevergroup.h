#ifndef BROADCASTRECEVERGROUP_H
#define BROADCASTRECEVERGROUP_H

#include "broadcastrecever.h"

class BroadcastReceverGroup : public BroadcastRecever
{
    Q_OBJECT
public:
    BroadcastReceverGroup(QObject *parent = 0);
    ~BroadcastReceverGroup();
    QThread *getThread();
signals:
    void recvData(QString key, NetworkData data);

    // BroadcastRecever interface
protected slots:
    void readyRead();
};

#endif // BROADCASTRECEVERGROUP_H
