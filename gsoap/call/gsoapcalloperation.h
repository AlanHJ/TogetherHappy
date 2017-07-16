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
    // ��Ե�����
    void signalChatOneInterface(QString desuid, QString desip, MessageData msg);
    // Ⱥ������
    void signalChatGroupInerface(QStringList key, QString gkey, MessageData msg);
    // �����𶯣��ö�������
    void signalChatShakeInterface(QString desuid, QString desip);
    // �����ļ�����
    void signalSendFileExec(QString key, QString content);
    // �����ļ�����
    void signalSendFileAccept(QString key, QString content);
    // ���͹�ơ�ļ�
    void signalUpdateHappy();
    // �����ļ���Ϣ
    void signalUpdateInfo(QString info);
    // �������Ⱥ
    void signalInviteGroup(QStringList lst, QString buider,
                           QString title, QString gkey, QString members);
    void signalGroupRespond(QStringList keys, QString key, QString gkey, QString content);

    // �����׳��ź�

    // ������Ϣʧ��
    void signalChatMsgSendFail(QString uid, QString error);
    // Gsoapִ��ʧ��
    void signalGsoapFail(QString key, QString error);
    // Ⱥ��Ϣ��Ϣ
    void signalGroupSendResult(QString key, QString error);

    // signal resource
    void signalGetResource(QString srcuid, QString desip, QString type, QString res);

    // send uid recource
    void signalSetResource(QString desuid, QString srcip, QString type, QString res);
private slots:
    // ��Ե�����
    void slotChatOneInterface(QString desuid, QString desip, MessageData msg);
    // Ⱥ������
    void slotChatGroupInerface(QStringList keys, QString gkey, MessageData msg);
    // �����𶯣��ö�������
    void slotChatShakeInterface(QString desuid, QString desip);
    // �����ļ�����
    void slotSendFileExec(QString key, QString content);
    // �����ļ�����
    void slotSendFileAccept(QString key, QString content);
    // ���͹�ơ�ļ�
    void slotUpdateHappy();
    // �������Ⱥ
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
