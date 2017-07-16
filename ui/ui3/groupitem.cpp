#include "groupitem.h"
#include <QtWidgets>
#include "cardui.h"
#include "../../broadcast/broadcastrecevergroup.h"
#include "../../broadcast/broadcastsender.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "groupchat.h"
#include "msgbox.h"
#include "selfmenu.h"
#include "../../tcp/muiltdataopt.h"
#include "../../data/groupinvitedata.h"
#include "selftabwidget.h"
#include "../../sql/sql.h"
#include "../../gsoap/call/gsoapcalloperation.h"
#include "../../gsoap/gsoapmessageinstance.h"
#include <QtConcurrent/QtConcurrent>

GroupItem::GroupItem(QTreeWidgetItem *it, SelfTabWidget *tab, QWidget *parent)
    : ItemEditor(parent)
{
    chat = nullptr;
    recever = nullptr;
    this->tab = tab;
    item = it;
    showMaxMin(false);

    connect(emoji, &EmojiLabel::enterEmoji, this, [=] () {
        CardUI *ui = new CardUI();
        ui->setPicture(emoji->ImagePath());
        ui->setFirstTxt(gdata.getName());
        ui->setSecondTxt(gdata.getCreator());
        ui->setThirdTxt(gdata.getAttr());
        ui->setVersion(HAPPY_VERSON);
        ui->setIntroduceTxt(gdata.getGroupdescribe());
        ui->showAnimation();
        ui->move(mapToGlobal(this->pos()).x() - 360, QCursor::pos().y() - 50);

        connect(emoji, &EmojiLabel::leaveEmoji, ui, [=] () {
//            disconnect(emoji, &EmojiLabel::leaveEmoji, 0, 0);
            ui->hideAnimation();
        });
    });
    sender = BroadcastSender::Sender();
    menu = new SelfMenu(this);
    connect(menu->addAction(cns("�뿪Ⱥ/������")), &QAction::triggered,
            this, [=] () {
        if (!gdata.getDeleteable())
        {
            MsgBox::ShowMsgBox(cn("��ʾ"),
                               cn("�����뿪 %1").
                               arg(gdata.getName()),
                               cn("ȷ��"));
            return;
        }
        int rlt = MsgBox::ShowMsgBox(cn("�뿪ǰ����һ�仰����"),
                                     cn("����Ĵ����뿪 %1 %2ô?").
                                     arg(gdata.getName()).arg(gdata.getAttr()),
                                     cn("����"), cn("������"));
        if (rlt != 0)
            return;
        // ���ݿ����
        QString er;
        SqlDB *db = Sql::instance()->open(er);
        if (!db)
        {
            MsgBox::ShowMsgBox(cn("ʧ��"),
                               cn("���ݿ����\n") + er,
                               cn("ȷ��"));
            return;
        }
        bool b = db->deldata(&gdata, "uid", er);
        db->close();
        if (!b)
        {
            MsgBox::ShowMsgBox(cn("ʧ��"),
                               cn("���ݿ����\n") + er,
                               cn("ȷ��"));
            return;
        }
        // �����뿪���ݰ�
        gdata.setUpdatetime(QDateTime::currentDateTime().toTime_t());
        QStringList uids = gdata.getGroupmember().split(";");
        uids.removeOne(myuid);
        gdata.setGroupmember(uids.join(";"));
        MessageData msg;
        msg.setUid(myuid);
        GroupInviteData *content = new GroupInviteData;
        content->setGroupData(gdata);
        msg.setDatatype(content->datatype());
        msg.setContent(content);
        // �̰߳�ȫ?
        QtConcurrent::run(this, &GroupItem::sendData, msg, 2);
        connect(this, &GroupItem::sendFinished,
                this, [=] () {
            emit leaveGroup(gdata);
            if (chat)
            {
                chat->close();
                chat->deleteLater();
            }
            delete item;
            item = nullptr;
        });
    });

    connect(tab, &SelfTabWidget::updateGroupMember,
            this, [=] (QStringList res) {
        if (chat)
            chat->updateInfo(res);
    });
}

GroupItem::GroupItem()
{
    chat = nullptr;
    recever = nullptr;
    this->tab = nullptr;
    item = nullptr;
}

GroupItem::~GroupItem()
{
    if (recever)
        recever->stop();
}

