#ifndef MESSAGECHARDATA_H
#define MESSAGECHARDATA_H

#include "messagecontent.h"

class MessageCharData : public MessageContent
{
public:
    MessageCharData();
    virtual~MessageCharData();
    void setContent(QByteArray content);
    QByteArray content();
    void setType(int type);
    int type();
    // MessageContent interface
public:
    virtual MessageType datatype();
    virtual MessageContent *copy();
    virtual void toStream(NetworkData &data);
    virtual void parserStream(NetworkData &data);
private:
    class PrivData;
    PrivData *mData;
};

#endif // MESSAGECHARDATA_H
