#ifndef MESSAGERICHTEXT_H
#define MESSAGERICHTEXT_H

#include "messagecontent.h"

// 采用Html进行完成
class MessageRichText : public MessageContent
{
public:
#pragma pack(push)
#pragma pack(1)
    struct RichTextHead
    {
        qint32 totalleng;
        qint32 htmllength;
        qint16 imagecounts;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    struct ImageSrc
    {
        qint16 positon;
        qint16 imageTypeLength;
        qint32 imagelength;
    };
#pragma pack(pop)

    MessageRichText();
    virtual~MessageRichText();
    void setMessageHtml(QString html);
    QString messageHtml();
    QStringList messageImagePath();
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

#endif // MESSAGERICHTEXT_H
