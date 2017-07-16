#include "personitem.h"
#include "../../broadcast/broadcastreceverjson.h"
#include "../../ui/ui3/personchat.h"
#include "../../ui/ui3/cardui.h"
#include "../../data/json/broadcastjsondata.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../gsoap/call/gsoapcalloperation.h"
#include "../../gsoap/gsoapmessageinstance.h"
#include "selftabwidget.h"
#include "../../config/configuredata.h"
#include "../../data/db/matchdata.h"
#include "../../sql/sql.h"
#include <QtConcurrent/QtConcurrent>
#include "selfmenu.h"
#include "msgbox.h"
#include "inputbox.h"
#include <QtWidgets>

PersonItem::PersonItem(BroadcastRecever *recever,
                       QTreeWidgetItem *it,
                       SelfTabWidget *t, QWidget *parent)
    : ItemEditor(parent)
{
    jsondata = nullptr;
    item = nullptr;
    this->recever = nullptr;
    chat = nullptr;
    shakeabled = true;
    rejectShake = false;
    memoname.clear();
    item = it;
    tab = t;
    showMaxMin(ConfigureData::getInstance()->IsTrue("item_show"));


    GsoapCallOperation *sender = GsoapCallOperation::Sender();
    GsoapMessageInstance *msgrecever = GsoapMessageInstance::GetInstance();

    connect(msgrecever, &GsoapMessageInstance::signalGetResource,
            this, [=] (QString srcUid, QString type, QString res) {
        if (srcUid != jsondata->getUid())
            return;
        if (type == "photo")
        {
            if (QFile::exists(res))
            {
                emoji->setMoiveRes(res,
                                   QSize(emojiSize - 10, emojiSize - 10));

            }
        }
        else if (type =="image")
        {
            if (QFile::exists(res))
            {
                image = res;
            }
        }
    });

    jsondata = new BroadcastJsonData(this);

    connect(jsondata, &BroadcastJsonData::versionChanged,
            this, [=] () {
        if (!jsondata->getTcpenabled())
            return;
        tab->updateMsg(jsondata->getVersion(),
                       jsondata->getVersioninfo(),
                       jsondata->getGsoapip());
    });

    connect(jsondata, &BroadcastJsonData::nicknameChanged,
            this, [=] () {
        showMemoName();
    });
    connect(jsondata, &BroadcastJsonData::statusChanged,
            this, [=] () {
        if (itemType != realityBody)
            return;
        int id = jsondata->getStatus();
        if (id == 3)
        {
            emit ItemClose(jsondata->getUid());
            // left close this;
            delete item;
            item = nullptr;
        }
        else
        {
            other->setText(StatusStr[jsondata->getStatus()]);
        }
    });
    connect(jsondata, &BroadcastJsonData::feelChanged,
            this, [=] () {
        QFontMetrics fm(cn("微软雅黑"));
        introduce->setText(fm.elidedText(jsondata->getFeel(), Qt::ElideMiddle, 200));
    });
    connect(jsondata, &BroadcastJsonData::photoChanged,
            this, [=] () {
        // gsoap get photo
        QString file1 = QFileInfo(emoji->ImagePath()).fileName();
        QString file2 = QFileInfo(jsondata->getPhoto()).fileName();
        if (file1 == file2)
            return;
        emit sender->signalGetResource(jsondata->getUid(),
                                       jsondata->getGsoapip(),
                                       "photo", jsondata->getPhoto());
    });
    connect(jsondata, &BroadcastJsonData::imageChanged,
            this, [=] () {
        // gsoap get image
        QString file1 = QFileInfo(image).fileName();
        QString file2 = QFileInfo(jsondata->getImage()).fileName();
        if (file1 == file2)
            return;
        emit sender->signalGetResource(jsondata->getUid(),
                                       jsondata->getGsoapip(),
                                       "image", jsondata->getImage());
    });
    BroadcastReceverJson *jsonrecever = qobject_cast<BroadcastReceverJson*>(recever);
    if (jsonrecever)
    {
        this->recever = jsonrecever;
        connect(this->recever, &BroadcastReceverJson::jsonMsg,
                this, [=] (QString uid, QJsonObject json) {
            if (uid != this->uid)
                return;
            jsondata->parserJson(json);
        });
    }

    connect(this, &PersonItem::handlePaser, this, [=] (QJsonObject json) {
        jsondata->parserJson(json);
    });

    connect(emoji, &EmojiLabel::enterEmoji, this, [=] () {
        CardUI *ui = new CardUI();
        ui->setPicture(image);
        ui->setFirstTxt(jsondata->getNickname());
        ui->setSecondTxt(memoname);
        ui->setVersion(jsondata->getVersion());
        ui->setThirdTxt(jsondata->getGsoapip());
        ui->setIntroduceTxt(jsondata->getFeel());
        ui->showAnimation();
//        ui->activateWindow();
        ui->move(mapToGlobal(this->pos()).x() - 360, QCursor::pos().y() - 50);
        connect(emoji, &EmojiLabel::leaveEmoji, ui, [=] () {
//            disconnect(emoji, &EmojiLabel::leaveEmoji, 0, 0);
            ui->hideAnimation();
        });
    });
    menu = new SelfMenu(this);
    connect(menu->addAction(cns("备注名")), &QAction::triggered, this, [=] () {
        bool ok;
        QString name = InputBox::GetInput(cns("输入备注名"), &ok);
        if (!ok)
            return;
        if (name.trimmed().isEmpty())
        {
            MsgBox::ShowMsgBox(cn("提示"), cn("不能为空"), cn("确定"));
            return;
        }
        if (name.trimmed().length() >= 18)
        {
            MsgBox::ShowMsgBox(cn("错误"), cn("昵称长度18限制"), cn("确定"));
            return;
        }
        QString er;
        SqlDB *db = Sql::instance()->open(er);
        if (!db)
        {
            MsgBox::ShowMsgBox(cn("失败"), cn("数据库错误\n") + er, cn("确定"));
            return;
        }
        QString sql = "SELECT * FROM "+ MatchData().objectName() +
                " WHERE uid = '" + uid + "'";
        int rlt;
        QList<QObject *> objs = db->executeQuery(sql,
                                                 MatchData().metaObject(),
                                                 rlt, er);
        if (rlt != 0)
        {
            MsgBox::ShowMsgBox(cn("失败"), cn("数据库读取错误\n") + er, cn("确定"));
            return;
        }
        bool b  = true;
        if (objs.isEmpty())
        {
            // insert
            MatchData md;
            md.setUid(uid);
            md.setEditname(name);
            md.setNickname(jsondata->getNickname());
            md.setEdittime(QDateTime::currentDateTime().toTime_t());
            md.setIp(jsondata->getGsoapip());
            md.setModifycounts(1);
            b = db->insert(&md, er);
        }
        else
        {
            // update
            MatchData *md = qobject_cast<MatchData *>(objs.first());
            if (md)
            {
                md->setEditname(name);
                md->setNickname(jsondata->getNickname());
                md->setEdittime(QDateTime::currentDateTime().toTime_t());
                md->setIp(jsondata->getGsoapip());
                md->setModifycounts(md->getModifycounts() + 1);
                b = db->update(md, "uid", er);
            }
            else
            {
                b = false;
                er = cn("无法修改，读取错误!!!");
            }

            auto it = objs.begin();
            while (it != objs.end())
            {
                QObject *obj = *it;
                delete obj;
                obj = nullptr;
                ++it;
            }
        }
        db->close();
        if (!b)
        {
            MsgBox::ShowMsgBox(cn("修改失败"), er, cn("确定"));
        }
        else
        {
            memoname = name;
        }
        showMemoName();
    });
}

