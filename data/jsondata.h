#ifndef JSONDATA_H
#define JSONDATA_H

#include "messagecontent.h"

class JsonData : public MessageContent
{
public:
    JsonData();
    ~JsonData();

    // MessageContent interface
public:
    virtual MessageType datatype();
    virtual MessageContent *copy();
    virtual void toStream(NetworkData &data);
    virtual void parserStream(NetworkData &data);
};

#endif // JSONDATA_H
