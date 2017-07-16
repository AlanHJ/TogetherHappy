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

#ifndef SQLMETAOBJECT_H
#define SQLMETAOBJECT_H
#include <QtCore>
#include <QSqlDatabase>

class SqlMetaObject
{
public:
    SqlMetaObject(QObject* object, QSqlQuery* query, bool flag = false);
    virtual ~SqlMetaObject();
    bool insert();
    bool update( QString pkname);
    bool del( QString pkname);
    int  load( QString pkname );
    QObject* object();
    static QObject* newInstance(const QMetaObject* metaobj,QSqlQuery* query);
private:
    class PrivData;
    PrivData* m_data;
};

#endif // SQLMETAOBJECT_H
