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

#include "sql.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "heartthread.h"
#include "sqlmetaobject.h"
#include <QDebug>

#define  DURATION_FREE  (300000) //5分钟
#define  GETDBTIMEOUT   (2000)    //2秒
#define  HEARTBEATTIMER (60000)    //1分钟
static QHash<QString,Sql*> g__SqlHash;
static QMutex sqlmutex;
class Sql::PrivData
{
public:
    PrivData()
    {
        maxconnection = 1;
        initconnection = 1;
        QObject::connect(&timer,SIGNAL(timeout()),&heartObject,SLOT(slotHeartBeat()));
        timer.start(HEARTBEATTIMER);
    }
    ~PrivData()
    {
        timer.stop();
        clear();
    }

    void   clear()
    {
        foreach (SqlDB* database, sqllist) {
            delete database;
        }
        sqllist.clear();
    }

    SqlDB* getFreeDatabase(int & rlt , QString& error)
    {
        rlt = 0;
        QList<SqlDB*> list;
        foreach (SqlDB* db, sqllist) {
            if(db->isUsed()<0)
            {
                list.append(db);
            }
        }
        foreach (SqlDB* db, list) {
            sqllist.removeOne(db);
            delete db;
        }
        foreach (SqlDB* db, sqllist) {
            if(db->isUsed()==0)
            {
                db->setUsed(1);
                return db;
            }
        }
        if( sqllist.count()>= maxconnection )
        {
            return nullptr;
        }
        QSqlDatabase database = QSqlDatabase::addDatabase(dbdriver,QString("%1").arg(QUuid::createUuid().toString()));
        database.setDatabaseName(dbname);
        database.setHostName( hostname );
        database.setPort(port );
        database.setUserName( username );
        database.setPassword( password );
        if( database.open()==false )
        {
            rlt = -1;
            error = QTextCodec::codecForLocale()->toUnicode("执行数据库操作失败:") + database.lastError().text();
            return nullptr;
        }
        SqlDB* db = new SqlDB();
        db->setCheckSql(heartbeatSql);
        db->setUsed(true);
        db->setDatabase(database);
        sqllist.append(db);
        return db;
    }

public:
    QString dbdriver;
    QString hostname;
    int     port;
    QString dbname;
    QString username;
    QString password;
    QList<SqlDB*> sqllist;
    QString heartbeatSql;
    int     maxconnection;
    int     initconnection;
    QTimer  timer;
    HeartThread heartObject;
};

Sql::Sql()
{
    m_data = new PrivData();
    m_data->heartObject.setSql(this);
}

Sql::~Sql()
{
    delete m_data;
}

Sql *Sql::instance(QString instancename)
{
    QMutexLocker lock(&sqlmutex);
    Sql* sql = g__SqlHash.value(instancename,nullptr);
    if( sql == nullptr )
    {
        sql = new Sql();
        g__SqlHash.insert(instancename,sql);
    }
    return sql;
}

void Sql::remInstance(QString instancename)
{
    QMutexLocker lock(&sqlmutex);
    Sql* sql = g__SqlHash.value(instancename,nullptr );
    if( sql != nullptr )
    {
        delete sql;
    }
    g__SqlHash.remove(instancename);
}

void Sql::remAll()
{
    QMutexLocker lock(&sqlmutex);
    auto it = g__SqlHash.begin();
    while( it != g__SqlHash.end() )
    {
        Sql* sql = it.value();
        delete sql;
        ++it;
    }
    g__SqlHash.clear();
}

void Sql::setDatabaseInfo(QString driverName, QString hostname,
                          int port, QString dbname,
                          QString username, QString password,
                          int maxconnection,int initconnection,
                          QString heartbeatSql)
{
    QMutexLocker lock(&sqlmutex);
    m_data->clear();
    m_data->dbdriver = driverName;
    m_data->hostname = hostname;
    m_data->port     = port;
    m_data->dbname   = dbname;
    m_data->username = username;
    m_data->password = password;
    m_data->heartbeatSql = heartbeatSql;
    m_data->maxconnection = maxconnection;
    m_data->initconnection = initconnection;
    Init(m_data->initconnection);
}

SqlDB *Sql::open(QString &error)
{

    QTime time;
    time.start();
    int rlt = 0;
    SqlDB* db = nullptr;
    while( db == nullptr )
    {
        db = m_data->getFreeDatabase(rlt,error);
        if( db != nullptr )
        {
            return db;
        }
        if( rlt <0 )
        {
            break;
        }
        if( time.elapsed()>= GETDBTIMEOUT )
        {
            break;
        }
        QThread::msleep(10);
    }
    if( db == nullptr && rlt == 0)
    {
        error = QTextCodec::codecForLocale()->toUnicode("获取数据库对象超时");
    }
    return db;
}

void Sql::heartBeat()
{
    QList<SqlDB*> extraList;
    foreach (SqlDB* db, m_data->sqllist)
    {
        if( db->isUsed() == false && db->getDuration() >= DURATION_FREE )
        {
            QString error;
            if( db->execute(m_data->heartbeatSql,error)== false )
            {
                qDebug() << QTextCodec::codecForLocale()->toUnicode("执行心跳查询失败:")+error;
                db->getDatabase().close();
                QSqlDatabase database = QSqlDatabase::addDatabase(m_data->dbdriver);
                database.setDatabaseName(m_data->dbname);
                database.setHostName( m_data->hostname );
                database.setPort(m_data->port );
                database.setUserName( m_data->username );
                database.setPassword( m_data->password );
                if( database.open()==false )
                {
                    extraList.append(db);
                }
                else
                {
                    db->setDatabase(database);
                }
            }
        }
    }
    foreach (SqlDB* db, extraList) {
        m_data->sqllist.removeOne(db);
        delete db;
    }
    Init(m_data->initconnection - m_data->sqllist.count());
}

