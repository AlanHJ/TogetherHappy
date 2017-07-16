#include "matchdata.h"

MatchData::MatchData(QObject *parent) : QObject(parent)
{
    setObjectName("match_person");
}

MatchData::MatchData(const MatchData &data)
{
    setObjectName("match_person");
    MatchData &d = (MatchData&)data;
    uid = d.uid;
    ip = d.ip;
    editname = d.editname;
    nickname = d.nickname;
    modifycounts = d.modifycounts;
    edittime = d.edittime;
}

MatchData *MatchData::operator=(const MatchData &data)
{
    setObjectName("match");
    MatchData &d = (MatchData&)data;
    uid = d.uid;
    ip = d.ip;
    editname = d.editname;
    nickname = d.nickname;
    modifycounts = d.modifycounts;
    edittime = d.edittime;
    return this;
}

MatchData::~MatchData()
{

}
QString MatchData::getUid() const
{
    return uid;
}

void MatchData::setUid(const QString &value)
{
    uid = value;
}
QString MatchData::getIp() const
{
    return ip;
}

void MatchData::setIp(const QString &value)
{
    ip = value;
}

QString MatchData::getEditname() const
{
    return editname;
}

void MatchData::setEditname(const QString &value)
{
    editname = value;
}
QString MatchData::getNickname() const
{
    return nickname;
}

void MatchData::setNickname(const QString &value)
{
    nickname = value;
}
int MatchData::getModifycounts() const
{
    return modifycounts;
}

void MatchData::setModifycounts(int value)
{
    modifycounts = value;
}

int MatchData::getEdittime() const
{
    return edittime;
}

void MatchData::setEdittime(int value)
{
    edittime = value;
}







