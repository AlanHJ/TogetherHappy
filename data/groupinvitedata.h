#ifndef GROUPINVITEDATA_H
#define GROUPINVITEDATA_H

#include "messagecontent.h"
#include "db/groupdata.h"

class GroupInviteData : public MessageContent
{
public:
#pragma pack(push)
#pragma pack(1)
    struct InviteHead
    {
        int updatetime;
        int port;
        qint8 deleteable;
        qint8 editable;
    };
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
    struct InviteStrSize
    {
        int uids;
        int names;
        int photos;
        int buildertimes;
        int creators;
        int dess;
        int members;
        int attrs;
        int masks;
        int images;
    };
#pragma pack(pop)
    GroupInviteData();
    virtual~GroupInviteData();
    void setGroupData(GroupData &d);
    GroupData groupData();

    // MessageContent interface
public:
    MessageType datatype();
    MessageContent *copy();
    void toStream(NetworkData &data);
    void parserStream(NetworkData &data);

private:
    GroupData gd;
    int size;

    QString parserStr(int len, char* buff, int *pos);
};

#endif // GROUPINVITEDATA_H
