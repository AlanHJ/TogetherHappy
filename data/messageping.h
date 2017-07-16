#ifndef MESSAGEPING_H
#define MESSAGEPING_H

#include "messagecontent.h"

class MessagePing : public MessageContent
{
public:
    MessagePing();
    virtual~MessagePing();
    // MessageContent interface
public:
    MessageType datatype() override;
    MessageContent *copy() override;
    void toStream(NetworkData &data) override;
    void parserStream(NetworkData &data) override;
};

#endif // MESSAGEPING_H
