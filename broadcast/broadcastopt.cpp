#include "broadcastopt.h"
#include "existbroadcast.h"
#include "../tcp/tcpdatadeal.h"
#include "../data/messagerichtext.h"
#include "../data/messagechardata.h"
#include <QUdpSocket>
#include <QThread>
#include <QMutex>

class BroadcastOpt::PrivData
{
public:
    PrivData()
    {

    }
    ~PrivData()
    {

    }
public:
};

BroadcastOpt::BroadcastOpt()
{
    qRegisterMetaType<NetworkData>("NetworkData");
    qRegisterMetaType<MessageData>("MessageData");
    mData = new PrivData();
}

BroadcastOpt::~BroadcastOpt()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}