void Sql::Init(int count)
{

    for(int i=0;i<count;i++ )
    {
        QString error;
        SqlDB* db = open(error);
        if( db == nullptr )
        {
            qDebug() << error;
        }
        else
        {
            db->setUsed(false);
        }
    }
}

class SqlDB::PrivData
{
public:
    PrivData()
    {
        isuse = 0;
        transaction = false;
        time.start();
        checksql = "SELECT UUID()";
    }
    ~PrivData()
    {
        QMutexLocker lock(&mutex);
        if(database.isOpen() )
            database.close();
    }
public:
    QSqlDatabase database;
    int          isuse;
    QMutex       mutex;
    QTime        time;
    bool         transaction;
    QString      checksql;
};

SqlDB::SqlDB()
{
    m_data = new PrivData();
}

SqlDB::~SqlDB()
{
    delete m_data;
}

bool SqlDB::transaction()
{
    QMutexLocker lock(&m_data->mutex);
    m_data->transaction = m_data->database.transaction();
    return m_data->transaction;
}

void SqlDB::close()
{
    QMutexLocker lock(&m_data->mutex);
    checkerror(m_data->database.lastError());
    if( m_data->transaction == true )
    {
        if( m_data->database.lastError().NoError )
            m_data->database.commit();
        else
            m_data->database.rollback();
    }
    m_data->transaction = false;
    if( m_data->isuse >=0 )
    {
        m_data->isuse = 0;
    }
}

void SqlDB::commit()
{
    m_data->database.commit();
}

void SqlDB::rollback()
{
    m_data->database.rollback();
}

bool SqlDB::insert(QObject *object, QString &error, bool flag)
{
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    SqlMetaObject meta(object,&query, flag);
    if(  meta.insert() == false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        return false;
    }
    return true;
}

bool SqlDB::update(QObject *object, QString pkname, QString &error, bool flag)
{
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    SqlMetaObject meta(object,&query, flag);
    if( meta.update(pkname) == false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        return false;
    }
    return true;
}

bool SqlDB::deldata(QObject *object, QString pkname, QString &error)
{
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    SqlMetaObject meta(object,&query);
    if( meta.del(pkname) == false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        return false;
    }
    return true;
}

bool SqlDB::execute(QString sql, QString &error)
{
//    qDebug() << sql;
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    if( query.exec(sql)==false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        qDebug() << "sql : error :"<< error;
        return false;
    }
    return true;
}

QSqlQuery SqlDB::sqlQuery()
{
   QSqlQuery query(m_data->database);
   return query;
}



QVariant SqlDB::executeQuery(QString sql, int &rlt, QString &error)
{
//    qDebug() << sql;
    QVariant var;
    rlt = 0;
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    if( query.exec(sql)==false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        rlt = -1;
    }
    if( query.next() )
    {
        var = query.value(0);
    }
    else
    {
        rlt = 1;
    }
    return var;
}

QList<QObject *> SqlDB::executeQuery(QString sql, const QMetaObject *metaobject, int &rlt, QString &error)
{
//    qDebug() << sql;
    QList<QObject*> list;
    rlt = 0;
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    if( query.exec(sql)==false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        rlt = -1;
    }
    while( query.next() )
    {
        QObject* object = SqlMetaObject::newInstance(metaobject,&query);
        if(object != nullptr )
        {
            list.append(object);
        }
    }

    return list;
}

QList<QVariant> SqlDB::executeQueryVariant(QString sql, int &rlt, QString &error)
{
//    qDebug() << sql;
    QList<QVariant> list;
    rlt = 0;
    QMutexLocker lock(&m_data->mutex);
    m_data->time.restart();
    QSqlQuery query(m_data->database);
    if( query.exec(sql)==false )
    {
        checkerror(query.lastError());
        error = query.lastError().text();
        rlt = -1;
    }
    while( query.next() )
    {
        list.append(query.value(0));
    }

    return list;
}

void SqlDB::setUsed(int isuse)
{
    QMutexLocker lock(&m_data->mutex);
    m_data->isuse = isuse;
}

int SqlDB::isUsed()
{
    return m_data->isuse;
}

void SqlDB::setDatabase(QSqlDatabase database)
{
    m_data->database = database;
}

QSqlDatabase SqlDB::getDatabase()
{
    return m_data->database;
}

void SqlDB::setCheckSql(QString sql)
{
    m_data->checksql = sql;
}

long SqlDB::getDuration()
{
    return m_data->time.elapsed();
}

void SqlDB::checkerror(QSqlError error)
{
    QSqlError::ErrorType errortype = error.type();
    if( errortype == QSqlError::ConnectionError )
    {
        m_data->isuse = -1;
    }
    QSqlQuery query(m_data->database);
    if( query.exec(m_data->checksql)==false )
    {
        m_data->isuse = -1;
    }
}