PersonItem::PersonItem()
{
    jsondata = nullptr;
    item = nullptr;
    this->recever = nullptr;
    chat = nullptr;
    shakeabled = true;
    rejectShake = false;
    item = nullptr;
    tab = nullptr;
    memoname.clear();
}

PersonItem::~PersonItem()
{
    disconnect(this, &PersonItem::cloneCreateChat, 0, 0);
}

QTreeWidgetItem *PersonItem::getItem()
{
    return item;
}

void PersonItem::setStatus(PersonItem::status s)
{
    other->setText(StatusStr[s]);
}

void PersonItem::setIntroduce(QString str)
{
    introduce->setText(str);
}

void PersonItem::setJsondata(BroadcastJsonData data)
{
    jsondata->clone(data);
}

QString PersonItem::getGsopip()
{
    QString ip;
    if (jsondata)
        ip = jsondata->getGsoapip();
    return ip;
}

QString PersonItem::getMemoname()
{
    return memoname;
}

QString PersonItem::getNickname()
{
    QString str;
    if (jsondata)
        str = jsondata->getNickname();
    return str;
}

bool PersonItem::checkVerson(QString &ip, QString &info)
{
    if (!jsondata)
        return false;
    if (!jsondata->getTcpenabled())
        return false;
    if (jsondata->getVersion() <= HAPPY_VERSON)
        return false;
    ip = jsondata->getGsoapip();
    info = jsondata->getVersioninfo();
    return true;
}

