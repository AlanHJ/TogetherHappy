#ifndef UPDATEDATA_H
#define UPDATEDATA_H

#include "messagecontent.h"


#define TCP_END  0xDDDDDDDD

class UpdateData : public MessageContent
{
public:
#pragma pack(push)
#pragma pack(1)
    struct UpdateHead
    {
        int size;
        int md5;
        int file;
    };
#pragma pack(pop)

    UpdateData();
    ~UpdateData();

    // MessageContent interface
public:
    MessageType datatype();
    MessageContent *copy();
    void toStream(NetworkData &data);
    void parserStream(NetworkData &data);
};

#endif // UPDATEDATA_H
