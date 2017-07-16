#ifndef GROUPINFO_H
#define GROUPINFO_H

#include <QWidget>
#include <QListWidget>
#include "../../data/db/groupdata.h"

class QLabel;
class EmojiLabel;

class MemberItem : public QListWidgetItem
{
public:
    MemberItem(QListWidget *view = 0, int type = Type)
        : QListWidgetItem(view, type)
    {

    }
    QString getuid()
    {
        return uid;
    }
    void setuid(QString id)
    {
        uid = id;
    }

private:
    QString uid;
};

class GroupList : public QListWidget
{
    Q_OBJECT
public:
    GroupList(QWidget *parent = 0);
    bool getNameById(QString id, QString &name);
signals:
    void doubleClicked(QString uid);

};

class GroupInfo : public QWidget
{
    Q_OBJECT
public:
    explicit GroupInfo(QWidget *parent = 0);
    ~GroupInfo();
    bool contains(QString uid, QString &name);
    QHash<QString , QString> getUidName();
signals:
    void itemClicked(QString uid);
public slots:
    void setResource(GroupData gd);
    void updateList(QStringList res);

private:
    EmojiLabel *image;
    GroupList *list;
    QLabel *groupName;
    QLabel *groupClass;
    QLabel *groupBuilder;
    QLabel *groupDescrible;
    QString member;

    QHash<QString, QString> uidMatchName;
};

#endif // GROUPINFO_H
