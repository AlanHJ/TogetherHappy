#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H

// 数据发送类
#include <QObject>
#include "networkdata.h"
#include "messagecontent.h"
#include "../util/thmethod.h"


#define TCP_HEAD 0xEEEEEEEE

class MessageData
{
public:
#pragma pack(push)
#pragma pack(1)
    struct MessageHead
    {
        qint32  tcphead;
        quint32 length;
        quint16 type;
        quint16 uidlength;
    };
#pragma pack(pop)
    MessageData();
    MessageData(const MessageData &data);
    virtual~MessageData();
    MessageData *operator=(const MessageData &data);

    void setDatatype(MessageType type);
    MessageType datatype();

    void setContent(MessageContent *content);
    MessageContent *content();

    void toStream(NetworkData &data);

    void setUid(QString uid);
    QString getuid();

private:
    class PrivData;
    PrivData *mData;
};

#endif // MESSAGEDATA_H
