#include "headerphotoui.h"
#include <QtWidgets>
#include "editlabel.h"
#include "selfmenu.h"
#include "cardui.h"
#include "../../util/thmethod.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../config/configuredata.h"
#include "../../broadcast/broadcastsender.h"
#include "../../data/json/broadcastjsondata.h"
#include "../../tcp/socket/tcpserver.h"

#define FixHeight 70
HeaderPhotoUI::HeaderPhotoUI(QWidget *parent) : QWidget(parent)
{
    id = -1;
    statusid = 0;
    tcpEnabled = false;
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    BroadcastSender::Sender()->start();
    init();
}

HeaderPhotoUI::~HeaderPhotoUI()
{
    sendClose();
}

void HeaderPhotoUI::setPhoto(QString res)
{
    photo->setMoiveRes(res, QSize(FixHeight - 5, FixHeight - 5));
}

void HeaderPhotoUI::updateResource()
{
    ConfigureData *conf = ConfigureData::getInstance();
    QString str = conf->getIni("nickname");
    int len = str.length();
    QString css = cns("QLabel{font-family:微软雅黑;font:bold %1px;color:white;"
                      "}");
    if (len <= 8)
    {
        name->setStyleSheet(css.arg("14"));
    }
    else
    {
        name->setStyleSheet(css.arg("10"));
    }
    name->setText(str);
}

