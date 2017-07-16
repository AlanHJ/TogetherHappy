#include "groupinfo.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../sql/sql.h"
#include "../../util/thmethod.h"
#include <QtWidgets>

GroupInfo::GroupInfo(QWidget *parent) : QWidget(parent)
{
    // 创建界面
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(5, 5, 8, 5);
    v->setSpacing(0);

    image = new EmojiLabel(this);
    image->setMovieable(true);
    groupName = new QLabel(cn("群名称"), this);
    groupName->setAlignment(Qt::AlignCenter);
    groupClass = new QLabel(cn("群属性"), this);
    groupClass->setAlignment(Qt::AlignCenter);
    groupBuilder = new QLabel(cn("创建者"), this);
    groupBuilder->setAlignment(Qt::AlignCenter);
    groupDescrible = new QLabel(cn("描述"), this);
    groupDescrible->setAlignment(Qt::AlignCenter);
    groupDescrible->setWordWrap(true);

    list = new GroupList(this);
    list->setFixedWidth(150);

    v->addWidget(image);
    v->addSpacing(4);
    v->addWidget(groupName, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(groupClass, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(groupBuilder, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(groupDescrible, 0, Qt::AlignHCenter);
    v->addSpacing(25);
    v->addWidget(list, 1, Qt::AlignHCenter);
    v->addSpacing(10);

    QString css = cn("font-family:微软雅黑;font:12px;color:white;");
    groupName->setStyleSheet(css);
    groupClass->setStyleSheet(css);
    groupBuilder->setStyleSheet(css);
    groupDescrible->setStyleSheet(css);
    QFile file;
    file.setFileName(":res/css/list2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        list->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    connect(list, &GroupList::doubleClicked,
            this, &GroupInfo::itemClicked);
}

GroupInfo::~GroupInfo()
{

}

bool GroupInfo::contains(QString uid, QString &name)
{
    return list->getNameById(uid, name);
}

QHash<QString, QString> GroupInfo::getUidName()
{
    return uidMatchName;
}

void GroupInfo::setResource(GroupData gd)
{
    QString res = gd.getPhoto();
    QImage ig(res);
    image->setMoiveRes(res, ig.rect().contains(QRect(0, 0, 150, 220))
                       ? QSize(150, 220) : ig.size());
    groupName->setText(cn("名称:") + gd.getName());
    groupClass->setText(cn("属性:") + gd.getAttr());
    groupBuilder->setText(cn("创建者:") + gd.getCreator());
    groupDescrible->setText(cn("描述:") + gd.getGroupdescribe());
    member = gd.getGroupmember();
}

void GroupInfo::updateList(QStringList res)
{
    list->clear();
    uidMatchName.clear();
    QHash<QString, QString> hash;
    for (QString str : res)
    {
        QStringList list = str.split(":");
        if (list.size() == 2)
        {
            hash.insert(list.last(), list.first());
        }
    }
    if (member.isEmpty())
    {
        auto it = hash.begin();
        while (it != hash.end())
        {
            QString id = it.key();
            QString name = it.value();
            MemberItem *item = new MemberItem(list);
            item->setTextAlignment(Qt::AlignLeft);
            item->setText(name);
            item->setuid(id);
            uidMatchName.insert(id, name);
            ++it;
        }
    }
    else
    {
        QStringList memberlist = member.split(";");
        auto it = hash.begin();
        while (it != hash.end())
        {
            QString id = it.key();
            QString name = it.value();
            if (memberlist.contains(id))
            {
                MemberItem *item = new MemberItem(list);
                item->setTextAlignment(Qt::AlignLeft);
                item->setText(name);
                item->setuid(id);
                uidMatchName.insert(id, name);
            }
            ++it;
        }
    }
}



GroupList::GroupList(QWidget *parent) : QListWidget(parent)
{
    connect(this, &QListWidget::itemDoubleClicked,
            this, [=] (QListWidgetItem *item) {
        MemberItem *it = dynamic_cast<MemberItem *>(item);
        if (it)
            emit doubleClicked(it->getuid());
    });
}

bool GroupList::getNameById(QString id, QString &name)
{
    bool b = false;
    for (int i = 0; i < count(); i++)
    {
        QListWidgetItem *it = item(i);
        MemberItem *mit = dynamic_cast<MemberItem*>(it);
        if (mit && id == mit->getuid())
        {
            name = mit->text();
            b = true;
            break;
        }
    }
    return b;
}
