#include "logout.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include "../util/thmethod.h"

Logout *Logout::log = nullptr;
static QMutex mutex;
Logout::Logout()
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    QString logPath = GetWorkPath() + "/log";
    isOk = QDir().mkpath(logPath);
    if (!isOk)
    {
        return;
    }
    file.setFileName(logPath + "/" + date + ".log");
    isOk &= file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
}

Logout::~Logout()
{
    file.close();
}

Logout *Logout::GetInstance()
{
    QMutexLocker lock(&mutex);
    if (!log)
    {
        log = new Logout();
    }
    return log;
}

void Logout::Destruct()
{
    if (log)
    {
        delete log;
        log = nullptr;
    }
}

void Logout::write(QString type, QString msg)
{
    QMutexLocker lock(&mutex);
    if (!isOk)
    {
        return;
    }
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << "->" << type << ": " << msg << endl << endl;
}

