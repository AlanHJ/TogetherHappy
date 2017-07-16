#include "networkdata.h"
#include <QtCore>

class NetworkData::PrivData
{
public:
    PrivData()
    {
        data = nullptr;
        length = 0;
    }
    ~PrivData()
    {
        clear();
    }
    void clear()
    {
        if (nullptr != data)
        {
            delete[] data;
            data = nullptr;
        }
        length = 0;
    }

public:
    char *data;
    int length;
};

NetworkData::NetworkData()
{
    mData = new PrivData();
}

NetworkData::NetworkData(const NetworkData &data)
{
    mData = new PrivData();
    mData->length = ((NetworkData &)data).length();
    mData->data = new char[mData->length];
    memcpy(mData->data, ((NetworkData &)data).data(), mData->length);
}

NetworkData::NetworkData(char *data, int length)
{
    mData = new PrivData();
    if (length <= 0 || nullptr == data)
    {
        return;
    }
    mData->length = length;
    mData->data = new char[mData->length];
    memcpy(mData->data, data, length);
}

NetworkData::~NetworkData()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

char *NetworkData::data()
{
    return mData->data;
}

int NetworkData::length()
{
    return mData->length;
}

NetworkData *NetworkData::operator=(const NetworkData &data)
{
    mData->clear();
    mData->length = ((NetworkData &)data).length();
    mData->data = new char[mData->length];
    memcpy(mData->data, ((NetworkData &)data).data(), mData->length);
    return this;
}

void NetworkData::setData(char *data, int length)
{
    mData->clear();
    mData->length = length;
    mData->data = new char[mData->length];
    memcpy(mData->data, data, mData->length);
}

void NetworkData::appendData(char *data, int length)
{
    char *sz = new char[mData->length + length];
    if (mData->length > 0)
    {
        memcpy(sz, mData->data, mData->length);
    }
    memcpy(sz + mData->length, data, length);
    int len = mData->length;
    mData->clear();
    mData->data = sz;
    mData->length = len + length;
}

void NetworkData::clear()
{
    mData->clear();
}

bool NetworkData::isEmpty()
{
    bool flag = false;
    if (mData->length <= 0)
    {
        flag = true;
    }
    return flag;
}
