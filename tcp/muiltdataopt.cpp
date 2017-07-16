#include "muiltdataopt.h"
#include "tcpdatadeal.h"
#include "messagedatadeal.h"
#include "../data/messagerichtext.h"
#include "../data/groupinvitedata.h"
#include <QThread>

MuiltDataOpt::MuiltDataOpt(QThread *thread, QObject *parent)
    : QObject(parent)
{
    msg = new MessageDataDeal(this);
    this->moveToThread(thread);
    msg->moveToThread(thread);
    connect(msg, &MessageDataDeal::recvMessageData,
            this, [=] (MessageData data) {
        if (data.datatype() == RichText)
        {
            MessageRichText *rich = dynamic_cast<MessageRichText*>(data.content());
            if (rich)
                emit recvMsg(data.getuid(), rich->messageHtml());
        }
        else if (data.datatype() == Invite)
        {
            // update info
            GroupInviteData *invite = dynamic_cast<GroupInviteData*>(data.content());
            if (invite)
                emit updateGroupData(data.getuid(), invite->groupData());
        }
        else
        {
            return;
        }
    });
    connect(thread, &QThread::finished, this, &MuiltDataOpt::deleteLater);
}

MuiltDataOpt::~MuiltDataOpt()
{
    tcps.clear();
}

void MuiltDataOpt::recvMuiltData(QString key, NetworkData data)
{
    if (!tcps.contains(key))
    {
        TcpDataDeal *tcp = new TcpDataDeal(this);
        connect(tcp, &TcpDataDeal::fullFrameData,
                msg, &MessageDataDeal::ParserFrameData);
        tcps.insert(key, tcp);
    }
    TcpDataDeal *tcp = tcps.value(key, nullptr);
    if (!tcp)
        return;
    tcp->slotPosData(data);
}

void MuiltDataOpt::resetQueue()
{
    auto it = tcps.begin();
    while (it != tcps.end())
    {
        TcpDataDeal *tcp = it.value();
        if (tcp)
        {
            tcp->deleteLater();
        }
        ++it;
    }
    tcps.clear();
}

