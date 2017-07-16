#include "filemanager.h"
#include <QStringList>

FileManager *FileManager::FManager = nullptr;
static QMutex FMutex;
FileManager::FileManager(QObject *parent) : QObject(parent)
{

}

FileManager::~FileManager()
{

}

FileManager *FileManager::GetInstance()
{
    if (!FManager)
    {
        FManager = new FileManager;
    }
    return FManager;
}

void FileManager::Destruct()
{
    if (FManager)
    {
        delete FManager;
        FManager = nullptr;
    }
}

void FileManager::insertQueue(QString uid, QStringList queue)
{
    QMutexLocker lock(&FMutex);
    sendQueue.insert(uid, queue);
}

void FileManager::removeQueue(QString uid)
{
    QMutexLocker lock(&FMutex);
    if (sendQueue.contains(uid))
    {
        sendQueue.remove(uid);
    }
}

QStringList FileManager::getQueue(QString uid)
{
    QMutexLocker lock(&FMutex);
    QStringList list;
    if (sendQueue.contains(uid))
    {
        list = sendQueue.value(uid);
    }
    return list;
}

