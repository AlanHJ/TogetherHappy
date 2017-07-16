#include "selftabwidget.h"
#include "selftoolbutton.h"
#include "happytabwidget.h"
#include "../../thwidgets/happytreewidget.h"
#include "../../thwidgets/model/itemeditor.h"
#include "../../config/configuredata.h"
#include "../../broadcast/broadcastreceverjson.h"
#include "../../gsoap/call/gsoapcalloperation.h"
#include "../../gsoap/gsoapmessageinstance.h"
#include "../../data/json/broadcastjsondata.h"
#include "../../data/groupinvitedata.h"
#include "../../sql/sql.h"
#include "topwidget.h"
#include "groupitem.h"
#include "personitem.h"
#include "chat.h"
#include "msgbox.h"
#include "selfmenu.h"
#include "creategroup.h"
#include "../notifyui.h"
#include <QPainter>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <TlHelp32.h>
#endif

bool isUpdateExist()
{
    QString name = "HappyUpdate.exe";
    bool ret = false;
#ifdef Q_OS_WIN
    PROCESSENTRY32 p32;
    p32.dwSize = sizeof(p32);
    HANDLE hProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcess == INVALID_HANDLE_VALUE)
    {
        qDebug() << "INVALID_HANDLE_VALUE";
    }
    BOOL b = ::Process32First(hProcess, &p32);
    while (b)
    {
        QString processName = QString::fromWCharArray(p32.szExeFile);
        if (name.compare(processName, Qt::CaseInsensitive) == 0)
        {
            ret = true;
            break;
        }
        b = ::Process32Next(hProcess, &p32);
    }
    CloseHandle(hProcess);
#endif
    return ret;
}


SelfTabWidget::SelfTabWidget(QSystemTrayIcon *tray, QWidget *parent) : QWidget(parent)
{
    timerid = -1;
    selfitem = nullptr;
    icons << ":/res/happy.png";
    this->tray = tray;
    connect(this->tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayActivated(QSystemTrayIcon::ActivationReason)));
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    recever = nullptr;
    init();
}

SelfTabWidget::~SelfTabWidget()
{
    if (recever)
    {
        recever->stop();
    }
}

void SelfTabWidget::clearItems()
{
    QString selfid = myuid;
    PersonItem *selfpi = nullptr;
    auto it = items.begin();
    while (it != items.end())
    {
        QString uid = it.key();
        ItemEditor *ie = it.value();
        PersonItem *pi = dynamic_cast<PersonItem *>(ie);
        if (pi)
        {
            if (uid == selfid)
            {
                selfpi = pi;
            }
            else
            {
                QTreeWidgetItem *item = pi->getItem();
                delete item;
                item = nullptr;
            }
        }
        ++it;
    }
    items.clear();
    if (selfpi)
        items.insert(selfid, selfpi);
}

void SelfTabWidget::addSelfItem()
{
    myuid = config->getUuid();
    bool b = config->IsTrue("show_self");
    if (b)
    {
        // add self
        selfitem = new QTreeWidgetItem;
        personTree->addTopLevelItem(selfitem);
        TopWidget *selftopw = new TopWidget(selfitem, personTree);
        selftopw->setText(QStringLiteral("自己"));
        personTree->setItemWidget(selfitem, 0, selftopw);

        QTreeWidgetItem *citem = new QTreeWidgetItem(selfitem);
        PersonItem *selfw= new PersonItem(recever, citem, this, personTree);
        connect(selfw, &PersonItem::sendNotify, this, [=] (QString res) {
            if (!icons.contains(res))
            {
                icons << res;
            }
            timerid = startTimer(350);
        });
        connect(this, &SelfTabWidget::signalShowChat, selfw, [=] () {
            selfw->showChat();
        });
        connect(this, &SelfTabWidget::setItemMaxMin,
                selfw, &PersonItem::showMaxMin);
        items.insert(myuid, selfw);
        selfw->setUid(myuid);
        selfw->setName(config->getIni("nickname"));
        selfw->setIntroduce(config->getIni("feel"));
        personTree->setItemWidget(citem, 0, selfw);

//        connect(this, &SelfTabWidget::selfStatusChanged, [=] (int stats) {
//            selfw->setStatus((ItemEditor::status)stats);
//        });
//        connect(this, &SelfTabWidget::selfFeelChanged, [=] (QString str) {
//            selfw->setIntroduce(str);
//        });
    }
}

