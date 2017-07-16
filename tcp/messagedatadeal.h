#ifndef MESSAGEDATADEAL_H
#define MESSAGEDATADEAL_H

#include <QObject>
#include "../data/messagedata.h"

class MessageDataDeal : public QObject
{
    Q_OBJECT
public:
    explicit MessageDataDeal(QObject *parent = 0);
    virtual~MessageDataDeal();
signals:
    void recvMessageData(MessageData data);
public slots:
    void ParserFrameData(NetworkData data);
private:
    class PrivData;
    PrivData *mData;
};

#endif // MESSAGEDATADEAL_H
