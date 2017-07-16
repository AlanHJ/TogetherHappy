#include "clientopt.h"
#include "datathread.h"
#include "tcpdatadeal.h"
#include "messagedatadeal.h"

class DataThread::PrivData
{
public:
    PrivData()
    {
        opt = nullptr;
        tcpdeal = nullptr;
        msgdeal = nullptr;
    }
    ~PrivData()
    {

    }
public:
    ClientOpt *opt;
    TcpDataDeal *tcpdeal;
    MessageDataDeal *msgdeal;
};

DataThread::DataThread(ClientOpt *opt)
{
    mData = new PrivData();
    mData->opt = opt;
    mData->tcpdeal = new TcpDataDeal();
    mData->msgdeal = new MessageDataDeal();
    mData->tcpdeal->moveToThread(this);
    mData->msgdeal->moveToThread(this);

    connect(mData->tcpdeal, &TcpDataDeal::fullFrameData,
            mData->msgdeal, &MessageDataDeal::ParserFrameData);

    connect(mData->msgdeal, &MessageDataDeal::recvMessageData,
            mData->opt, &ClientOpt::recvMessageData);

    connect(mData->opt, &ClientOpt::parserMessage,
            mData->tcpdeal, &TcpDataDeal::parserTcpdata);

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(finished()), mData->tcpdeal, SLOT(deleteLater()));
    connect(this, SIGNAL(finished()), mData->msgdeal, SLOT(deleteLater()));

    start();
}

DataThread::~DataThread()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}