QHash<QString, Chat *> *SelfTabWidget::returnChats()
{
    return &chats;
}

QHash<QString, ItemEditor *> *SelfTabWidget::returnItems()
{
    return &items;
}

void SelfTabWidget::signalPersonDClicked(ItemEditor *ie)
{
    emit personTree->memberDoubleClicked(ie);
}

QStringList SelfTabWidget::packageItems()
{
    QStringList list;
    auto it = items.begin();
    while (it != items.end())
    {
        QString uid = it.key();
        ItemEditor *item = it.value();
        list << item->getName() + ":" + uid;
        ++it;
    }
    return list;
}

void SelfTabWidget::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::DoubleClick:
        {
            // show all hide chat
            stopTimer();
            icons.clear();
            icons << ":/res/happy.png";
            tray->setIcon(QIcon(":/res/happy.png"));
            auto it = items.begin();
            while (it != items.end())
            {
                ItemEditor *itee = it.value();
                if (!itee->isChatVisible())
                {
                    emit signalShowChat();
                    return;
                }
                ++it;
            }
            for (int i = 0; i < groupTree->topLevelItemCount(); i++)
            {
                QTreeWidgetItem *item = groupTree->topLevelItem(i);
                QWidget *w = groupTree->itemWidget(item, 0);
                GroupItem *git = dynamic_cast<GroupItem*>(w);
                if (git && git->isChatVisible() == false)
                {
                    emit signalShowChat();
                    return;
                }
            }
            QWidget *pw = parentWidget();
            if (pw)
            {
                pw->setWindowState(Qt::WindowActive);
                pw->activateWindow();
                pw->show();
                pw->showNormal();
            }
        }
        break;
    }
}

QTreeWidgetItem *SelfTabWidget::createFriendItem(QString uid, QJsonObject json)
{
    QTreeWidgetItem *citem = new QTreeWidgetItem(friendItem);
    PersonItem *selfw= new PersonItem(recever, citem, this, personTree);
    connect(selfw, &PersonItem::sendNotify, this, [=] (QString res) {
        stopTimer();
        if (!icons.contains(res))
        {
            icons << res;
        }
        timerid = startTimer(350);
    });
    connect(this, &SelfTabWidget::signalShowChat, selfw, [=] () {
        selfw->showChat();
    });
    connect(selfw, &PersonItem::ItemClose, this, [=] (QString uid) {
        if (items.contains(uid))
        {
            items.remove(uid);
            emit updateGroupMember(packageItems());
        }
    });
    connect(this, &SelfTabWidget::showMemoName,
            selfw, &PersonItem::showMemoName);
    connect(this, &SelfTabWidget::setItemMaxMin,
            selfw, &PersonItem::showMaxMin);
    items.insert(uid, selfw);
    selfw->setUid(uid);
//    selfw->setName(json.getGsoapip());
    personTree->setItemWidget(citem, 0, selfw);
//    selfw->setJsondata(json);
    emit selfw->handlePaser(json);
    bool flag = friendItem->isExpanded();
    friendItem->setExpanded(!flag);
    friendItem->setExpanded(flag);
    emit updateGroupMember(packageItems());
    return citem;
}

