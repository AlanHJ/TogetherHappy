/************************************************************************
* ��Ȩ���� (C)2016, �����������¼������޹�˾��
*
* �ļ����ƣ� // �ļ���
* �ļ���ʶ�� // �����ù���ƻ���
* ����ժҪ�� // ��Ҫ�������ļ������ݣ�������Ҫģ�顢�������书�ܵ�˵��
* ����˵���� // �������ݵ�˵��
* ��ǰ�汾�� // v1.0
* ��    �ߣ� // ��˳��
* ������ڣ� //
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
