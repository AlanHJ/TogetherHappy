#ifndef GROUPITEM_H
#define GROUPITEM_H

#include "../../data/db/groupdata.h"
#include "../../thwidgets/model/itemeditor.h"
#include "../../data/messagedata.h"

class SelfMenu;
class BroadcastSender;
class SelfTabWidget;
class BroadcastReceverGroup;
class GroupChat;

class GroupItem : public ItemEditor
{
    Q_OBJECT
public:
    GroupItem(QTreeWidgetItem *it,
              SelfTabWidget *tab,
              QWidget *parent = 0);
    GroupItem();
    ~GroupItem();
    void setGroupData(GroupData &d);
signals:
    void resetQueue();
    void leaveGroup(GroupData d);
    void sendFinished();
    // ItemEditor interface
public:
    virtual Chat *createChat();
    virtual bool isChatVisible();
    virtual void showChat();
    virtual bool setChat(Chat *chat);
    ItemEditor *clone();

public slots:
    void sendInvite(GroupData gd, QStringList uids);

private:
    BroadcastSender *sender;
    BroadcastReceverGroup *recever;
    SelfTabWidget *tab;
    GroupData gdata;
    GroupChat *chat;
    SelfMenu *menu;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *e);

private:
    void sendData(MessageData msg, uint times);

};

#endif // GROUPITEM_H
