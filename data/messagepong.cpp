#include "messagepong.h"

MessagePong::MessagePong()
{

}

MessagePong::~MessagePong()
{

}

MessageType MessagePong::datatype()
{
    return Pong;
}


MessageContent *MessagePong::copy()
{
    MessagePong *msg = new MessagePong();
    return msg;
}

void MessagePong::toStream(NetworkData &/*data*/)
{
}

void MessagePong::parserStream(NetworkData &/*data*/)
{
}
