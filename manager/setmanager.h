#ifndef SETMANAGER_H
#define SETMANAGER_H

#include <QThread>

class SetManager : public QThread
{
    Q_OBJECT
public:
    SetManager();
    virtual~SetManager();
signals:
    void signalResouceError(QString error);
    // QThread interface
protected:
    void run();
};

#endif // SETMANAGER_H