void GroupItem::setGroupData(GroupData &d)
{
    gdata = d;
    // ui
    setName(gdata.getName());
    other->setText(gdata.getAttr());
    QString str = gdata.getGroupdescribe();
    QFontMetrics fm(cn("΢���ź�"));
    introduce->setText(fm.elidedText(str, Qt::ElideMiddle, 200));
    emoji->setMoiveRes(gdata.getPhoto(),
                       QSize(ItemMaxHeight - 10, ItemMaxHeight - 10));
    // build recever
    recever = new BroadcastReceverGroup();
    recever->start();
    emit recever->createRecever(QHostAddress(gdata.getNetmask()),
                                gdata.getPort());
    connect(recever, &BroadcastReceverGroup::joinMulticastGroupFail, this, [=] () {
        MsgBox::ShowMsgBox(cn("���ش���"),
                           cn("�޷�����%1�鲥\n�޷���ȡ��Ϣ")
                           .arg(gdata.getName()),
                           cn("ȷ��"));
    });
     // create deal broadcast data
    QThread *thread = recever->getThread();
    MuiltDataOpt *opt = new MuiltDataOpt(thread);
    connect(this, &GroupItem::resetQueue,
            opt, &MuiltDataOpt::resetQueue);
    connect(recever, &BroadcastReceverGroup::recvData,
            opt, &MuiltDataOpt::recvMuiltData);
    connect(opt, &MuiltDataOpt::recvMsg,
            this, [=] (QString uid, QString html) {
            if (!chat)
            {
                createChat();
                chat->hide();
                this->starNotify();
            }
            tab->addRecent(this);
            chat->recvMsg(html, uid);
    });
    connect(opt, &MuiltDataOpt::updateGroupData,
            this, [=] (QString uid, GroupData gd) {
        if (uid == myuid)
            return;
        if (gdata.getUid() != gd.getUid())
            return;
        if (gdata.getUpdatetime() >= gd.getUpdatetime())
            return;
        // update sql
        QString er;
        SqlDB *db = Sql::instance()->open(er);
        if (!db)
        {
            MsgBox::ShowBRbox(cn("��ʾ"),
                              cn("���� %1 %2������Ϣʧ��\n")
                              .arg(gdata.getName())
                              .arg(gdata.getAttr()) + er);
            return;
        }
        bool b = db->update(&gd, "uid", er);
        db->close();
        if (b)
        {
            // update chat
            if (chat)
            {
                chat->setGroupData(gd);
                chat->showStatus(gdata.getGroupmember(),
                                 gd.getGroupmember());
                chat->updateInfo(tab->packageItems());
            }
            gdata = gd;
        }
        else
        {
            MsgBox::ShowBRbox(cn("��ʾ"),
                              cn("���� %1 %2������Ϣʧ��\n")
                              .arg(gdata.getName())
                              .arg(gdata.getAttr()) + er);
        }
    });
}

Chat *GroupItem::createChat()
{
    if (chat)
    {
        chat->showNormal();
        chat->activateWindow();
        return chat;
    }
    chat = new GroupChat;
    chat->setGroupData(gdata);
    chat->updateInfo(tab->packageItems());
    chat->setChatTitle(gdata.getName(), emoji->ImagePath());
    connect(chat, &GroupChat::chatClose, this, [=] () {
        disconnect(chat, &GroupChat::chatClose, this, 0);
        disconnect(chat, &GroupChat::itemClicked, this, 0);
        disconnect(chat, &GroupChat::sendInvite, this, 0);
        chat = nullptr;
        // reset queue
        emit resetQueue();
    });
    connect(chat, &GroupChat::sendInvite,
            this, &GroupItem::sendInvite);
    connect(chat, &GroupChat::itemClicked,
            this, [=] (QString uid) {
        if (tab->returnItems()->contains(uid))
        {
            ItemEditor *ie = tab->returnItems()->value(uid);
            tab->signalPersonDClicked(ie);
        }
        else
        {
            MsgBox::ShowMsgBox(cn("��ʾ"),
                               cn("�ú��Ѳ�������"),
                               cn("ȷ��"));
        }
    });
    chat->show();
    chat->activateWindow();
    return chat;
}

bool GroupItem::isChatVisible()
{
    if (!chat)
        return true;
    return chat->isVisible();
}


void GroupItem::showChat()
{
    if (chat && chat->isHidden())
    {
        chat->show();
        chat->activateWindow();
    }
}


bool GroupItem::setChat(Chat *chat)
{
    Q_UNUSED(chat)
    return true;
}

void GroupItem::sendInvite(GroupData gd, QStringList uids)
{
    QtConcurrent::run(tab, &SelfTabWidget::sendInvite, gd,
                      uids);
    if (tab->writeGroupDb(gd))
    {
        gdata = gd;
        chat->setGroupData(gdata);
        chat->updateInfo(tab->packageItems());
    }
}


void GroupItem::contextMenuEvent(QContextMenuEvent *e)
{
    menu->exec(QCursor::pos());
    e->accept();
}

void GroupItem::sendData(MessageData msg, uint times)
{
    NetworkData data;
    msg.toStream(data);
    char *d = data.data();
    int len = data.length();
    for (uint i = 0; i < times; i++)
    {
        // ÿ����෢��4096����
        int pos = 0;
        while (pos < len)
        {
            int sendsize = 8192;
            int offset = len - pos;
            if (offset <= 8192)
            {
                sendsize = offset;
            }
            NetworkData senddata(d + pos, sendsize);
            emit sender->signalSendData(senddata,
                                        QHostAddress(gdata.getNetmask())
                                        , gdata.getPort(), gdata.getUid());
            pos += sendsize;
        }
        thread()->msleep(3);
    }
    emit sendFinished();
}


ItemEditor *GroupItem::clone()
{
    GroupItem *it = new GroupItem();
    it->setItemTyle(cloneBody);
    it->setUid(uid);
    it->setName(name->text());
    it->emoji->setMoiveRes(emoji->ImagePath(),
                           QSize(emojiSize - 10, emojiSize - 10));
    it->introduce->setText(introduce->text());
    it->showMaxMin();
    return it;
}