Chat *SelfTabWidget::recvMsgToChat(QString uid,
                                   QString ip,
                                   bool notify)
{
    if (!items.contains(uid))
    {
        // create items
        BroadcastJsonData data;
        data.setUid(uid);
        data.setGsoapip(ip);
        createFriendItem(uid, data.toObject());
    }
    ItemEditor *ie = items.value(uid);
    if (!chats.contains(uid))
    {
        Chat *c = ie->createChat();
        c->hide();
        chats.insert(uid, c);
        if (notify)
            ie->starNotify();
        // add recent
    }
    addRecent(ie);
    Chat *chat = chats.value(uid);
    ie->setChat(chat);
    if (!notify)
    {
        stopTimer();
        icons.clear();
        icons << ":/res/happy.png";
        tray->setIcon(QIcon(":/res/happy.png"));
    }
    return chat;
}

QTreeWidgetItem *SelfTabWidget::createGroup(GroupData gd)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    groupTree->addTopLevelItem(item);
    GroupItem *gitem = new GroupItem(item, this, groupTree);
    connect(gitem, &GroupItem::sendNotify,
            this, [=] (QString res) {
        stopTimer();
        if (!icons.contains(res))
        {
            icons << res;
        }
        timerid = startTimer(350);
    });
    connect(gitem, &GroupItem::leaveGroup,
            this, [=] (GroupData gd) {
        // 离开
        MsgBox::ShowBRbox(cn("%1离开提示").arg(gd.getAttr()),
                          cn("你离开了 %1 %2").
                          arg(gd.getName()).
                          arg(gd.getAttr()));
    });
    connect(this, &SelfTabWidget::signalShowChat, gitem, [=] () {
        gitem->showChat();
    });
    groupTree->setItemWidget(item, 0, gitem);
    gitem->setGroupData(gd);
    gitem->setUid(gd.getUid());
    return item;
}

bool SelfTabWidget::writeGroupDb(GroupData gd)
{
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        MsgBox::ShowMsgBox(cn("创建%1失败").arg(gd.getAttr()),
                           cn("数据库错误\n") + er,
                           cn("确定"));
        return false;
    }
    int rlt = 0;
    QString sql = "SELECT * FROM " +
            gd.objectName() + " WHERE uid = '" + gd.getUid() + "'";
    QList<QObject *> objs = db->executeQuery(sql, GroupData().metaObject(), rlt, er);
    if (rlt != 0)
    {
        MsgBox::ShowMsgBox(cn("创建%1失败").arg(gd.getAttr()),
                           cn("数据库查询失败\n") + er,
                           cn("确定"));
        db->close();
        return false;
    }
    else
    {
        if (objs.isEmpty())
        {
            bool b = db->insert(&gd, er);
            db->close();
            if (!b)
            {
                MsgBox::ShowMsgBox(cn("创建%1失败").arg(gd.getAttr()),
                                   cn("数据库错误\n") + er,
                                   cn("确定"));
                return false;
            }
        }
        else
        {
            // need update?
            QObject *obj = objs.first();
            GroupData *oldgd = qobject_cast<GroupData*>(obj);
            bool b = false;
            if (oldgd)
            {
                if (gd.getUpdatetime() > oldgd->getUpdatetime())
                {
                    b = db->update(&gd, "uid", er);
                    if (!b)
                    {

                        MsgBox::ShowMsgBox(cn("创建%1失败").arg(gd.getAttr()),
                                           cn("数据库错误\n") + er,
                                           cn("确定"));
                    }
                    else
                    {
                        // success
                    }
                }
                else
                {
                    // no need update
                }
            }
            else
            {
                MsgBox::ShowBRbox(cn("提示"),
                                  cn("更新 %1 %2数据信息失败\n"
                                     "错误的转换值!")
                                  .arg(gd.getName())
                                  .arg(gd.getAttr()));
                b = false;
            }
            auto it = objs.begin();
            while (it != objs.end())
            {
                QObject *obj = *it;
                delete obj;
                obj = nullptr;
                ++it;
            }
            oldgd = nullptr;
            db->close();
            return b;
        }
    }
    createGroup(gd);
    return true;
}