void HeaderPhotoUI::init()
{
    photo = new EmojiLabel(this);
    photo->setMovieable(true);
    name = new QLabel(this);
    saysay = new EditLabel(this);
    level = new QLabel(this);
    level->setVisible(false);
    status = new QPushButton(QStringLiteral("在线"), this);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *v1 = new QVBoxLayout();
    v1->setContentsMargins(0, 0, 0, 0);

    h2->addWidget(name, 9, Qt::AlignBottom | Qt::AlignLeft);
    h2->addWidget(level, 1, Qt::AlignBottom | Qt::AlignLeft);
    h2->addWidget(status, 1, Qt::AlignBottom | Qt::AlignLeft);

    v1->addLayout(h2);
    v1->addWidget(saysay, 0, Qt::AlignTop | Qt::AlignLeft);

    h1->addWidget(photo, 1, Qt::AlignCenter);
    h1->addLayout(v1, 5);

    setLayout(h1);

    setFixedSize(230, FixHeight);

    resize(230, FixHeight);

    photo->setMoiveRes(":res/baidu/15.png", QSize(FixHeight - 5, FixHeight - 5));
//    photo->setMoiveRes("F:/maya.png", QSize(FixHeight - 5, FixHeight - 5));
    photo->setFixedSize(FixHeight, FixHeight);
    photo->setStyleSheet(cns("QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}"));
    QFont font;
    font.setFamily(cns("微软雅黑"));
    font.setBold(true);
    font.setPointSize(10);
    QFontMetrics elidfont(font);
    name->setText(elidfont.elidedText(cns("昵称"), Qt::ElideRight, name->width()));
//    name->setContentsMargins(-10, 0, 0, 0);
    name->setStyleSheet(cns("QLabel{font-family:微软雅黑;font:bold 10px;color:white;"
                            "}"));
    saysay->setText(cns(""));
    saysay->setPlaceholderText(cns("输入你的个性宣言"));
    saysay->setFixedWidth(150);
    level->setPixmap(QPixmap(":res/money/13.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    level->setFixedSize(22, 22);
    SelfMenu *menu = new SelfMenu(status);
    QAction *a1 = new QAction(QStringLiteral("在线"), menu);
    a1->setData(0);
    QAction *a2 = new QAction(QStringLiteral("勿扰"), menu);
    a2->setData(1);
    QAction *a3 = new QAction(QStringLiteral("离开"), menu);
    a3->setData(2);
    menu->addAction(a1);
    menu->addAction(a2);
    menu->addAction(a3);
    menu->setContentsMargins(0, 0, 0, 0);
    menu->setStyleSheet(cns("QMenu{background-color:rgb(47, 107, 117, 0);"
                            "padding:0px;"
                            "border-width:0px;border-style:solid;border-radius:3px;border-color:transparent;}"
                            "QMenu::item{background:transparent;"
                            "color:white}"
                            "QMenu::item:selected{background:rgba(225, 225, 225, 45);}"));
//    menu->setWindowOpacity(0.9);
    status->setMenu(menu);
    status->setStyleSheet(cns("QPushButton{font-family:微软雅黑;"
                                         "color:white;"
                                         "font:10px;"
                                         "background:transparent;"
                                         "border-width:1px;"
                                         "border-style:solid;"
                                         "border-color: transparent;"
                                         "border-radius: 3px;}"
                                         "QPushButton:hover{border-color: white;}"
                                         "QPushButton:menu-indicator{subcontrol-position:right center;"
                                         "subcontrol-origin:padding;"
                                         "image:url(:/res/ui3/arrow_down_normal.png);"
                                         "top:0px}"));
    status->setFixedSize(37, 20);


    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateResource();
    saysay->setText(conf->getIni("feel"));
    if (conf->getIntIni("load_status") != 0)
    {
        status->setText(conf->getIni("status"));
    }
    photo->setMoiveRes(conf->getIni("photo"), QSize(FixHeight - 5, FixHeight - 5));
    // connect
    connect(photo, &EmojiLabel::enterEmoji, this, [=] () {
        CardUI *ui = new CardUI();
        ui->setPicture(conf->getIni("image"));
        ui->setFirstTxt(name->text());
        ui->setSecondTxt(QStringLiteral("自己"));
        ui->setVersion(HAPPY_VERSON);
        ui->setThirdTxt(conf->getIni("ip"));
        ui->setIntroduceTxt(saysay->text());
        ui->showAnimation();
        ui->activateWindow();
        ui->move(mapToGlobal(this->pos()).x() - 370, QCursor::pos().y() - 80);
        connect(photo, &EmojiLabel::leaveEmoji, ui, [=] () {
            disconnect(photo, &EmojiLabel::leaveEmoji, 0, 0);
            ui->hideAnimation();
        });
    });

    connect(menu, &SelfMenu::triggered, this, [=] (QAction *a) {
        if (!a)
            return;
        statusid = a->data().toInt();
        status->setText(a->text());
        emit selfStatusChanged(statusid);
        conf->setIni("status", a->text());
    });
    connect(saysay, &EditLabel::editingFinished, this, [=] () {
        emit selfFeelChanged(saysay->text());
        conf->setIni("feel", saysay->text());
    });

    // start listen
    TcpServer *server = new TcpServer(this);
    tcpEnabled = server->listen(QHostAddress::Any, HAPPY_LISTEN_PORT);

    // send self braodcast

    id = startTimer(6000);

//    QTimer::singleShot(6000, this, [=] () {
//        // test send close
//        sendClose();
//    });
}

void HeaderPhotoUI::sendBroadcast()
{
    BroadcastSender *sender = BroadcastSender::Sender();
    ConfigureData *config = ConfigureData::getInstance();
    BroadcastJsonData data;
    data.setUid(config->getUuid());
    data.setGsoapip(config->getIni("ip"));
    data.setNickname(name->text());
    data.setVersion(HAPPY_VERSON);
    data.setVersioninfo(cn("更新说明:"
                           "\n1.解决文件传输点击终止崩溃问题"
                           "\n2.增加文件传输时的状态判断"
                           "\n3.更新升级程序为1.1"
                           "\n20170713"));
    data.setTcpenabled(tcpEnabled);
    data.setStatus(statusid);
    data.setFeel(saysay->text());
    data.setPhoto(photo->ImagePath());
    data.setImage(config->getIni("image"));
    data.setMac(config->getIni(("mac")));
    QByteArray ba = data.toJson();
    emit sender->signalSendData(NetworkData(ba.data(), ba.size()),
                                QHostAddress(HAPPY_DEFULT_BROADCASR_ADDRESS),
                                HAPPY_DEFULT_BROADCASR_PORT, config->getUuid());
}

void HeaderPhotoUI::sendClose()
{
    // stopTimer
    if (id != -1)
    {
        killTimer(id);
        id = -1;
    }
    statusid = 3;
    sendBroadcast();
}



void HeaderPhotoUI::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == id)
    {
        sendBroadcast();
    }
}
