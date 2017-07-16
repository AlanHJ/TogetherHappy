#ifndef MESSAGEPONG_H
#define MESSAGEPONG_H

#include "messagecontent.h"

class MessagePong : public MessageContent
{
public:
    MessagePong();
    virtual~MessagePong();
    // MessageContent interface
public:
    MessageType datatype() override;
    MessageContent *copy() override;
    void toStream(NetworkData &data) override;
    void parserStream(NetworkData &data) override;
};

#endif // MESSAGEPONG_H
