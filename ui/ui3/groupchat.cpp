#include "groupchat.h"
#include <QtWidgets>
#include "../../broadcast/broadcastsender.h"
#include "groupinfo.h"
#include "../../config/configuredata.h"
#include "../../thwidgets/happytextedit.h"
#include "../../data/messagerichtext.h"
#include "../../data/groupinvitedata.h"
#include "msgbox.h"
#include "creategroup.h"
#include <QtConcurrent/QtConcurrent>

GroupChat::GroupChat(QWidget *w) : Chat(false, w)
{
    init();
    sender = BroadcastSender::Sender();
    connect(sender, &BroadcastSender::sendFail,
            this, [=] (QString uid) {
        if (uid != gd.getUid())
            return;
        inserHtml(cn("����ʧ�ܣ�������"), cn("��ơ��ʾ"), Qt::red);
    });
    connect(this, &GroupChat::sendDataMore,
            this, [=] () {
        inserHtml(cn("���͵Ķ���Ҳ̫����,��������"), cn("��ơ��ʾ"), Qt::yellow);
    });
    myuid = ConfigureData::getInstance()->getUuid();
    timerid = startTimer(30000);
}

GroupChat::~GroupChat()
{

}

void GroupChat::setGroupData(GroupData &d)
{
    gd = d;
    info->setResource(gd);
}

void GroupChat::updateInfo(QStringList res)
{
//    info->updateList(res);
    items = res;
    emit updateItems(res);
}

void GroupChat::init()
{
    QPushButton *infoBtn = new QPushButton(this);
    infoBtn->setToolTip(QStringLiteral("Ⱥ��Ϣ"));
    infoBtn->setFixedSize(35, 30);
    infoBtn->setCheckable(true);

    stackBtnGroup->addButton(infoBtn, 0);
    topBar->addWidget(infoBtn);

    info = new GroupInfo(stack);
    connect(info, &GroupInfo::itemClicked,
            this, &GroupChat::itemClicked);
    info->setMouseTracking(true);
    stack->addWidget(info);

    infoBtn->setChecked(true);

    QPushButton *inviteBtn = new QPushButton(this);
    inviteBtn->setToolTip(cn("�������"));
    inviteBtn->setFixedSize(20, 20);
    centerBar->insertWidget(hidea, inviteBtn);

    QFile file;
    file.setFileName(":res/css/button.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString str = file.readAll();
        infoBtn->setStyleSheet(str.arg("group_info").arg("group_info").arg("group_info"));
        inviteBtn->setStyleSheet(str.arg("invite").arg("invite").arg("invite"));
    }
    file.close();

    connect(this, &GroupChat::updateItems,
            info, &GroupInfo::updateList);

    connect(inviteBtn, &QPushButton::clicked,
            this, [=] () {
        // invite check
        QString members = gd.getGroupmember();
        if (members.isEmpty())
        {
            MsgBox::ShowMsgBox(cn("��ʾ"),
                               cn("��%1�޷���������").arg(gd.getAttr()),
                               cn("ȷ��"));
            return;
        }
        InviteWidget *invite = new InviteWidget(items,
                                                gd.getGroupmember());
        connect(invite, &InviteWidget::confrimInvite,
                this, [=] (QStringList lst) {
            QStringList names;
            QStringList templst = gd.getGroupmember().split(";");
            for (QString str : lst)
            {
                QString name;
                if (info->contains(str, name))
                {
                    if (str != myuid)
                    {
                        names << name;
                    }
                }
                else
                {
                    templst.append(str);
                }
            }
            if (!names.isEmpty())
            {
                int rlt = MsgBox::ShowMsgBox(cn("��ʾ"),
                                             cn("�������ò�ư������Ѿ����ڵĳ�Ա\n%1\n"
                                                "(����Ŀǰ��������������, ��Ȼ�Ⲣ��һ��׼ȷ)\n"
                                                "��ȷ����Ȼ��������ô").arg(names.join(",")),
                                             cn("ȷ��"), cn("ȡ��"));
                if (rlt != 0)
                {
                    return;
                }
            }
            gd.setGroupmember(templst.join(";"));
            gd.setUpdatetime(QDateTime::currentDateTime().toTime_t());
            info->setResource(gd);
            emit sendInvite(gd, lst);
        });
        invite->show();
        invite->activateWindow();
        QPoint p(this->pos().x(), inviteBtn->mapToGlobal(inviteBtn->pos()).y());
        invite->move(p.x() - 25, p.y() - invite->height());
    });

}