void SelfTabWidget::updateCheck()
{
    NotifyUI::Show3sNotify(cn("正在检查，请稍候..."), 1500);
    QString ip;
    QString info;
    bool b = false;
    auto it = items.begin();
    while (it != items.end())
    {
        ItemEditor *ie = it.value();
        if (ie)
        {
            PersonItem *pi = dynamic_cast<PersonItem *>(ie);
            if (pi)
            {
                b = pi->checkVerson(ip, info);
                if (b)
                {
                    updateExec(ip, info);
                    return;
                }
            }
        }
        ++it;
    }
    NotifyUI::Show3sNotify(cn("你现在使用的已经是最新版本的哈啤了"));
}

void SelfTabWidget::sendInvite(GroupData gd, QStringList uids)
{
    GsoapCallOperation *sender = GsoapCallOperation::Sender();
    MessageData msg;
    msg.setUid(myuid);
    GroupInviteData *content = new GroupInviteData;
    content->setGroupData(gd);
    msg.setDatatype(content->datatype());
    msg.setContent(content);
    uids.removeOne(myuid);
    auto it = items.begin();
    while (it != items.end())
    {
        QString uid = it.key();
        ItemEditor *ie = it.value();
        if (uids.contains(uid))
        {
            PersonItem *pi = dynamic_cast<PersonItem *>(ie);
            if (pi)
            {
                QString ip = pi->getGsopip();
                emit sender->signalChatOneInterface(uid,
                                                    ip,
                                                    msg);
            }
        }
        ++it;
    }
}

void SelfTabWidget::stopTimer()
{
    if (timerid != -1)
    {
        killTimer(timerid);
        timerid = -1;
    }
}

void SelfTabWidget::updateMsg(float version, QString versioninfo, QString ip)
{
    if (version <= HAPPY_VERSON)
        return;
    bool accept = config->IsTrue("accept_update");
    if (!accept)
    {
        //reject
        return;
    }
    updateExec(ip, versioninfo);
}

void SelfTabWidget::updateExec(QString ip, QString versioninfo)
{
    config->setIni("accept_update", false);
    bool silence = config->IsTrue("update_silence");
    if (!silence)
    {
        int rlt = MsgBox::ShowMsgBox(cn("升级提示"),
                           cn("检测到高版本的哈啤，是否升级\n")
                           + versioninfo,
                           cn("现在升级"), cn("以后再说"));
        if (rlt != 0)
            return;
    }
    // check update exe
    if (isUpdateExist())
    {
        NotifyUI::Show3sNotify(cn("更新程序已经启动了"));
        return;
    }
    // run update process
    QFile file(GetWorkPath() + "/update/HappyUpdate.exe");
    if (!file.exists())
    {
        MsgBox::ShowMsgBox(cn("升级错误"),
                        cn("没有找到升级程序"),
                        cn("确定"));
        return;
    }
    QString temp = qApp->applicationDirPath() + "/HappyUpdate.exe";
    QFile::remove(temp);
    QString er;
    bool b = CpFile(file.fileName(), temp, er);
    if (!b)
    {
        MsgBox::ShowMsgBox(cn("升级错误"),
                        cn("无法运行升级程序"),
                        cn("确定"));
        return;
    }
    QStringList arguments;
    arguments << QString::fromLocal8Bit(qApp->applicationFilePath().toLocal8Bit());         // 中文存在问题
    arguments << ip;
    arguments << config->getColorIni("color1").name();
    arguments << config->getColorIni("color2").name();
    b = QProcess::startDetached(temp, arguments);
    if (!b)
    {
        MsgBox::ShowMsgBox(cn("升级错误"),
                        cn("升级程序运行失败"),
                        cn("确定"));
        return;
    }
}

