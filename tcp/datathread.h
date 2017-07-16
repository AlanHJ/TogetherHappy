#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
class ClientOpt;
class DataThread : public QThread
{
public:
    DataThread(ClientOpt *opt);
    virtual~DataThread();
private:
    class PrivData;
    PrivData *mData;
};

#endif // DATATHREAD_H
