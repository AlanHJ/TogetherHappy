#include "groupdata.h"

GroupData::GroupData(QObject *parent) : QObject(parent)
{
    setObjectName("group_organization");
    deleteable = false;
    editable = false;
    updatetime = 0;
    port = 24661;
}

GroupData::GroupData(const GroupData &data)
{
    setObjectName("group_organization");
    GroupData &d = (GroupData &)data;
    uid = d.uid;
    name = d.name;
    photo = d.photo;
    deleteable = d.deleteable;
    editable = d.editable;
    creationtime = d.creationtime;
    updatetime = d.updatetime;
    creator = d.creator;
    groupdescribe = d.groupdescribe;
    groupmember = d.groupmember;
    attr = d.attr;
    netmask = d.netmask;
    port = d.port;
}

GroupData *GroupData::operator=(const GroupData &data)
{
    GroupData &d = (GroupData &)data;
    uid = d.uid;
    name = d.name;
    photo = d.photo;
    deleteable = d.deleteable;
    editable = d.editable;
    creationtime = d.creationtime;
    updatetime = d.updatetime;
    creator = d.creator;
    groupdescribe = d.groupdescribe;
    groupmember = d.groupmember;
    attr = d.attr;
    netmask = d.netmask;
    port = d.port;
    return this;
}

GroupData::~GroupData()
{

}
QString GroupData::getUid() const
{
    return uid;
}

void GroupData::setUid(const QString &value)
{
    uid = value;
}
QString GroupData::getName() const
{
    return name;
}

void GroupData::setName(const QString &value)
{
    name = value;
}
bool GroupData::getDeleteable() const
{
    return deleteable;
}

void GroupData::setDeleteable(bool value)
{
    deleteable = value;
}
bool GroupData::getEditable() const
{
    return editable;
}

void GroupData::setEditable(bool value)
{
    editable = value;
}
QString GroupData::getCreationtime() const
{
    return creationtime;
}

void GroupData::setCreationtime(const QString &value)
{
    creationtime = value;
}
int GroupData::getUpdatetime() const
{
    return updatetime;
}

void GroupData::setUpdatetime(int value)
{
    updatetime = value;
}
QString GroupData::getCreator() const
{
    return creator;
}

void GroupData::setCreator(const QString &value)
{
    creator = value;
}
QString GroupData::getGroupdescribe() const
{
    return groupdescribe;
}

void GroupData::setGroupdescribe(const QString &value)
{
    groupdescribe = value;
}
QString GroupData::getGroupmember() const
{
    return groupmember;
}

void GroupData::setGroupmember(const QString &value)
{
    groupmember = value;
}
QString GroupData::getAttr() const
{
    return attr;
}

void GroupData::setAttr(const QString &value)
{
    attr = value;
}
QString GroupData::getNetmask() const
{
    return netmask;
}

void GroupData::setNetmask(const QString &value)
{
    netmask = value;
}
QString GroupData::getPhoto() const
{
    return photo;
}

void GroupData::setPhoto(const QString &value)
{
    photo = value;
}
int GroupData::getPort() const
{
    return port;
}

void GroupData::setPort(int value)
{
    port = value;
}














