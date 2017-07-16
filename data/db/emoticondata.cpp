#include "emoticondata.h"

EmoticonData::EmoticonData(QObject *parent) : QObject(parent)
{
    setObjectName("emoticon");
    name = "";
    path = "";
    thumb = "";
    counts = 0;
    inserttime = "";
    lasttime = "";
    group = "";
    deleteable = false;
    editable = false;
    readable = true;
}

EmoticonData::EmoticonData(const EmoticonData &data)
{
    setObjectName("emoticon");
    EmoticonData &d = (EmoticonData &)data;
    name = d.name;
    path = d.path;
    thumb = d.thumb;
    counts = d.counts;
    inserttime = d.inserttime;
    lasttime = d.lasttime;
    group = d.group;
    deleteable = d.deleteable;
    editable = d.editable;
    readable = d.readable;
}

EmoticonData *EmoticonData::operator=(const EmoticonData &data)
{
    setObjectName("emoticon");
    EmoticonData &d = (EmoticonData &)data;
    name = d.name;
    path = d.path;
    thumb = d.thumb;
    counts = d.counts;
    inserttime = d.inserttime;
    lasttime = d.lasttime;
    group = d.group;
    deleteable = d.deleteable;
    editable = d.editable;
    readable = d.readable;
    return this;
}

EmoticonData::~EmoticonData()
{

}
QString EmoticonData::getName() const
{
    return name;
}

void EmoticonData::setName(const QString &value)
{
    name = value;
}
QString EmoticonData::getPath() const
{
    return path;
}

void EmoticonData::setPath(const QString &value)
{
    path = value;
}
int EmoticonData::getCounts() const
{
    return counts;
}

void EmoticonData::setCounts(int value)
{
    counts = value;
}
QString EmoticonData::getInserttime() const
{
    return inserttime;
}

void EmoticonData::setInserttime(const QString &value)
{
    inserttime = value;
}
QString EmoticonData::getLasttime() const
{
    return lasttime;
}

void EmoticonData::setLasttime(const QString &value)
{
    lasttime = value;
}
QString EmoticonData::getThumb() const
{
    return thumb;
}

void EmoticonData::setThumb(const QString &value)
{
    thumb = value;
}
QString EmoticonData::getGroup() const
{
    return group;
}

void EmoticonData::setGroup(const QString &value)
{
    group = value;
}
bool EmoticonData::getDeleteable() const
{
    return deleteable;
}

void EmoticonData::setDeleteable(bool value)
{
    deleteable = value;
}
bool EmoticonData::getEditable() const
{
    return editable;
}

void EmoticonData::setEditable(bool value)
{
    editable = value;
}
bool EmoticonData::getReadable() const
{
    return readable;
}

void EmoticonData::setReadable(bool value)
{
    readable = value;
}











