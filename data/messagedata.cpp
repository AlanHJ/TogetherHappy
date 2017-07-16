#include "messagedata.h"

class MessageData::PrivData
{
public:
    PrivData()
    {
        content = nullptr;
        memset(&head, 0, sizeof(MessageData::MessageHead));
        head.tcphead = TCP_HEAD;
        head.type = undefine;
        head.length = sizeof(MessageData::MessageHead) - sizeof(qint32);
    }
    ~PrivData()
    {
        if (nullptr != content)
        {
            delete content;
            content = nullptr;
        }
    }
public:
    MessageContent *content;
    MessageData::MessageHead head;
    QString uid;
};

MessageData::MessageData()
{
    mData = new PrivData();
}

MessageData::MessageData(const MessageData &data)
{
    mData = new PrivData();
    MessageData &d = (MessageData &)data;
    mData->head.type = d.datatype();
    mData->uid = d.mData->uid;
    if (nullptr != mData->content)
    {
        delete mData->content;
        mData->content = nullptr;
    }
    if (nullptr != d.content())
    {
        mData->content = d.content()->copy();
    }
}

MessageData::~MessageData()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

MessageData *MessageData::operator=(const MessageData &data)
{
    MessageData &d = (MessageData &)data;
    mData->head.type = d.datatype();
    if (nullptr != mData->content)
    {
        delete mData->content;
        mData->content = nullptr;
    }
    if (nullptr != d.content())
    {
        mData->content = d.content()->copy();
    }
    return this;
}

void MessageData::setDatatype(MessageType type)
{
    mData->head.type = type;
}

MessageType MessageData::datatype()
{
    return (MessageType)mData->head.type;
}

void MessageData::setContent(MessageContent *content)
{
    if (nullptr != mData->content)
    {
        delete mData->content;
        mData->content = nullptr;
    }
    mData->content = content;
}

MessageContent *MessageData::content()
{
    return mData->content;
}

void MessageData::toStream(NetworkData &data)
{
    mData->head.tcphead = TCP_HEAD;
    NetworkData contentdata;
    mData->content->toStream(contentdata);
    QByteArray ba = mData->uid.toLocal8Bit();
    mData->head.uidlength = ba.length();
    mData->head.length = contentdata.length() + sizeof(MessageHead)
                        + mData->head.uidlength - sizeof(qint32);
    data.appendData((char *)&mData->head, sizeof(MessageHead));
    // pack uid
    data.appendData(ba.data(), ba.length());
    data.appendData(contentdata.data(), contentdata.length());
}

void MessageData::setUid(QString uid)
{
    mData->uid = uid;
}

QString MessageData::getuid()
{
    return mData->uid;
}
