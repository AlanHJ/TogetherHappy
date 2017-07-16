#ifndef CLEARTEMPOPT_H
#define CLEARTEMPOPT_H

#include <QThread>

class ClearTempOpt : public QThread
{
    Q_OBJECT
public:
    ClearTempOpt();
    virtual~ClearTempOpt();
signals:
    void signalFinished();
    void signalProgress(int value);
    void signalProgressinfo(QString info);
    // QThread interface
protected:
    virtual void run();
};

#endif // CLEARTEMPOPT_H
