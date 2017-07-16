#include "tcpdatadeal.h"
#include "../data/messagedata.h"
#include "hlqueue.h"
#include <QTime>
#include <QDebug>
#include <QThread>

class TcpDataDeal::PrivData
{
public:
    PrivData()
    {
//        data = nullptr;
//        offset = 0;
//        datalength = 0;

        queue = new HLQueue(MAX_QUEUE_LENGTH);
        m_Time.start();
    }
    ~PrivData()
    {
//        if (nullptr != data)
//        {
//            delete[] data;
//            data = nullptr;
//        }
        delete queue;
        queue = nullptr;
    }
public:
//    int offset;
//    int datalength;
//    char *data;

    HLQueue*    queue;
    QTime       m_Time;
};

TcpDataDeal::TcpDataDeal(QObject *parent)
    : QObject(parent)
{
    mData = new PrivData();
    connect(this, SIGNAL(signalParser()), this, SLOT(slotParser()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(signalPosData(NetworkData)), this, SLOT(slotPosData(NetworkData)),
            Qt::QueuedConnection);
}

TcpDataDeal::~TcpDataDeal()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void TcpDataDeal::slotParser()
{
    NetworkData result;
    int parserRlt = ParserData(mData->queue,&result);
    if( parserRlt == 0)
    {
        emit fullFrameData(result);
    }
    if (parserRlt >= 0)
    {
        slotParser();
    }
}

void TcpDataDeal::slotPosData(NetworkData data)
{
    char *buff = data.data();
    int len = data.length();

    int iRlt = -1;
    while( iRlt == -1 )
    {
        iRlt = mData->queue->WriteQueue(buff,len);
        int parserRlt = 0;
        while(parserRlt >= 0)
        {
            NetworkData result;
            parserRlt = ParserData(mData->queue, &result);
            if(parserRlt == 0)
            {
                emit fullFrameData(result);
            }
        }
    }

//    if (mData->queue->WriteQueue(buff, len) < 0)
//    {
//        emit signalParser();
//        emit signalPosData(data);
//    }
//    else
//    {
//        emit signalParser();
//    }
}

void TcpDataDeal::parserTcpdata(NetworkData data)
{
    emit signalPosData(data);
}

int TcpDataDeal::ParserData(HLQueue *pQueue, NetworkData *data)
{
    MessageHead framehead;
    int ret = ParserFrameHead(pQueue,&framehead);
    if( ret < 0 )
    {
        return -1;
    }
    else if( ret > 0 )
    {
        return 1;
    }
    int chlen = framehead.length - sizeof(quint32) - sizeof(quint16);
    char* ch = new char[chlen];
    if(pQueue->ReadQueue(ch, chlen)== -1)
    {
        delete[] ch;
        return 1;
    }
    data->appendData((char *)&framehead, sizeof(MessageHead));
    data->appendData(ch, chlen);
    delete[] ch;
    return 0;
}

int TcpDataDeal::ParserFrameHead(HLQueue *pQueue, TcpDataDeal::MessageHead *framehead)
{
    if( pQueue == NULL )
    {
        return -100;
    }
    if( pQueue->GetQueueAvailLength() <= sizeof(MessageHead))
    {
        return -1;
    }
    memset(framehead,0,sizeof(MessageHead));
    if( pQueue->ReadQueue((char*)framehead, sizeof(MessageHead))== -1 )
    {
        return -1;
    }
    if(framehead->tcphead != FRAME_START_TAG /*|| framehead->length >= FRAME_MAX_LENGTH*/)
    {
        int i = sizeof(MessageHead);
        pQueue->MoveQueueReadFlag(1 - i); //反向移动，丢弃一个字节
        return 1;
    }
    if( framehead->length > pQueue->GetQueueAvailLength() + sizeof(MessageHead) - sizeof(U32))
    {
        int i = sizeof(MessageHead);
        pQueue->MoveQueueReadFlag(0 - i); //数据长度不够，保留该头
        return -1;

    }
    return 0;
}
