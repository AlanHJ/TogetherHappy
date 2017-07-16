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

#ifndef SQL_H
#define SQL_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>

class SqlDB
{
    friend class Sql;
public:
    SqlDB();
    virtual ~SqlDB();
    bool   transaction();
    void   close();
    void   commit();
    void   rollback();
    bool   insert(QObject* object,QString& error, bool flag = false);
    bool   update(QObject* object,QString pkname,QString& error, bool flag = false);
    bool   deldata(QObject* object,QString pkname,QString& error);
    bool   execute(QString sql,QString& error);
    QSqlQuery sqlQuery();

    QVariant  executeQuery(QString sql, int &rlt, QString& error);
    QList<QObject*> executeQuery(QString sql,const QMetaObject* metaobject,int& rlt,QString& error);
    QList<QVariant> executeQueryVariant(QString sql,int& rlt,QString& error);
protected:
    void   setUsed(int isuse=1);
    int    isUsed();
    void   setDatabase(QSqlDatabase database);
    QSqlDatabase getDatabase();
    void   setCheckSql(QString sql);
    long   getDuration();
    void   checkerror(QSqlError error);
private:
    class PrivData;
    PrivData* m_data;
};

class Sql
{
    friend class HeartThread;
private:
    Sql();
    virtual ~Sql();
public:
    static Sql* instance(QString instancename=QString("default"));
    static void remInstance(QString instancename=QString("default"));
    static void remAll();
    void   setDatabaseInfo(QString driverName, QString hostname,
                           int port, QString dbname,
                           QString username, QString password,
                           int maxconnection, int initconnection,
                           QString heartbeatSql);
    SqlDB* open(QString& error);
private:
    void   heartBeat();
    void   Init(int count);
private:
    class   PrivData;
    PrivData* m_data;

};

#endif // SQL_H
