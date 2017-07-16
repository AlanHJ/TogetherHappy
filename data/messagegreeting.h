#ifndef MESSAGEGREETING_H
#define MESSAGEGREETING_H

#include "messagecontent.h"

class MessageGreeting : public MessageContent
{
public:
    MessageGreeting();
    virtual~MessageGreeting();
    void setGreetingKey(QString key);
    QString greetingKey();
    // MessageContent interface
public:
    MessageType datatype() override;
    MessageContent *copy() override;
    void toStream(NetworkData &data) override;
    void parserStream(NetworkData &data) override;
private:
    class PrivData;
    PrivData *mData;
};

#endif // MESSAGEGREETING_H
