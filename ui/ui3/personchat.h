#ifndef PERSONCHAT_H
#define PERSONCHAT_H

#include "chat.h"

class FriendInfo;
class FileTransmit;
class BroadcastJsonData;
class GsoapCallOperation;
class GsoapMessageInstance;
class BroadcastReceverJson;
class PersonChat : public Chat
{
    Q_OBJECT
public:
    PersonChat(QWidget *w = 0);
    ~PersonChat();
    void setJsonData(BroadcastJsonData *data);
    void setRecever(BroadcastReceverJson *recever);
    // Chat interface
public:
    virtual void init();
    void setChatTitle(QString content, QString ico);

    // Chat interface
public slots:
    void sendMsg();
    void recvMsg(QString html, QString src);
    void execRecvCmd(RecvCmd cmd, QVariant var = QVariant());
    void setRejectShake(bool b);
    void setShakeEnable(bool b);
signals:
    void starkShake();
    void shakeRejectStatusChanged(bool b);

private:
    QPushButton *transmitBtn;
    BroadcastJsonData *jsondata;
    GsoapCallOperation *sender;
    FileTransmit *transmit;
    FriendInfo *info;
    bool shakeabled;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *e);
};

#endif // PERSONCHAT_H
