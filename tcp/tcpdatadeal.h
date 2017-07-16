#ifndef TCPDATADEAL_H
#define TCPDATADEAL_H

#include <QObject>
#include "../data/networkdata.h"
//#include "../util/thmethod.h"

class HLQueue;
class TcpDataDeal : public QObject
{
    Q_OBJECT
public:
#pragma pack(push)
#pragma pack(1)
    struct MessageHead
    {
        qint32  tcphead;
        quint32 length;
        quint16 type;
    };
#pragma pack(pop)
    TcpDataDeal(QObject *parent = 0);
    virtual~TcpDataDeal();
signals:
    void signalParser();
    void signalPosData(NetworkData data);
    void fullFrameData(NetworkData data);
public slots:
    void slotParser();
    void slotPosData(NetworkData data);
    void parserTcpdata(NetworkData data);
private:
    virtual int  ParserData(HLQueue* pQueue, NetworkData* data);
    virtual int  ParserFrameHead(HLQueue* pQueue, MessageHead* framehead);
private:
    class PrivData;
    PrivData *mData;
};

#endif // TCPDATADEAL_H
