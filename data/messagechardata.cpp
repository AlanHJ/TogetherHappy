#include "messagechardata.h"

class MessageCharData::PrivData
{
public:
    PrivData()
    {
        length = 0;
        type = 0;
    }
    ~PrivData()
    {

    }
public:
    QByteArray content;
    int length;
    int type;
};

MessageCharData::MessageCharData()
{
    mData = new PrivData();
}

MessageCharData::~MessageCharData()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void MessageCharData::setContent(QByteArray content)
{
    mData->content = content;
    mData->length = content.length();
}

QByteArray MessageCharData::content()
{
    return mData->content;
}

void MessageCharData::setType(int type)
{
    mData->type = type;
}

int MessageCharData::type()
{
    return mData->type;
}

MessageType MessageCharData::datatype()
{
    return commonStr;
}

MessageContent *MessageCharData::copy()
{
    MessageCharData *chardata = new MessageCharData();
    chardata->setContent(mData->content);
    chardata->setType(mData->type);
    return chardata;
}

void MessageCharData::toStream(NetworkData &data)
{
    data.appendData((char *)&mData->type, sizeof(int));
    data.appendData((char *)&mData->length, sizeof(int));
    data.appendData(mData->content.data(), mData->length);
}

void MessageCharData::parserStream(NetworkData &data)
{
    char *buff = data.data();
    int length = data.length();
    int type = 0;
    int len = 0;
    memcpy(&type, buff, sizeof(int));
    memcpy(&len, buff + sizeof(int), sizeof(int));
    if (len < 0)
    {
        return;
    }
    if (len + sizeof(int) * 2 != length)
    {
        return;
    }
    char *temp = new char[len];

    memcpy(temp, buff + sizeof(int) * 2, len);
    mData->type = type;
    mData->length = len;
    mData->content = QByteArray(temp, len);
}
