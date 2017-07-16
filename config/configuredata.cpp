#include "configuredata.h"
#include "../sql/sql.h"
#include "../data/db/configdata.h"
#include <QtConcurrent/QtConcurrent>

static ConfigureData *ConfigmInstance = nullptr;
static QMutex confMutex;
class ConfigureData::PrivData
{
public:
    PrivData()
    {

    }
    ~PrivData()
    {
        valueHash.clear();
    }
public:
    QHash<QString, QVariant> valueHash;
    QMutex mutex;
};

ConfigureData::ConfigureData()
{
    mData = new PrivData();
}

ConfigureData::~ConfigureData()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

ConfigureData *ConfigureData::getInstance()
{
    QMutexLocker lock(&confMutex);
    if (!ConfigmInstance)
    {
        ConfigmInstance = new ConfigureData();
    }
    return ConfigmInstance;
}

void ConfigureData::disInstance()
{
    if (nullptr != ConfigmInstance)
    {
        delete ConfigmInstance;
        ConfigmInstance = nullptr;
    }
}

void ConfigureData::initConfig()
{
    QString error;
    SqlDB *db = Sql::instance()->open(error);
    if (!db)
        return;
    int rlt = 0;
    QString sql = "SELECT * FROM " + ConfigData().objectName();
    QList<QObject *> vals = db->executeQuery(sql, ConfigData().metaObject(), rlt, error);
    if (rlt != 0)
        qDebug() << QStringLiteral("¶ÁÈ¡ÅäÖÃÊý¾Ý¿âÊ§°Ü:") << error;
    auto it = vals.begin();
    while (it != vals.end())
    {
        ConfigData *cd = qobject_cast<ConfigData *>(*it);
        if (cd)
        {
            mData->valueHash.insert(cd->getKey(), cd->getValue());
        }
        delete cd;
        cd = nullptr;
        ++it;
    }
}


bool ConfigureData::readIni(QString key, QVariant &value, QVariant deval)
{
    QMutexLocker lock(&confMutex);
    bool b = mData->valueHash.contains(key);
    if (b)
    {
        value = mData->valueHash.value(key);
    }
    else
    {
        value = deval;
    }
    return b;
}

bool ConfigureData::writeIni(QString key, QVariant val)
{
    QMutexLocker lock(&confMutex);
    bool b = mData->valueHash.contains(key);
    mData->valueHash.insert(key, val);
    QtConcurrent::run(this, &ConfigureData::writedb, key, val);
    return b;
}

void ConfigureData::writedb(QString key, QVariant var)
{
    QMutexLocker lock(&mData->mutex);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
        return;
    int rlt = 0;
    ConfigData cd;
    cd.setKey(key);
    cd.setValue(var.toString());
    QString sql = "SELECT COUNT(*) FROM " + ConfigData().objectName()+
            " WHERE key = '" + key + "'";
    QVariant tempvar = db->executeQuery(sql, rlt, er);
    if (rlt != 0)
    {
        qDebug() << "select resource counts error:" + er +
                     " key:" + key;
    }
    else
    {
        bool b = true;
        if (tempvar.toInt() == 0)
        {
            b = db->insert(&cd, er);
            if (!b)
            {
                qDebug() << "insert key:" + key + " error:" << er;
            }
        }
        else
        {
            b = db->update(&cd, "key", er);
            if (!b)
            {
                qDebug() << "update key:" + key + " error:" << er;
            }
        }
    }
    db->close();
}

void ConfigureData::writeDbAll()
{
    QMutexLocker lock(&confMutex);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
        return;
    int rlt = 0;
    auto it = mData->valueHash.begin();
    while (it != mData->valueHash.end())
    {
        QString k = it.key();
        QVariant var = it.value();
        ConfigData cd;
        cd.setKey(k);
        cd.setValue(var.toString());
        QString sql = "SELECT COUNT(*) FROM " + ConfigData().objectName()+
                " WHERE key = '" + k + "'";
        QVariant tempvar = db->executeQuery(sql, rlt, er);
        if (rlt != 0)
        {
            qDebug() << "select resource counts error:" + er +
                         " key:" + k;
        }
        else
        {
            bool b = true;
            if (tempvar.toInt() == 0)
            {
                b = db->insert(&cd, er);
                if (!b)
                {
                    qDebug() << "insert key:" + k + " error:" << er;
                }
            }
            else
            {
                b = db->update(&cd, "key", er);
                if (!b)
                {
                    qDebug() << "update key:" + k + " error:" << er;
                }
            }
        }
        ++it;
    }
    db->close();
}
