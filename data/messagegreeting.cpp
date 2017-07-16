#include "messagegreeting.h"
#include <QDebug>

class MessageGreeting::PrivData
{
public:
    PrivData()
    {
        keylength = 0;
    }
    ~PrivData()
    {

    }
public:
    int keylength;
    QString key;
};

MessageGreeting::MessageGreeting()
{
    mData = new PrivData();
}

MessageGreeting::~MessageGreeting()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void MessageGreeting::setGreetingKey(QString key)
{
    mData->key = key;
}

QString MessageGreeting::greetingKey()
{
    return mData->key;
}

MessageType MessageGreeting::datatype()
{
    return Greeting;
}

MessageContent *MessageGreeting::copy()
{
    MessageGreeting *msg = new MessageGreeting();
    msg->setGreetingKey(mData->key);
    return msg;
}

void MessageGreeting::toStream(NetworkData &data)
{
    mData->keylength = mData->key.toLocal8Bit().size();
    data.appendData((char *)&mData->keylength, sizeof(int));
    data.appendData(mData->key.toLocal8Bit().data(), mData->keylength);
}

void MessageGreeting::parserStream(NetworkData &data)
{
    char *buff = data.data();
    int length = data.length();
    memcpy(&mData->keylength, buff, sizeof(int));
    int size = mData->keylength + sizeof(int);
    if (length != size)
    {
        qDebug() << "parser greeting key lenght error";
        return;
    }
    char *keybuff = new char[mData->keylength];
    memcpy(keybuff, buff + sizeof(int), mData->keylength);
    mData->key = QString::fromLocal8Bit(keybuff, mData->keylength);
    delete[] keybuff;
    keybuff = nullptr;
}
