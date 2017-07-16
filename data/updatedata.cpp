#include "updatedata.h"
#include <QDebug>
#include <QCryptographicHash>

UpdateData::UpdateData()
{

}

UpdateData::~UpdateData()
{

}



MessageType UpdateData::datatype()
{
    return update;
}

MessageContent *UpdateData::copy()
{
    UpdateData *update = new UpdateData;
    return update;
}

void UpdateData::toStream(NetworkData &data)
{
    qint32 end = TCP_END;
    UpdateHead head;
    memset(&head, 0, sizeof(UpdateHead));
    QFile file(qApp->applicationFilePath());
    if (!file.open(QIODevice::ReadOnly))
    {
        head.md5 = 0;
        head.size = sizeof(UpdateHead) + sizeof(qint32);
        char *buff = new char[head.size];
        memcpy(buff, &head, sizeof(UpdateHead));
        memcpy(buff + sizeof(UpdateHead), &end, sizeof(qint32));
        data.appendData(buff, head.size);
        delete[] buff;
        buff = nullptr;
        return;
    }
    QByteArray fileba = file.readAll();
    file.close();
    QByteArray md5 = QCryptographicHash::hash(fileba, QCryptographicHash::Md5);
    head.md5 = md5.length();
    head.file = fileba.length();
    head.size = sizeof(UpdateHead) + sizeof(qint32)
             + head.md5 + head.file;
    int pos = 0;
    char *buff = new char[head.size];
    memcpy(buff, &head, sizeof(UpdateHead));
    pos += sizeof(UpdateHead);
    memcpy(buff + pos, md5.data(), md5.length());
    pos += md5.length();
    memcpy(buff + pos, fileba.data(), fileba.length());
    pos += fileba.length();
    memcpy(buff + pos, &end, sizeof(qint32));
    pos += sizeof(qint32);
    if (pos == head.size)
    {
        data.appendData(buff, head.size);
    }
    else
    {
        qDebug() << "update data package length error!!!";
    }
    delete[] buff;
    buff = nullptr;
}

void UpdateData::parserStream(NetworkData &data)
{
    char *buff = data.data();
    int length = data.length();
    int pos = 0;
    qint32 end = 0;
    UpdateHead head;
    memcpy(&head, buff + pos, sizeof(UpdateHead));
    pos += sizeof(UpdateHead);
    if (head.size != length)
    {
        qDebug() << "Update data parser length error!!!";
        return;
    }
    memcpy(&end, buff + pos + head.md5 + head.file, sizeof(qint32));
    if (end != TCP_END)
    {
        qDebug() << "Update data parser end error!!!";
        return;
    }
    char *temp = new char[head.md5];
    memcpy(temp, buff + pos, head.md5);
    pos += head.md5;
    QByteArray md5(temp, head.md5);
    delete[] temp;
    temp = nullptr;
    temp = new char[head.file];
    memcpy(temp, buff + pos, head.file);
    pos += head.file;
    QByteArray fileba(temp, head.file);
    delete[] temp;
    temp = nullptr;
    QByteArray filemd5 = QCryptographicHash::hash(fileba, QCryptographicHash::Md5);
    if (md5 != filemd5)
    {
        return;
    }
    QString name = GetWorkPath() + "/update/TogetherHappy.exe";
    if (QFile::exists(name))
    {
        if (!QFile::remove(name))
        {
            return;
        }
    }
    QFile file(name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(fileba);
    }
    file.close();
}
