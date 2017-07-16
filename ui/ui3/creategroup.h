#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QListWidget>
#include <QCheckBox>
#include <QFile>
#include <QTextCodec>
#include "../thwidgetbase.h"
#include "../../data/db/groupdata.h"

class CheckItem : public QListWidgetItem
{
public:
    CheckItem(QListWidget *view = 0, int type = Type)
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

class CheckList : public QListWidget
{
    Q_OBJECT
public:
    CheckList(QWidget *parent = 0)
        : QListWidget(parent)
    {

    }
    QString checkedids()
    {
        QStringList list;
        for (int i = 0; i < count(); i++)
        {
            QListWidgetItem *it = item(i);
            CheckItem *cit = dynamic_cast<CheckItem*>(it);
            QWidget *w = itemWidget(it);
            QCheckBox *box = dynamic_cast<QCheckBox*>(w);
            if (cit && box)
            {
                if (box->isChecked())
                    list << cit->getuid();
            }
        }
        return list.join(";");
    }

    void setmyuid(QString id)
    {
        myuid = id;
    }

public slots:
    void addCheckItems(QStringList items)
    {
        clear();
        QString css;
        QFile file;
        file.setFileName(":res/css/checkbox.css");
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray ba = file.readAll();
            css = QTextCodec::codecForLocale()->toUnicode(ba);
        }
        file.close();
        for (QString str : items)
        {
            QStringList item = str.split(":");
            if (item.size() == 2)
            {
                QString name = item.first();
                QString uid = item.last();
                QCheckBox *ck = new QCheckBox(name,
                                             this);
                if (uid == myuid)
                {
                    ck->setChecked(true);
                    ck->setEnabled(false);
                }
                ck->setStyleSheet(css);
                CheckItem * cit = new CheckItem(this);
                cit->setuid(uid);
                setItemWidget(cit, ck);
            }
        }
    }

private:
    QString myuid;
};

class InviteWidget : public THWidgetBase
{
    Q_OBJECT
public:
    explicit InviteWidget(QStringList items,
                          QString gd, QWidget *parent = 0);
    ~InviteWidget();
signals:
    void confrimInvite(QStringList members);
    void updateItems(QStringList items);
private:
    QString gdm;
    QStringList members;
};

class CreateGroup : public THWidgetBase
{
    Q_OBJECT
public:
    explicit CreateGroup(QStringList items, QWidget *parent = 0);
    ~CreateGroup();

signals:
    void confrimCreate(GroupData gd);
    void updateItems(QStringList items);
private:
    QString createNetmask();
};

#endif // CREATEGROUP_H
