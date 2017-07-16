#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QHash>
#include <QObject>

class SocketOpt;
class FileManager : public QObject
{
    Q_OBJECT
private:
    explicit FileManager(QObject *parent = 0);
    ~FileManager();
    static FileManager *FManager;
    QHash<QString, QStringList> sendQueue;

signals:
    void sendRequested(QString uid, SocketOpt *opt);

public:
    static FileManager *GetInstance();
    static void Destruct();

    void insertQueue(QString uid, QStringList queue);
    void removeQueue(QString uid);
    QStringList getQueue(QString uid);
};

#endif // FILEMANAGER_H
