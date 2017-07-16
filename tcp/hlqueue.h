#ifndef QMYQUEUE_H
#define QMYQUEUE_H
#include "../util/thmethod.h"

class HLQueue
{
public:
    HLQueue(U32 length);
    ~HLQueue();
    void ResetQueue(void);
    void ResetQueue(U32 length);
    int ReadQueue(IN char *pReadData, IN U32 read_len);
    int WriteQueue(IN char *pWritData, IN U32 write_len);
    int MoveQueueReadFlag(IN I32 iMoveLength);
    U32 GetQueueAvailLength(void);

protected:
    char*       m_pData;
    U32			m_ReadAddr;		//读地址
    U32			m_WriteAddr;		//写地址
    U32			m_SizeOfBuffer;	//队列长度
    U32			m_AvailLength;	//有效数据长度
    QMutex      m_Lock;
};

#endif // QMYQUEUE_H