Chat *PersonItem::createChat()
{
    if (chat)
    {
        chat->showNormal();
        chat->activateWindow();
        return chat;
    }
    chat = new PersonChat;
    chat->setRecever(recever);
    chat->setShakeEnable(shakeabled);
    chat->setRejectShake(rejectShake);
    if (jsondata)
        chat->setJsonData(jsondata->copy());
    chat->setChatTitle(cns("与") + name->text() + cns("聊天中"), emoji->ImagePath());
    connect(chat, &PersonChat::chatClose, this, [=] () {
        disconnect(chat, &PersonChat::chatClose, this, 0);
        disconnect(chat, &PersonChat::chatShow, this, 0);
        disconnect(chat, &PersonChat::starkShake, this, 0);
        disconnect(chat, &PersonChat::shakeRejectStatusChanged, this, 0);
        chat = nullptr;
        tab->returnChats()->remove(jsondata->getUid());
    });
    connect(chat, &PersonChat::chatShow, this, [=] () {
        stopNotify();
    });
    connect(chat, &PersonChat::shakeRejectStatusChanged,
            this, [=] (bool b) {
        rejectShake = b;
    });
    connect(chat, &PersonChat::starkShake, this, [=] () {
        // 计时开始
        //30s后重置
        shakeabled = false;
        QTimer::singleShot(10000, this, [=] () {
            shakeabled = true;
            if (chat)
                chat->setShakeEnable(true);
        });
    });
    chat->show();
    chat->activateWindow();
    emit cloneCreateChat(chat);
    return chat;
}

bool PersonItem::isChatVisible()
{
    if (!chat)
        return true;
    return chat->isVisible();
}


void PersonItem::showChat()
{
    if (chat && chat->isHidden())
    {
        chat->show();
        chat->activateWindow();
    }
}


bool PersonItem::setChat(Chat *ct)
{
    if (!ct)
        return false;
    if (chat)
    {
        return false;
    }
    PersonChat *pchat = dynamic_cast<PersonChat *>(ct);
    if (!pchat)
    {
        return false;
    }
    chat = pchat;
    chat->setShakeEnable(shakeabled);
    connect(chat, &PersonChat::chatClose, this, [=] () {
        disconnect(chat, &PersonChat::chatClose, this, 0);
        disconnect(chat, &PersonChat::chatShow, this, 0);
        disconnect(chat, &PersonChat::starkShake, this, 0);
        disconnect(chat, &PersonChat::shakeRejectStatusChanged, this, 0);
        chat = nullptr;
        tab->returnChats()->remove(jsondata->getUid());
    });
    connect(chat, &PersonChat::chatShow, this, [=] () {
        stopNotify();
    });
    connect(chat, &PersonChat::shakeRejectStatusChanged,
            this, [=] (bool b) {
        rejectShake = b;
    });
    connect(chat, &PersonChat::starkShake, this, [=] () {
        // 计时开始
        //30s后重置
        shakeabled = false;
        QTimer::singleShot(10000, this, [=] () {
            shakeabled = true;
            if (chat)
                chat->setShakeEnable(true);
        });
    });
    return true;
}

void PersonItem::showMemoName()
{
    // load data
    ConfigureData *config = ConfigureData::getInstance();
    nameFlag = config->getIntIni("show_name");
    if (nameFlag == 1)
    {
        if (jsondata)
        {
            QString nick = jsondata->getNickname();
            if (nick.isEmpty())
                setName(jsondata->getGsoapip());
            else
                setName(nick);
        }
    }
    else
    {
        if (memoname.isEmpty())
        {
            if (jsondata)
            {
                QString nick = jsondata->getNickname();
                if (nick.isEmpty())
                    setName(jsondata->getGsoapip());
                else
                    setName(nick);
            }
            else
            {
                setName(cn("昵称获取中"));
            }
        }
        else
        {
            setName(memoname);
        }
    }

    if (chat)
    {
        chat->setChatTitle(cns("与") + name->text() + cns("聊天中"), emoji->ImagePath());
    }
}


void PersonItem::contextMenuEvent(QContextMenuEvent *e)
{
    if (uid == ConfigureData::getInstance()->getUuid())
        return;
    menu->exec(QCursor::pos());
    e->accept();
}


void PersonItem::setUid(QString id)
{
    ItemEditor::setUid(id);

    // load sql
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (db)
    {
        QString sql = "SELECT * FROM "+ MatchData().objectName() +
                " WHERE uid = '" + uid + "'";
        int rlt;
        QList<QObject *> objs = db->executeQuery(sql,
                                                 MatchData().metaObject(),
                                                 rlt, er);
        db->close();
        if (objs.size() >= 1)
        {
            MatchData *md = qobject_cast<MatchData *>(objs.first());
            if (md)
            {
                memoname = md->getEditname();
            }
        }
        auto it = objs.begin();
        while (it != objs.end())
        {
            QObject *obj = *it;
            delete obj;
            obj = nullptr;
            ++it;
        }
    }
}


ItemEditor *PersonItem::clone()
{
    PersonItem *it = new PersonItem();
    it->setItemTyle(cloneBody);
    it->setUid(uid);
    it->setName(name->text());
    it->emoji->setMoiveRes(emoji->ImagePath(),
                           QSize(emojiSize - 10, emojiSize - 10));
    it->introduce->setText(introduce->text());
    it->showMaxMin();
    return it;
}
