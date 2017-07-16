#include "hlqueue.h"

HLQueue::HLQueue( U32 length)
{
   m_pData =  new char[length];
   m_ReadAddr = 0;
   m_WriteAddr = 0;
   m_SizeOfBuffer = length;
   m_AvailLength = 0;

}

HLQueue::~HLQueue()
{
    m_Lock.lock();
    if (NULL != m_pData)
    {
        free(m_pData);
    }
    m_pData = NULL;
    m_ReadAddr = 0;
    m_WriteAddr = 0;
    m_SizeOfBuffer = 0;
    m_AvailLength = 0;
    m_Lock.unlock();
}


void HLQueue::ResetQueue(void)
{
    m_Lock.lock();
    m_ReadAddr = 0;
    m_WriteAddr = 0;
    m_AvailLength = 0;
    m_Lock.unlock();
}

void HLQueue::ResetQueue(U32 length)
{
    m_Lock.lock();
    if (NULL != m_pData)
    {
        free(m_pData);
    }
    m_pData =  new char[length];
    m_SizeOfBuffer = length;
    m_ReadAddr = 0;
    m_WriteAddr = 0;
    m_AvailLength = 0;
    m_Lock.unlock();
}

int HLQueue::ReadQueue(IN char *pReadData, IN U32 read_len)
{
    if (m_AvailLength < read_len)
    {
        return -1;
    }

    m_Lock.lock();
    int len1 = (m_SizeOfBuffer - m_ReadAddr);
    if( len1 >= (int)read_len)
    {
        memcpy(pReadData, m_pData + m_ReadAddr  , read_len);
        m_ReadAddr = ((m_ReadAddr + read_len ) % m_SizeOfBuffer);
    }
    else
    {
        memcpy(pReadData,m_pData+m_ReadAddr ,len1);
        m_ReadAddr = ((m_ReadAddr + len1 ) % m_SizeOfBuffer);
        memcpy(pReadData + len1 ,m_pData + m_ReadAddr, (read_len-len1 ));
        m_ReadAddr = ((m_ReadAddr + read_len-len1 ) % m_SizeOfBuffer);
    }
    m_AvailLength = (m_AvailLength - read_len) % m_SizeOfBuffer;
    m_Lock.unlock();
    return 0;
}

int HLQueue::WriteQueue(IN char *pWritData, IN U32 write_len)
{
    if ((m_AvailLength + write_len) >= m_SizeOfBuffer)
    {
        return -1;
    }
    m_Lock.lock();
    U32 len1 = (m_SizeOfBuffer - m_WriteAddr);
    if( len1 >= write_len)
    {
        memcpy(&m_pData[m_WriteAddr ],pWritData,write_len);
        m_WriteAddr = ((m_WriteAddr+write_len  )%m_SizeOfBuffer);
    }
    else
    {
        memcpy(&m_pData[m_WriteAddr ],pWritData,len1 );
        m_WriteAddr = ((m_WriteAddr+len1 )%m_SizeOfBuffer);

        memcpy(&m_pData[m_WriteAddr],&pWritData[len1],(write_len-len1));
        m_WriteAddr = ((m_WriteAddr+write_len-
                        len1 )%m_SizeOfBuffer);

    }
    m_AvailLength = (m_AvailLength + write_len)%m_SizeOfBuffer;
    m_Lock.unlock();
    return 0;
}

int HLQueue::MoveQueueReadFlag(IN I32 iMoveLength)
{
    if ((U32)abs(iMoveLength) > m_SizeOfBuffer)
    {
        return -1;
    }
    m_Lock.lock();
    m_ReadAddr = (m_ReadAddr  + iMoveLength + m_SizeOfBuffer) % m_SizeOfBuffer;
    m_AvailLength = (m_AvailLength  - iMoveLength)%m_SizeOfBuffer;
    m_Lock.unlock();
    return 0;
}

U32 HLQueue::GetQueueAvailLength(void)
{
    return m_AvailLength;
}

