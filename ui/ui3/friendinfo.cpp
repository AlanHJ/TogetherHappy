#include "friendinfo.h"
#include <QtWidgets>
#include "editlabel.h"
#include "../../config/configuredata.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../data/json/broadcastjsondata.h"
#include "../../broadcast/broadcastreceverjson.h"
#include "../../gsoap/call/gsoapcalloperation.h"
#include "../../gsoap/gsoapmessageinstance.h"
#include "../../sql/sql.h"
#include "../../data/db/matchdata.h"

FriendInfo::FriendInfo(QWidget *parent) : QWidget(parent)
{
    // 创建界面
    data = nullptr;
    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(5, 5, 8, 5);
    v->setSpacing(0);

    // friend
    photoLbl = new EmojiLabel(this);
    photoLbl->setMovieable(true);
    QImage image(":res/ui3/show_default.gif");
    photoLbl->setMoiveRes(":res/ui3/show_default.gif",
                          image.rect().contains(QRect(0, 0, 150, 220)) ? QSize(150, 220) : image.size());
    nameLbl = new QLabel(QStringLiteral("资源获取中..."), this);
    editLbl = new QLabel(QStringLiteral(""), this);
    versionLbl = new QLabel(QStringLiteral(""), this);
    extraInfoLbl = new QLabel((""), this);
    sayLbl = new QLabel(QStringLiteral(""), this);
    sayLbl->adjustSize();
    sayLbl->setWordWrap(true);

    v->addWidget(photoLbl);
    v->addSpacing(4);
    v->addWidget(nameLbl, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(editLbl, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(versionLbl, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(extraInfoLbl, 0, Qt::AlignHCenter);
    v->addSpacing(2);
    v->addWidget(sayLbl);

    // 自己
//    EmojiLabel *selfImge = new EmojiLabel(this);
//    selfImge->setMovieable(true);
//    QLabel *nameLbl1 = new QLabel(QStringLiteral(""), this);
//    QLabel *editLbl1 = new QLabel(QStringLiteral("自己"), this);
//    QLabel *extraInfoLbl1 = new QLabel(("127.0.0.1"), this);
//    QLabel *sayLbl1 = new QLabel(QStringLiteral(""), this);
//    sayLbl1->adjustSize();
//    sayLbl1->setWordWrap(true);

//    v->addStretch(1);
//    v->addWidget(selfImge);
//    v->addSpacing(4);
//    v->addWidget(nameLbl1, 0, Qt::AlignHCenter);
//    v->addSpacing(2);
//    v->addWidget(editLbl1, 0, Qt::AlignHCenter);
//    v->addSpacing(2);
//    v->addWidget(extraInfoLbl1, 0, Qt::AlignHCenter);
//    v->addSpacing(2);
//    v->addWidget(sayLbl1);

    v->addStretch(4);

    setLayout(v);


    QString css = QStringLiteral("font-family:微软雅黑;font:12px;color:white;");
    nameLbl->setStyleSheet(css);
    editLbl->setStyleSheet(css);
    versionLbl->setStyleSheet(css);
    extraInfoLbl->setStyleSheet(css);
    sayLbl->setStyleSheet(css);

    sender = GsoapCallOperation::Sender();
    GsoapMessageInstance *recever = GsoapMessageInstance::GetInstance();

    connect(recever, &GsoapMessageInstance::signalGetResource,
            this, [=] (QString srcUid, QString type, QString res) {
        if (srcUid != data->getUid())
            return;
        else if (type =="image")
        {
            if (QFile::exists(res))
            {
                QImage image(res);
                photoLbl->setMoiveRes(res, image.rect().contains(QRect(0, 0, 150, 220))
                                      ? QSize(150, 220) : image.size());
            }
        }
    });
    data = new BroadcastJsonData(this);
    connect(data, &BroadcastJsonData::nicknameChanged,
            this, [=] () {
        QString nick = data->getNickname();
        if (!nick.isEmpty())
            nameLbl->setText(nick);
        else
            nameLbl->setText(data->getGsoapip());
        extraInfoLbl->setText(data->getGsoapip());
    });
    connect(data, &BroadcastJsonData::feelChanged,
            this, [=] () {
        sayLbl->setText(cn("心情:") + data->getFeel());
    });
    connect(data, &BroadcastJsonData::imageChanged,
            this, [=] () {
        // gsoap get image
        QString file1 = QFileInfo(photoLbl->ImagePath()).fileName();
        QString file2 = QFileInfo(data->getImage()).fileName();
        if (file1 == file2)
            return;
        emit sender->signalGetResource(data->getUid(),
                                       data->getGsoapip(),
                                       "image", data->getImage());
    });

    connect(data, &BroadcastJsonData::parserComplete,
            this, [=] () {
        versionLbl->setText(cn("哈啤%1").arg(data->getVersion()));
    });

//    nameLbl1->setStyleSheet(css);
//    editLbl1->setStyleSheet(css);
//    extraInfoLbl1->setStyleSheet(css);
//    sayLbl1->setStyleSheet(css);

    // load self
//    ConfigureData *config = ConfigureData::getInstance();
//    selfImge->setMoiveRes(config->getIni("image"), QSize(80, 80));
//    nameLbl1->setText(config->getIni("nickname"));
//    extraInfoLbl1->setText(config->getIni("ip"));
//    sayLbl1->setText(config->getIni("feel"));
}

FriendInfo::~FriendInfo()
{

}

void FriendInfo::setJsondata(QString uid)
{
    this->data->setUid(uid);
    updateMemoName();
}

void FriendInfo::updateMemoName()
{
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (db)
    {
        QString sql = "SELECT * FROM "+ MatchData().objectName() +
                " WHERE uid = '" + data->getUid() + "'";
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
                editLbl->setText(md->getEditname());
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

void FriendInfo::setRecever(BroadcastReceverJson *recever)
{
    connect(recever, &BroadcastReceverJson::jsonMsg,
            this, [=] (QString uid, QJsonObject json) {
        if (!data)
            return;
        if (uid != data->getUid())
            return;
        data->parserJson(json);
    });
}

