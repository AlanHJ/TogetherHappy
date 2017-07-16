#include "groupinvitedata.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"

GroupInviteData::GroupInviteData()
{
    size = 0;
}

GroupInviteData::~GroupInviteData()
{

}

void GroupInviteData::setGroupData(GroupData &d)
{
    gd = d;
}

GroupData GroupInviteData::groupData()
{
    return gd;
}



MessageType GroupInviteData::datatype()
{
    return Invite;
}

MessageContent *GroupInviteData::copy()
{
    GroupInviteData *invite = new GroupInviteData;
    invite->setGroupData(gd);
    return invite;
}

void GroupInviteData::toStream(NetworkData &data)
{
    // ´ò°ü
    InviteStrSize strSize;
    QByteArray uidba = gd.getUid().toLocal8Bit();
    strSize.uids = uidba.length();
    QByteArray nameba = gd.getName().toLocal8Bit();
    strSize.names = nameba.length();
    QByteArray photoba = gd.getPhoto().toLocal8Bit();
    strSize.photos = photoba.length();
    QByteArray buildertimeba = gd.getCreationtime().toLocal8Bit();
    strSize.buildertimes = buildertimeba.length();
    QByteArray creatorba = gd.getCreator().toLocal8Bit();
    strSize.creators = creatorba.length();
    QByteArray desba = gd.getGroupdescribe().toLocal8Bit();
    strSize.dess = desba.length();
    QByteArray memberba = gd.getGroupmember().toLocal8Bit();
    strSize.members = memberba.length();
    QByteArray attrba = gd.getAttr().toLocal8Bit();
    strSize.attrs = attrba.length();
    QByteArray maskba = gd.getNetmask().toLocal8Bit();
    strSize.masks = maskba.length();

    InviteHead head;
    head.port = gd.getPort();
    head.updatetime = gd.getUpdatetime();
    head.deleteable = gd.getDeleteable() ? 1 : 0;
    head.editable = gd.getEditable() ? 1 : 0;

    size += sizeof(int);
    size += sizeof(InviteHead);
    size += sizeof(InviteStrSize);

    // add image
    QByteArray imageba;
    QFile file(gd.getPhoto());
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            imageba = file.readAll();
        }
        file.close();
    }
    strSize.images = imageba.length();
    size = size + strSize.uids + strSize.names + strSize.photos
            + strSize.buildertimes + strSize.creators + strSize.dess
            + strSize.members + strSize.attrs + strSize.masks
            + strSize.images;

    char *buff = new char[size];
    int pos = 0;
    memcpy(buff + pos, &size, sizeof(int));
    pos += sizeof(int);
    memcpy(buff + pos, &head, sizeof(InviteHead));
    pos += sizeof(InviteHead);
    memcpy(buff + pos, &strSize, sizeof(InviteStrSize));
    pos += sizeof(InviteStrSize);
    memcpy(buff + pos, uidba.data(), strSize.uids);
    pos += strSize.uids;
    memcpy(buff + pos, nameba.data(), strSize.names);
    pos += strSize.names;
    memcpy(buff + pos, photoba.data(), strSize.photos);
    pos += strSize.photos;
    memcpy(buff + pos, buildertimeba.data(), strSize.buildertimes);
    pos += strSize.buildertimes;
    memcpy(buff + pos, creatorba.data(), strSize.creators);
    pos += strSize.creators;
    memcpy(buff + pos, desba.data(), strSize.dess);
    pos += strSize.dess;
    memcpy(buff + pos, memberba.data(), strSize.members);
    pos += strSize.members;
    memcpy(buff + pos, attrba.data(), strSize.attrs);
    pos += strSize.attrs;
    memcpy(buff + pos, maskba.data(), strSize.masks);
    pos += strSize.masks;
    memcpy(buff + pos, imageba.data(), strSize.images);
    pos += strSize.images;
    if (pos == size)
    {
        data.appendData(buff, size);
    }
    else
    {
        qDebug() << "invite data package length error!!!";
    }
    delete[] buff;
    buff = nullptr;
}

void GroupInviteData::parserStream(NetworkData &data)
{
    // ½âÎö
    char *buff = data.data();
    int length = data.length();
    int pos = 0;
    memcpy(&size, buff + pos, sizeof(int));
    if (size != length)
    {
        qDebug() << "invite data parser length error!!!";
        return;
    }
    pos += sizeof(int);
    InviteHead head;
    memcpy(&head, buff + pos, sizeof(InviteHead));
    pos += sizeof(InviteHead);
    InviteStrSize strSize;
    memcpy(&strSize, buff + pos, sizeof(InviteStrSize));
    pos += sizeof(InviteStrSize);

    gd.setUid(parserStr(strSize.uids, buff, &pos));
    gd.setName(parserStr(strSize.names, buff, &pos));
    gd.setPhoto(parserStr(strSize.photos, buff, &pos));
    gd.setCreationtime(parserStr(strSize.buildertimes, buff, &pos));
    gd.setCreator(parserStr(strSize.creators, buff, &pos));
    gd.setGroupdescribe(parserStr(strSize.dess, buff, &pos));
    gd.setGroupmember(parserStr(strSize.members, buff, &pos));
    gd.setAttr(parserStr(strSize.attrs, buff, &pos));
    gd.setNetmask(parserStr(strSize.masks, buff, &pos));

    char *imagedata = new char[strSize.images];
    memcpy(imagedata, buff + pos, strSize.images);
    pos += strSize.images;
    if (pos != size)
    {
        qDebug() << "invite data parser length error!!!";
        delete[] imagedata;
        imagedata = nullptr;
        return;
    }
    QByteArray ba(imagedata, strSize.images);
    QByteArray md5ba = QCryptographicHash::hash(ba, QCryptographicHash::Md5);
    QString md5 = md5ba.toHex();
    QString path = GetWorkPath() + "/face";
    QString name = path + "/" + md5 + "." + QFileInfo(gd.getPhoto()).suffix();
    if (QDir().mkpath(path))
    {
        QFile file(name);
        if (!file.exists())
        {
            if (file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                file.write(ba);
            }
            file.close();
        }
    }
    delete[] imagedata;
    imagedata = nullptr;
    gd.setPhoto(name);
    gd.setUpdatetime(head.updatetime);
    gd.setPort(head.port);
    gd.setDeleteable(head.deleteable != 0 ? true : false);
    gd.setEditable(head.editable != 0 ? true : false);
}

QString GroupInviteData::parserStr(int len, char *buff, int *pos)
{
    char *strbuff = new char[len];
    memcpy(strbuff, buff + (*pos), len);
    (*pos) += len;
    QString str = QString::fromLocal8Bit(strbuff, len);
    delete[] strbuff;
    strbuff = nullptr;
    return str;
}
