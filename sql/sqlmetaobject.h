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
