#ifndef BROADCASTOPT_H
#define BROADCASTOPT_H

#include <QObject>
#include "../data/messagedata.h"
#include "../data/networkdata.h"
#include <QHostAddress>

class QUdpSocket;
class ExistBroadcast;
class BroadcastOpt : public QObject
{
    Q_OBJECT
public:
    BroadcastOpt();
    virtual~BroadcastOpt();
private:
    class PrivData;
    PrivData *mData;
};

#endif // BROADCASTOPT_H
