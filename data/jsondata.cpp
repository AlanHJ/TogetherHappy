#include "jsondata.h"

JsonData::JsonData()
{

}

JsonData::~JsonData()
{

}



MessageType JsonData::datatype()
{
    return Json;
}

MessageContent *JsonData::copy()
{
    return nullptr;
}

void JsonData::toStream(NetworkData &data)
{
    Q_UNUSED(data)
}

void JsonData::parserStream(NetworkData &data)
{
    Q_UNUSED(data)
}