void SelfTabWidget::addRecent(ItemEditor *ie)
{
    QString uid = ie->getUid();
    for (int i = 0; i < recentTree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *it = recentTree->topLevelItem(i);
        QWidget *w1 = recentTree->itemWidget(it, 0);
        ItemEditor *ie1 = qobject_cast<ItemEditor *>(w1);
        if (ie1 == nullptr)
        {
            continue;
        }
        if (uid == ie1->getUid())
        {
            delete recentTree->takeTopLevelItem(i);
            break;
        }
    }
    QTreeWidgetItem *citem = new QTreeWidgetItem;
    recentTree->insertTopLevelItem(0, citem);
    ItemEditor *cie = ie->clone();
    cie->addTimeSign();
    recentTree->setItemWidget(citem, 0, cie);
}

void SelfTabWidget::init()
{
    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    SelfToolButton *person = new SelfToolButton(this);
    person->setFixedHeight(34);
    QStringList list1;
    list1 << ":res/ui3/person.png"
          << ":res/ui3/person_hover.png"
          << ":res/ui3/person_press.png";
    person->setImagePaths(list1);
    SelfToolButton *group = new SelfToolButton(this);
    group->setFixedHeight(34);
    QStringList list2;
    list2 << ":res/ui3/group.png"
          << ":res/ui3/group_hover.png"
          << ":res/ui3/group_press.png";
    group->setImagePaths(list2);
    SelfToolButton *recent = new SelfToolButton(this);
    recent->setFixedHeight(34);
    QStringList list3;
    list3 << ":res/ui3/history.png"
          << ":res/ui3/history_hover.png"
          << ":res/ui3/history_press.png";
    recent->setImagePaths(list3);
//    h1->addSpacerItem(new QSpacerItem(12, 34));
    h1->addWidget(person);
    h1->addWidget(group);
    h1->addWidget(recent);

    tab = new HappyTabWidget(this);
    tab->setFixedSize(230, 380);

    // 个人
    personTree = new HappyTreeWidget(this);
    personTree->setHeaderHidden(true);
    // defalut item
    friendItem = new QTreeWidgetItem;
    personTree->addTopLevelItem(friendItem);
    TopWidget *topw = new TopWidget(friendItem, personTree);
    personTree->setItemWidget(friendItem, 0, topw);
//    item->setExpanded(true);
//    for (int i = 0 ; i < 10; i++)
//    {
//        QTreeWidgetItem *citem = new QTreeWidgetItem;
//        item->addChild(citem);
//        personTree->setItemWidget(citem, 0, new ItemEditor);
//    }
    personTree->setIndentation(0);
    personTree->setContentsMargins(0, 0, 0, 0);
    // 群或讨论组
    groupTree = new HappyTreeWidget(this);
    groupTree->setIndentation(0);
    groupTree->setHeaderHidden(true);
    // 最近使用
    recentTree = new HappyTreeWidget(this);
    recentTree->setIndentation(0);
    recentTree->setHeaderHidden(true);
    // add tab
    tab->addTab(personTree, "");
    tab->addTab(groupTree, "");
    tab->addTab(recentTree, "");
//    tab->setFixedSize(230, 450);
    // connect
    person->setSign(0);
    group->setSign(1);
    recent->setSign(2);

    tab->setCurrentIndex(0);
    person->selectButton(tab->currentIndex());
    //add to layout
    v->addLayout(h1, 0);
    v->addWidget(tab, 1);
    setLayout(v);
    setFixedSize(230, 420);
    config = ConfigureData::getInstance();
    // init gsoap sender recever
    GsoapCallOperation::Sender()->startCall();
    GsoapMessageInstance *msgrecever = GsoapMessageInstance::GetInstance();
    connect(msgrecever, &GsoapMessageInstance::signalRecvChat,
            this, [=] (QString uid, QString ip, QString html) {
        // 接收消息
        Chat *chat = recvMsgToChat(uid, ip);
        ItemEditor *ie = items.value(uid, nullptr);
        if (!chat || !ie)
            return;
        chat->recvMsg(html, ie->getName());
    });

    connect(msgrecever, &GsoapMessageInstance::signalCommonStr,
            this, [=] (QString uid, QString ip, int type, QString str) {
        switch (type) {
        case FilePrev:
            recvMsgToChat(uid, ip, false)->execRecvCmd(Chat::fileprev, str);
            break;
        }
    });

    connect(msgrecever, &GsoapMessageInstance::signalShakeChat,
            this, [=] (QString uid, QString ip) {
        // 抖
        recvMsgToChat(uid, ip, false)->execRecvCmd(Chat::shake);
    });

    connect(msgrecever, &GsoapMessageInstance::signalInvite,
            this, [=] (GroupData gd) {
        MsgBox::ShowBRbox(cn("%1邀请提示").arg(gd.getAttr()),
                          cn("%1 邀请你加入 %2 %3").
                          arg(gd.getCreator()).
                          arg(gd.getName()).
                          arg(gd.getAttr()));
        writeGroupDb(gd);
    });

    // create broadcast
    recever = new BroadcastReceverJson();
    recever->start();

    emit recever->createRecever(QHostAddress(HAPPY_DEFULT_BROADCASR_ADDRESS),
                                HAPPY_DEFULT_BROADCASR_PORT);
    connect(recever, &BroadcastReceverJson::joinMulticastGroupFail, this, [=] () {
        MsgBox::ShowMsgBox(cn("严重错误"),
                           cn("无法加入组播"),
                           cn("确定"));
    });
//    return;

    connect(recever, &BroadcastReceverJson::jsonMsg,
            this, [=] (QString uid, QJsonObject json) {
            if (items.contains(uid))
                return;
            if (uid == myuid)
                return;
            // new friend
            BroadcastJsonData data;
            data.parserJson(json);
            // leave
            if (data.getStatus() == 3)
                return;
            createFriendItem(uid, json);
    });

    // double click
    connect(personTree, &HappyTreeWidget::memberDoubleClicked, [=] (ItemEditor *ie) {
        chats.insert(ie->getUid(), ie->createChat());
        addRecent(ie);
        });
    connect(groupTree, &HappyTreeWidget::topmemberDoubleClicked, [=] (ItemEditor *ie) {
        ie->createChat();
        addRecent(ie);

    });
    connect(recentTree, &HappyTreeWidget::topmemberDoubleClicked, [=] (ItemEditor *ie) {
        QString uid = ie->getUid();
        auto it = items.begin();
        while (it != items.end())
        {
            QString id = it.key();
            if (id == uid)
            {
                ItemEditor *personit = it.value();
                if (personit)
                    emit personTree->memberDoubleClicked(personit);
                return;
            }
            ++it;
        }
        for (int i = 0; i < groupTree->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *item = groupTree->topLevelItem(i);
            ItemEditor *groupit = dynamic_cast<ItemEditor*>(groupTree->itemWidget(item, 0));
            if (groupit && uid == groupit->getUid())
            {
                emit groupTree->topmemberDoubleClicked(groupit);
                return;
            }
        }
        NotifyUI::ShowNotify(cn("好友离线或不在分组中"));
    });
    connect(tab, &HappyTabWidget::currentChanged, this, [=] (){
        person->selectButton(tab->currentIndex());
        group->selectButton(tab->currentIndex());
        recent->selectButton(tab->currentIndex());
    });
    connect(person, &SelfToolButton::signalClicked, tab, [=] () {
        tab->setCurrentIndex(0);
    });
    connect(group, &SelfToolButton::signalClicked, tab, [=] () {
        tab->setCurrentIndex(1);
    });
    connect(recent, &SelfToolButton::signalClicked, tab, [=] () {
        tab->setCurrentIndex(2);
//        NotifyUI::ShowNotify(cn("暂未启用"));
    });

    connect(person, &SelfToolButton::signalArrowClicked,
            this, [=] () {
            SelfMenu menu;
            QAction *smallIcon = menu.addAction(cn("显示小图标"));
            smallIcon->setCheckable(true);
            QAction *name1 = menu.addAction(cn("显示昵称"));
            name1->setCheckable(true);
            name1->setData(1);
            QAction *name2 = menu.addAction(cn("显示备注名"));
            name2->setCheckable(true);
            name2->setData(0);
            QPoint p = person->mapToGlobal(person->pos());
            QActionGroup *nameag = new QActionGroup(&menu) ;
            nameag->addAction(name1);
            nameag->addAction(name2);
            int flag = config->getIntIni("show_name");
            if (flag == 1)
            {
                name1->setChecked(true);
            }
            else
            {
                name2->setChecked(true);
            }
            smallIcon->setChecked(config->IsTrue("item_show"));
            connect(nameag, &QActionGroup::triggered, this, [=] (QAction *a) {
                int i = a->data().toInt();
                config->setIni("show_name", i);
            });
            connect(smallIcon, &QAction::toggled,
                    this, [=] (bool checked) {
                emit setItemMaxMin(checked);
                config->setIni("item_show", checked);
                bool flag = friendItem->isExpanded();
                friendItem->setExpanded(!flag);
                friendItem->setExpanded(flag);
                if (selfitem)
                {
                    flag = selfitem->isExpanded();
                    selfitem->setExpanded(!flag);
                    selfitem->setExpanded(flag);
                }
            });
            menu.exec(QPoint(p.x() , p.y() + person->height() + 1));
            emit showMemoName();
    });

    connect(group, &SelfToolButton::signalArrowClicked,
            this, [=] () {
        SelfMenu menu;
        QAction *create = menu.addAction(cn("创建讨论组/群"));
        connect(create, &QAction::triggered, this, [=] () {
            // create group
            CreateGroup *create = new CreateGroup(packageItems());
            connect(this, &SelfTabWidget::updateGroupMember,
                    create, &CreateGroup::updateItems);
            connect(create, &CreateGroup::confrimCreate,
                    this, [=] (GroupData gd) {
                writeGroupDb(gd);
                // send invite
                QtConcurrent::run(this, &SelfTabWidget::sendInvite, gd,
                                  gd.getGroupmember().split(";"));
            });
            create->show();
            create->activateWindow();
        });
        QPoint p = group->mapToGlobal(group->pos());
        menu.exec(QPoint(p.x() - 100,
                         p.y() + person->height() + 1));
    });

    connect(this, &SelfTabWidget::updateGroupMember,
            this, [=] (QStringList list) {
        topw->setText(cn("默认分组 (%1)").arg(list.size() - 1));
    });

    // load data
    addSelfItem();

    // load group data
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        MsgBox::ShowMsgBox(cn("错误"),
                           cn("无法获取群信息\n") + er,
                           cn("确定"));
        return;
    }
    QString sql = "SELECT * FROM " + GroupData().objectName();
    int rlt = 0;
    QList<QObject *> objs = db->executeQuery(sql,
                                             GroupData().metaObject(),
                                             rlt, er);
    db->close();
    auto it = objs.begin();
    while (it != objs.end())
    {
        QObject *obj = *it;
        GroupData *gd = qobject_cast<GroupData *>(obj);
        if (gd)
        {
            // create group item
            createGroup(*gd);
        }
        delete obj;
        obj = nullptr;
        ++it;
    }
}

void SelfTabWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.fillPath(path, QBrush(QColor(230, 230, 230, 50)));
    QColor color(0, 0, 0, 50);
    for(int i = 1; i <= 2; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawLine(QPoint(0, 34 + i), QPoint(230, 34 + i));
    }
    return QWidget::paintEvent(e);
}

void SelfTabWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerid)
    {
        QString icon = icons.first();
        tray->setIcon(QIcon(icon));
        icons.removeFirst();
        icons.push_back(icon);
    }
}
