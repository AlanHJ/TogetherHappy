#include "gsoapthread.h"
#include "../soapService.h"
#include "../../util/thmethod.h"

class GsoapThread::PrivData
{
public:
    PrivData()
    {
        service = new Service(SOAP_C_MBSTRING);
        service->accept_timeout = 5;
        service->connect_timeout = 5;
        service->recv_timeout = 7;
        service->send_timeout = 7;
    }
    ~PrivData()
    {
        if (nullptr != service)
        {
            delete service;
            service = nullptr;
        }
    }
public:
    bool isRunning;
    Service *service;
};

GsoapThread::GsoapThread()
{
    setObjectName("Gsoap Service Thread");
    mData = new PrivData();
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

GsoapThread::~GsoapThread()
{
    wait();
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void GsoapThread::stopGoap()
{
    mData->isRunning = false;
    mData->service->soap_close_socket();
}

void GsoapThread::run()
{
    int port = HAPPY_GOASP_PORT;
    while (mData->isRunning = true && mData->service->run(port))
    {
//        msleep(5000);
        mData->service->soap_stream_fault(std::cerr);
//        emit gsoapServerError();
    }
    quit();
}
