#ifndef GSOAPTHREAD_H
#define GSOAPTHREAD_H

#include <QThread>

class GsoapThread : public QThread
{
    Q_OBJECT
public:
    GsoapThread();
    virtual~GsoapThread();
    void stopGoap();
    // QThread interface
protected:
    void run();
signals:
    void gsoapServerError();
private:
    class PrivData;
    PrivData *mData;
};

#endif // GSOAPTHREAD_H
