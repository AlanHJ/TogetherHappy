#ifndef GSOAPCALLOPERATION_H
#define GSOAPCALLOPERATION_H

#include <QObject>
#include "../../data/networkdata.h"
#include "../../data/messagedata.h"

class GsoapCallOperation : public QObject
{
    Q_OBJECT
public:
    explicit GsoapCallOperation();
    ~GsoapCallOperation();
    static GsoapCallOperation *Sender();
    static void Destruct();
    void setMyUid(QString id);
    void startCall();
    void stopCall();
signals:
    // 点对点聊天
    void signalChatOneInterface(QString desuid, QString desip, MessageData msg);
    // 群体聊天
    void signalChatGroupInerface(QStringList key, QString gkey, MessageData msg);
    // 发送震动（置顶）提醒
    void signalChatShakeInterface(QString desuid, QString desip);
    // 发送文件请求
    void signalSendFileExec(QString key, QString content);
    // 发送文件接收
    void signalSendFileAccept(QString key, QString content);
    // 发送哈啤文件
    void signalUpdateHappy();
    // 更新文件信息
    void signalUpdateInfo(QString info);
    // 邀请加入群
    void signalInviteGroup(QStringList lst, QString buider,
                           QString title, QString gkey, QString members);
    void signalGroupRespond(QStringList keys, QString key, QString gkey, QString content);

    // 对外抛出信号

    // 发送消息失败
    void signalChatMsgSendFail(QString uid, QString error);
    // Gsoap执行失败
    void signalGsoapFail(QString key, QString error);
    // 群消息信息
    void signalGroupSendResult(QString key, QString error);

    // signal resource
    void signalGetResource(QString srcuid, QString desip, QString type, QString res);

    // send uid recource
    void signalSetResource(QString desuid, QString srcip, QString type, QString res);
private slots:
    // 点对点聊天
    void slotChatOneInterface(QString desuid, QString desip, MessageData msg);
    // 群体聊天
    void slotChatGroupInerface(QStringList keys, QString gkey, MessageData msg);
    // 发送震动（置顶）提醒
    void slotChatShakeInterface(QString desuid, QString desip);
    // 发送文件请求
    void slotSendFileExec(QString key, QString content);
    // 发送文件接收
    void slotSendFileAccept(QString key, QString content);
    // 发送哈啤文件
    void slotUpdateHappy();
    // 邀请加入群
    void slotInviteGroup(QStringList lst, QString buider,
                           QString title, QString gkey, QString members);
    void slotGroupRespond(QStringList keys, QString key, QString gkey, QString content);

    void slotGsoapError(QString key, int result, std::wstring str);

    void slotGetResource(QString srcuid, QString desip, QString type, QString res);
    void slotSetResource(QString desuid, QString srcip, QString type, QString res);
private:
    class PrivData;
    PrivData *mData;

    static GsoapCallOperation *soapsender;
};

#endif // GSOAPCALLOPERATION_H
