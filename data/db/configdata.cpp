#include "configdata.h"

ConfigData::ConfigData(QObject *parent) : QObject(parent)
{
    setObjectName("config");
}

ConfigData::ConfigData(const ConfigData &data)
{
    setObjectName("config");
    ConfigData &d = (ConfigData&)data;
    k = d.k;
    val = d.val;
}

ConfigData *ConfigData::operator=(const ConfigData &data)
{
    setObjectName("config");
    ConfigData &d = (ConfigData&)data;
    k = d.k;
    val = d.val;
    return this;
}

ConfigData::~ConfigData()
{

}
QString ConfigData::getKey() const
{
    return k;
}

void ConfigData::setKey(const QString &value)
{
    this->k = value;
}
QString ConfigData::getValue() const
{
    return val;
}

void ConfigData::setValue(const QString &value)
{
    this->val = value;
}
