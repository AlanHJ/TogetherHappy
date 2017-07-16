#include "messagedatadeal.h"
#include "../data/messagecontent.h"
#include "../data/messagegreeting.h"
#include "../data/messageping.h"
#include "../data/messagepong.h"
#include "../data/messagerichtext.h"
#include "../data/filedatacontent.h"
#include "../data/messagechardata.h"
#include "../data/groupinvitedata.h"

class MessageDataDeal::PrivData
{
public:
    PrivData()
    {
        contentlst << new MessagePing()
                   << new MessagePong()
                   << new MessageGreeting()
                   << new MessageRichText()
                   << new FileDataContent()
                   << new MessageCharData()
                   << new GroupInviteData();
    }
    ~PrivData()
    {
        auto it = contentlst.begin();
        while (it != contentlst.end())
        {
            MessageContent *content = *it;
            delete content;
            content = nullptr;
            ++it;
        }
        contentlst.clear();
    }
public:
    QList<MessageContent *> contentlst;
};

MessageDataDeal::MessageDataDeal(QObject *parent) : QObject(parent)
{
    mData = new PrivData();
}

MessageDataDeal::~MessageDataDeal()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void MessageDataDeal::ParserFrameData(NetworkData data)
{
    MessageData::MessageHead head;
    char *buff = data.data();
    int length = data.length();
    int headlength = sizeof(MessageData::MessageHead);
    memcpy(&head, buff, headlength);
    char *uidbuff = new char[head.uidlength];
    memcpy(uidbuff, buff + headlength, head.uidlength);
    QString uid = QString::fromLocal8Bit(uidbuff, head.uidlength);
    delete[] uidbuff;
    uidbuff = nullptr;

    auto it = mData->contentlst.begin();
    while (it != mData->contentlst.end())
    {
        MessageContent *content = *it;
        if (content->datatype() == head.type)
        {
            NetworkData nd(buff + headlength + head.uidlength,
                           length - headlength - head.uidlength);
            content->parserStream(nd);
            MessageData msgdata;
            msgdata.setUid(uid);
            msgdata.setDatatype(content->datatype());
            msgdata.setContent(content->copy());
            emit recvMessageData(msgdata);
            break;
        }
        ++it;
    }
}