void GroupChat::sendMsg()
{
    QString html = edit->toHtml();
    MessageData msg;
    MessageRichText *content = new MessageRichText();
    content->setMessageHtml(html);
    msg.setUid(ConfigureData::getInstance()->getUuid());
    msg.setDatatype(content->datatype());
    msg.setContent(content);
    QtConcurrent::run(this, &GroupChat::sendMsgRun,
                      msg,QHostAddress(gd.getNetmask()),
                      gd.getPort(), gd.getUid());
//    emit sender->signalMsgData(msg,
//                                QHostAddress(gd.getNetmask()),
//                                gd.getPort(), gd.getUid());
}

void GroupChat::recvMsg(QString html, QString uid)
{
    QString name;
    if (!info->contains(uid, name))
        return;
    if (uid == myuid)
    {
        inserHtml(html, cn("��"));
        return;
    }
    inserHtml(html , name, Qt::darkBlue);
    qApp->alert(this, 0);
    isAlert = true;
    lastRecvTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void GroupChat::showStatus(QString oldmember, QString newmember)
{
    QHash<QString, QString> hash = info->getUidName();
    QStringList oldlist = oldmember.split(";");
    QStringList newlist = newmember.split(";");
    // ����/����
    for (QString member : newlist)
    {
        if (!oldlist.contains(member))
        {
            QString name = cn("(�ó�ԱĿǰ������, ���Բ�֪����ʲô)");
            if (hash.contains(member))
            {
                name = hash.value(member);
            }
            inserHtml(cn("���һ�ӭ %1 �����˱�%2")
                      .arg(name).arg(gd.getAttr()),
                      cn("��ơ��ʾ"), Qt::yellow);
            qApp->alert(this, 0);
        }
    }

    for (QString member : oldlist)
    {
        if (!newlist.contains(member))
        {
            QString name = cn("(�ó�ԱĿǰ������, ���Բ�֪����ʲô)");
            if (hash.contains(member))
            {
                name = hash.value(member);
            }
            inserHtml(cn("���ź� %1 �뿪�˱�%2")
                      .arg(name).arg(gd.getAttr()),
                      cn("��ơ��ʾ"), Qt::yellow);
            qApp->alert(this, 0);
        }
    }
}


void GroupChat::execRecvCmd(RecvCmd cmd, QVariant var)
{
    Q_UNUSED(cmd)
    Q_UNUSED(var)
}

void GroupChat::sendMsgRun(MessageData msg, QHostAddress addr, quint16 port, QString uid)
{
    NetworkData data;
    msg.toStream(data);
    char *d = data.data();
    int len = data.length();
    if (len > 1024 * 1024 * 10)
    {
        emit sendDataMore();
        return;
    }
    // ÿ����෢��UDP_SEND_DATA_SIZE����
    int pos = 0;
    while (pos < len)
    {
        int sendsize = UDP_SEND_DATA_SIZE;
        int offset = len - pos;
        if (offset <= UDP_SEND_DATA_SIZE)
        {
            sendsize = offset;
        }
        NetworkData senddata(d + pos, sendsize);
        emit sender->signalSendData(senddata, addr, port, uid);
        pos += sendsize;
        thread()->msleep(3);
    }
//    qDebug() << "send size 1:" << pos;
}


void GroupChat::timerEvent(QTimerEvent *e)
{
    if (timerid == e->timerId())
    {
        if (gd.getGroupmember().isEmpty())
            return;
        // sync data
        MessageData msg;
        msg.setUid(myuid);
        GroupInviteData *content = new GroupInviteData;
        content->setGroupData(gd);
        msg.setDatatype(content->datatype());
        msg.setContent(content);
        QtConcurrent::run(this, &GroupChat::sendMsgRun,
                          msg,QHostAddress(gd.getNetmask()),
                          gd.getPort(), gd.getUid());
    }
}
