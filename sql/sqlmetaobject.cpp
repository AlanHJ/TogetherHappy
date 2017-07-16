/************************************************************************
* 版权所有 (C)2016, 重庆会凌电子新技术有限公司。
*
* 文件名称： // 文件名
* 文件标识： // 见配置管理计划书
* 内容摘要： // 简要描述本文件的内容，包括主要模块、函数及其功能的说明
* 其它说明： // 其它内容的说明
* 当前版本： // v1.0
* 作    者： // 赖顺有
* 完成日期： //
*
************************************************************************/

#include "sqlmetaobject.h"
#include <QSqlQuery>
#include <QSqlRecord>

class SqlMetaObject::PrivData
{
public:
    PrivData()
    {
        object = nullptr;
        query = nullptr;
    }
    ~PrivData()
    {

    }
    void     Init(bool flag)
    {
        const QMetaObject* metaobj = object->metaObject();
        int count = metaobj->propertyCount();
        for( int i= (flag ? 0 : metaobj->propertyOffset());i<count;i++ )
        {
            QMetaProperty property = metaobj->property(i);
            QVariant var = property.read(object);
            QString str(property.name());
            if (str.compare("objectname", Qt::CaseInsensitive) == 0)
            {
                continue;
            }
            values.insert(property.name(),var.value<QString>());
            propertyList.append(property);
        }
    }

public:
    QObject* object;
    QHash<QString,QString> values;
    QList<QMetaProperty>   propertyList;
    QSqlQuery* query;
};

SqlMetaObject::SqlMetaObject(QObject *object, QSqlQuery *query, bool flag)
{
    m_data = new PrivData();
    m_data->object = object;
    m_data->query = query;
    m_data->Init(flag);
}

SqlMetaObject::~SqlMetaObject()
{
    delete m_data;
}

bool SqlMetaObject::insert()
{
    QString sql = QString("INSERT INTO ") + m_data->object->objectName() +" ( ";
    QString sqlv = " VALUES ( ";
    auto it = m_data->values.begin();
    QStringList vlist;
    while( it != m_data->values.end() )
    {
        if( it != m_data->values.begin() )
        {
            sql += QString(",");
            sqlv += QString(",");
        }
        sql += QString(" %1 ").arg(it.key());
        sqlv += QString(" ? ");
        vlist << it.value();
        ++it;
    }
    sql += " ) ";
    sqlv += " ) ";
    sql += sqlv;
    if( m_data->query->prepare(sql)==false )
        return false;
    for(int i=0;i<vlist.size();i++)
    {
        QVariantList varlist;
        varlist << vlist.at(i);
        m_data->query->addBindValue(varlist);
    }
    return m_data->query->execBatch();
}

bool SqlMetaObject::update(QString pkname)
{
    QString sql="";
    auto it = m_data->values.find(pkname);
    if( it == m_data->values.end() )
        return false;
    QString pkvalue = it.value();
    sql = QString("UPDATE ") + m_data->object->objectName() +" SET";
    it = m_data->values.begin();
    QStringList vlist;
    while( it != m_data->values.end() )
    {
        if( it != m_data->values.begin() )
        {
            sql += QString(",");
        }
        sql += QString(" %1=? ").arg(it.key());
        vlist << it.value();
        ++it;
    }
    sql += " WHERE " + pkname+"=? ";
    vlist.append(pkvalue);
    if( m_data->query->prepare(sql)==false )
        return false;
    for (int i = 0; i < vlist.size(); i++)
    {
        QVariantList varlist;
        varlist << vlist.at(i);
        m_data->query->addBindValue(varlist);
    }
    return m_data->query->execBatch();
}

bool SqlMetaObject::del(QString pkname)
{
    QString sql="";
    auto it = m_data->values.find(pkname);
    if( it == m_data->values.end() )
        return false;
    QString pkvalue = it.value();
    sql = QString("DELETE FROM ") + m_data->object->objectName() +" WHERE " + pkname+"=?";
    QStringList vlist;
    vlist.append(pkvalue);
    if( m_data->query->prepare(sql)==false )
        return false;
    m_data->query->addBindValue(vlist);
    return m_data->query->execBatch();
}

int SqlMetaObject::load(QString pkname)
{
    QString sql="";
    auto it = m_data->values.find(pkname);
    if( it == m_data->values.end() )
        return -1;
    QString pkvalue = it.value();
    sql = QString("SELECT "); //+ m_data->object->objectName() +" ";
    it = m_data->values.begin();
    QStringList vlist;
    while( it != m_data->values.end() )
    {
        if( it != m_data->values.begin() )
        {
            sql += QString(",");
        }
        sql += QString(" %1 ").arg(it.key());
        ++it;
    }
    sql += " FROM "+ m_data->object->objectName()+ " WHERE " + pkname+"=? ";
    vlist.append(pkvalue);
    if( m_data->query->prepare(sql)==false )
        return -1;
    m_data->query->addBindValue(vlist);
    if( m_data->query->execBatch(QSqlQuery::ValuesAsColumns)==false )
        return -1;
    if(m_data->query->next())
    {
        for(int i=0;i<m_data->propertyList.count();i++)
        {
            QMetaProperty property = m_data->propertyList.at(i);
            QString value = m_data->query->value(property.name()).toString();
            property.write(m_data->object,QVariant::fromValue(value));
        }

        return 0;
    }
    return 1;
}

QObject *SqlMetaObject::object()
{
    return m_data->object;
}

QObject *SqlMetaObject::newInstance(const QMetaObject *metaobj, QSqlQuery *query)
{
    QSqlRecord record =   query->record();
    if( record.count() <0 )
        return nullptr;
    bool isWrite = false;
    QObject* object = metaobj->newInstance();
    for(int i=0;i<metaobj->propertyCount();i++ )
    {
        QMetaProperty property = metaobj->property(i);
        if(query->record().contains(property.name()))
        {
            QVariant var = query->value(property.name() );
            if( var.isValid() )
            {
                property.write(object,var);
                isWrite = true;
            }
        }
    }
    if( isWrite == false )
    {
        delete object;
        return nullptr;
    }
    return object;
}


