#ifndef PERSONITEM_H
#define PERSONITEM_H

#include "../../thwidgets/model/itemeditor.h"

class SelfMenu;
class SelfTabWidget;
class PersonItem : public ItemEditor
{
    Q_OBJECT
public:
    PersonItem(BroadcastRecever *recever,
               QTreeWidgetItem *it,
               SelfTabWidget *t,
               QWidget *parent = 0);
    PersonItem();
    ~PersonItem();

    QTreeWidgetItem *getItem();
    enum status{online = 0, busy, leave};
    void setStatus(status s);
    void setIntroduce(QString str);
    void setJsondata(BroadcastJsonData data);
    QString getGsopip();
    QString getMemoname();
    QString getNickname();
    bool checkVerson(QString &ip, QString &info);
signals:
    void ItemClose(QString uid);
    void cloneCreateChat(PersonChat *chat);
    void handlePaser(QJsonObject json);

private:
    PersonChat *chat;

    SelfTabWidget *tab;
    BroadcastJsonData *jsondata;
    BroadcastReceverJson *recever;

    QString image;

    bool shakeabled;
    bool rejectShake;

    SelfMenu *menu;

    QString memoname;
    int nameFlag;

    // ItemEditor interface
public:
    virtual Chat *createChat();
    virtual bool isChatVisible();
    virtual void showChat();
    virtual bool setChat(Chat *ct);
    void setUid(QString id);
    ItemEditor *clone();

public slots:
    void showMemoName();

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *e);

};

#endif // PERSONITEM_H
