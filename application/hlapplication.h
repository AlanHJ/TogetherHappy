#ifndef HLAPPLICATION_H
#define HLAPPLICATION_H

#include <QApplication>
#include <QObject>
#include <QSharedMemory>
#include <QLocalServer>

class HLApplication : public QApplication
{
    Q_OBJECT
public:
    HLApplication(int &argc, char *argv[], const QString uniqueKey);
    ~HLApplication();
    bool isRunning();
    bool sendMessage(const QString &message);
public slots:
    void receiveMessage();
signals:
    void messageAvailable(QString message);
private:
    bool _isRunning;
    QString _uniqueKey;
    QSharedMemory sharedMemory;
    QLocalServer *localServer;
    static const int timeout = 1000;
};

#endif // HLAPPLICATION_H
