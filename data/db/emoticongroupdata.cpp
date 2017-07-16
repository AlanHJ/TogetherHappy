#include "emoticongroupdata.h"

EmoticonGroupData::EmoticonGroupData(QObject *parent) : QObject(parent)
{
    setObjectName("emoticon_group");
    deleteable = false;
    editable = false;
    readable = true;
}

EmoticonGroupData::EmoticonGroupData(const EmoticonGroupData &data)
{
    setObjectName("emoticon_group");
    EmoticonGroupData &d = (EmoticonGroupData &)data;
    group = d.group;
    deleteable = d.deleteable;
    editable = d.editable;
    readable = d.readable;
}

EmoticonGroupData *EmoticonGroupData::operator=(const EmoticonGroupData &data)
{
    setObjectName("emoticon_group");
    EmoticonGroupData &d = (EmoticonGroupData &)data;
    group = d.group;
    deleteable = d.deleteable;
    editable = d.editable;
    readable = d.readable;
    return this;
}

EmoticonGroupData::~EmoticonGroupData()
{

}

QString EmoticonGroupData::getGroup() const
{
    return group;
}

void EmoticonGroupData::setGroup(const QString &value)
{
    group = value;
}

bool EmoticonGroupData::getDeleteable() const
{
    return deleteable;
}

void EmoticonGroupData::setDeleteable(bool value)
{
    deleteable = value;
}
bool EmoticonGroupData::getEditable() const
{
    return editable;
}

void EmoticonGroupData::setEditable(bool value)
{
    editable = value;
}
bool EmoticonGroupData::getReadable() const
{
    return readable;
}

void EmoticonGroupData::setReadable(bool value)
{
    readable = value;
}







