#ifndef SELFTABWIDGET_H
#define SELFTABWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QSystemTrayIcon>
#include "../../data/db/groupdata.h"
class Chat;
class ItemEditor;
class ConfigureData;
class QTreeWidgetItem;
class QSystemTrayIcon;
class HappyTabWidget;
class HappyTreeWidget;
class BroadcastReceverJson;
class SelfTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelfTabWidget(QSystemTrayIcon *tray,
                           QWidget *parent = 0);
    ~SelfTabWidget();

    void clearItems();
    void addSelfItem();
    QHash<QString, Chat*> *returnChats();
    QHash<QString, ItemEditor*> *returnItems();
    void signalPersonDClicked(ItemEditor *ie);
    QStringList packageItems();
public slots:
    void sendInvite(GroupData gd, QStringList uids);
    bool writeGroupDb(GroupData gd);
    void updateCheck();
    void updateMsg(float version, QString versioninfo, QString ip);
    void addRecent(ItemEditor *ie);
signals:
    void setItemMaxMin(bool min);
    void selfStatusChanged(int status);
    void selfFeelChanged(QString str);
    void signalShowChat();
    void showMemoName();
    void updateGroupMember(QStringList list);
private slots:
    void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    QTreeWidgetItem *createFriendItem(QString uid, QJsonObject json);
    Chat *recvMsgToChat(QString uid, QString ip, bool notify = true);
    QTreeWidgetItem *createGroup(GroupData gd);
    void stopTimer();
    void updateExec(QString ip, QString versioninfo);
private:
    void init();

    ConfigureData *config;
    HappyTabWidget *tab;
    HappyTreeWidget *personTree;
    QTreeWidgetItem *friendItem;
    QTreeWidgetItem *selfitem;
    HappyTreeWidget *groupTree;
    HappyTreeWidget *recentTree;

    QHash<QString, ItemEditor*> items;
    QHash<QString, Chat*> chats;

    BroadcastReceverJson *recever;

    QSystemTrayIcon *tray;

    int timerid;

    QStringList icons;

    QString myuid;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *e);
};

#endif // SELFTABWIDGET_H
