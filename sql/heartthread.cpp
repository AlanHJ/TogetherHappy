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

#include "heartthread.h"
#include "sql.h"
HeartThread::HeartThread()
{
    m_Sql = nullptr;
    setObjectName("sql HeartThread");
}

HeartThread::~HeartThread()
{

}

void HeartThread::setSql(Sql *sql)
{
    m_Sql = sql;
}

void HeartThread::slotHeartBeat()
{
    if( m_Sql == nullptr )
        return;
    m_Sql->heartBeat();
}

