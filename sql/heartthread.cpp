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

