#include "messageping.h"

MessagePing::MessagePing()
{

}

MessagePing::~MessagePing()
{

}

MessageType MessagePing::datatype()
{
    return Ping;
}

MessageContent *MessagePing::copy()
{
    MessagePing *ping = new MessagePing();
    return ping;
}

void MessagePing::toStream(NetworkData &/*data*/)
{
}

void MessagePing::parserStream(NetworkData &/*data*/)
{
}
