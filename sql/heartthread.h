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

#ifndef HEARTTHREAD_H
#define HEARTTHREAD_H
#include <QtCore>
class Sql;
class HeartThread : public QObject
{
    Q_OBJECT
public:
    HeartThread();
    virtual ~HeartThread();
    void    setSql(Sql* sql);
public slots:
    void    slotHeartBeat();
private:
    Sql*    m_Sql;
};

#endif // HEARTTHREAD_H
