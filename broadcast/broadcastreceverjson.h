#ifndef BROADCASTRECEVERJSON_H
#define BROADCASTRECEVERJSON_H

#include "broadcastrecever.h"
#include <QJsonObject>

class BroadcastReceverJson : public BroadcastRecever
{
    Q_OBJECT
public:
    BroadcastReceverJson(QObject *parent = 0);
    ~BroadcastReceverJson();

signals:
    void jsonMsg(QString uid, QJsonObject json);

    // BroadcastRecever interface
protected slots:
    void readyRead();
};

#endif // BROADCASTRECEVERJSON_H
