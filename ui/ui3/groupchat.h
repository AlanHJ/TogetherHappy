#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include "chat.h"
#include "../../data/db/groupdata.h"
#include <QHostAddress>
#include "../../data/messagedata.h"

class GroupInfo;
class BroadcastSender;
class GroupChat : public Chat
{
    Q_OBJECT
public:
    GroupChat(QWidget *w = 0);
    ~GroupChat();
    void setGroupData(GroupData &d);
public slots:
    void updateInfo(QStringList res);

    // Chat interface
public:
    virtual void init();
    void sendMsg();
    void recvMsg(QString html, QString uid);
    void showStatus(QString oldmember, QString newmember);
    void execRecvCmd(RecvCmd cmd, QVariant var = QVariant());
private:
    GroupInfo *info;
    GroupData gd;
    BroadcastSender *sender;
    QString myuid;
    QStringList items;

    int timerid;

signals:
    void sendInvite(GroupData gd, QStringList uids);
    void sendDataMore();
    void itemClicked(QString uid);
    void updateItems(QStringList lst);
private slots:
    void sendMsgRun(MessageData msg, QHostAddress addr,
                    quint16 port, QString uid);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);
};

#endif // GROUPCHAT_H
