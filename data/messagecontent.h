#ifndef MESSAGECONTENT_H
#define MESSAGECONTENT_H

#include <QObject>
#include "networkdata.h"
#include "../util/thmethod.h"

class MessageContent
{
public:
    MessageContent();
    virtual~MessageContent();
    virtual MessageType datatype() = 0;
    virtual MessageContent *copy() = 0;
    virtual void toStream(NetworkData &data) = 0;
    virtual void parserStream(NetworkData &data) = 0;
};

#endif // MESSAGECONTENT_H
