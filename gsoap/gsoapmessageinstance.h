#ifndef GSOAPMESSAGEINSTANCE_H
#define GSOAPMESSAGEINSTANCE_H

#include <QObject>
#include "../data/messagedata.h"
#include "../data/db/groupdata.h"

class GsoapMessageInstance : public QObject
{
    Q_OBJECT
public:
    explicit GsoapMessageInstance();
    virtual~GsoapMessageInstance();
    static GsoapMessageInstance *GetInstance();
    static void DesInstance();
    int UpdateFile(QString v, std::wstring *str);
signals:
    void signalRecvChat(QString srcuid, QString srcip, QString html);
    void signalGetResource(QString srcuid, QString type, QString name);
    void signalShakeChat(QString srcuid, QString srcip);
    void signalOtherCmd(quint16 type, MessageData msg);
    void signalInvite(GroupData gd);
    void signalCommonStr(QString srcuid, QString srcip, int type, QString str);

    void signalRecvGroup(QString gkey, QString key, QString html);

    void signalFileExec(QString key, QString content);
    void signalFileAccept(QString key, QString content);



    void signalInviteGroup(QString b, QString t, QString k, QString m);
    void signalGroupRespond(QString k, QString gkey, QString c);

    void signalUpdateInfo(QString info);

    void signalUpdateToMe(QString key, QString version);

    void signalWhoUpdate(QString key, QString info);
public slots:
private:
    class PrivData;
    PrivData *mData;
};

#endif // GSOAPMESSAGEINSTANCE_H
