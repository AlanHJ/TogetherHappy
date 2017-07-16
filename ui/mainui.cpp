#include "mainui.h"
#include "configui.h"
#include "buildgroupui.h"
#include "loademojiui.h"
#include "groupchatui.h"
#include "memberui.h"
#include "happyprogressui.h"
#include "moveemojicustom.h"
#include "../manager/cleartempopt.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include "../broadcast/existbroadcast.h"
#include "../cutImage/widget.h"
#include <QTreeWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QHostInfo>
#include <QPainter>
#include <QCloseEvent>
#include <QtNetwork>
#include <QHash>
#include <QStyleFactory>
#include <QtConcurrent/QtConcurrent>
#include <QFontDatabase>
#include "hotkeyregiter.h"
#include "../gsoap/gsoapmessageinstance.h"
#include "emojiinstance.h"
#include "notifyui.h"
#include "ui3/searchwidget.h"
#include "ui3/happytabwidget.h"
#include "ui3/selftabwidget.h"
#include "../thwidgets/model/itemeditor.h"
#include <QTreeWidgetItem>
#include "ui3/headerphotoui.h"
#include "../thwidgets/happytreewidget.h"
#include "../gsoap/thread/gsoapthread.h"
#include "../gsoap/call/gsoapcalloperation.h"
#include "../cutImage/cutimagewidget.h"
#include "ui3/rotatebutton.h"
#include "ui3/swinglabel.h"
#include "ui3/calendarlabel.h"
#include "ui3/selfmenu.h"
#include "happytitlebar.h"
#include "ui3/msgbox.h"
#include "ui3/themepicker.h"
#include "ui3/optionsui.h"
#include "ui3/emotionsbox.h"
#include "ui3/emotionmanager.h"
#include "ui3/inputbox.h"
#include "ui3/lockwidget.h"
#include "../data/db/emoticongroupdata.h"
#include "ui3/listwidget.h"
#include "ui3/combox.h"
#include "ui3/personitem.h"
#include "ui3/abouthp.h"
#include "ui3/chat.h"
#include "ui3/muiltprogressbar.h"
#include "../manager/widgetmanager.h"

static const int NotifyInterval = 330;
MainUI::MainUI(THWidgetBase *parent)
    : THWidgetBase(parent)
{

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    qRegisterMetaType<EmoticonData>("EmoticonData");
    qRegisterMetaType<EmoticonGroupData>("EmoticonGroupData");

    init();



    mGsoap = new GsoapThread();
    mGsoap->start();
    connect(mGsoap, &GsoapThread::gsoapServerError, this, [=] () {
        MsgBox::ShowMsgBox(cn("错误"),
                           cn("无法监听"),
                           cn("确定"));
        mGsoap->stopGoap();
        qApp->exit();
    });

//    gsoapOpt  = new GsoapCallOperation();
//    gsoapOpt->startCall();

}

MainUI::~MainUI()
{

}

void MainUI::hotkeyclicked()
{
    int x = qApp->desktop()->width() - width();
    int y = 0;
    move(x, y);
}

void MainUI::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (mTray->isVisible() == false)
    {
        mTray->show();
    }
    switch (reason)
    {
//    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    {
//        setWindowState(Qt::WindowActive);
//        activateWindow();
//        this->show();
//        this->showNormal();
    }
        break;
    }
}

void MainUI::screenshot()
{
    CutImageWidget *cut = CutImageWidget::GetInstance();
    if (cut->isVisible())
        return;
    cut->init();
    cut->show();
    cut->activateWindow();

//    CutImageWidget *cut = new CutImageWidget();
//    cut->show();
//    cut->activateWindow();
}

void MainUI::updateHappy()
{
    // 从服务器获取新版本
    mTray->showMessage(cns("更新消息"), cns("开始检查"), QSystemTrayIcon::Information, 1000);
}

void MainUI::updateInfo(QString info)
{
    mTray->showMessage(cns("更新消息"), info, QSystemTrayIcon::Information, 1000);
    if (info == cns("更新成功"))
    {
        // 重启该应用完成更新
        mTray->showMessage(cns("更新消息"), cns("更新成功, 5秒后完成重启"), QSystemTrayIcon::Information, 1000);
        thread()->msleep(1000);

//        QString path = "D:/Program Files/happy/";
//        path.replace("/", "\\");

        bool b = QProcess::startDetached("UpdateHappy.exe");
        if (b == false)
        {
            mTray->showMessage(cns("更新消息"), cns("重启失败"), QSystemTrayIcon::Information, 1000);
        }
    }
}

void MainUI::updateUpdate()
{
    // get mac
    QList<QNetworkInterface> interfacelst = QNetworkInterface::allInterfaces();
    for (QNetworkInterface face : interfacelst)
    {
        if (face.flags() == (QNetworkInterface::IsUp |
                             QNetworkInterface::IsRunning |
                             QNetworkInterface::CanBroadcast |
                             QNetworkInterface::CanMulticast))
        {
            if (!face.humanReadableName().contains("VMw", Qt::CaseInsensitive)
                    && !face.humanReadableName().contains("Vir", Qt::CaseInsensitive))
            {
                for (QNetworkAddressEntry entry : face.addressEntries())
                {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    {
                        QString mac = face.hardwareAddress(); // MAC address
                        ConfigureData::getInstance()->setIni("mac", mac);
                        break;
                    }
                }
                break;
            }
        }
    }

    QFile::remove(qApp->applicationDirPath() + "/HappyUpdate.exe");
    QString error;
    if (!CpFile(":res/update/HappyUpdate.exe",
                  GetWorkPath() + "/update/HappyUpdate.exe",
                  error))
    {
        emit updateUpdateInfo(cn("释放新的升级程序失败"));
    }
}

void MainUI::clearTemp()
{
    // 清空缓存
    HappyProgressUI *ui = new HappyProgressUI(this);
    ClearTempOpt *opt = new ClearTempOpt();
    ui->setShowName(cns("正在删除你的缓存文件"));
    connect(opt, SIGNAL(signalProgressinfo(QString)),
            ui, SLOT(setProgressInfo(QString)));
    connect(opt, SIGNAL(signalProgress(int)),
            ui, SIGNAL(setProgress(int)));
    ui->show();
    movecenter(ui);
    ui->activateWindow();
    opt->start();
}

void MainUI::abouthappy()
{
    // 更新提示
    QTimer::singleShot(3000, this,  [=] () {
        emit about->clicked();
    });
//    this->showNormal();
//    QFile file(":res/updatestr/str.data");
//    if (file.exists())
//    {
//        if (file.open(QIODevice::ReadOnly))
//        {
//            QByteArray ba = file.readAll();
//            file.close();
//            QMessageBox::information(this, cns("关于哈啤") + QString::number(HAPPY_VERSON),
//                                     QString::fromLocal8Bit(ba).arg(HAPPY_VERSON), cns("吼啊"));
//        }
//    }
}

void MainUI::init()
{
    // 初始化界面
    // 创建dock栏图标
    mTray = new QSystemTrayIcon(this);

    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    photo = new HeaderPhotoUI(this);
    photo->setStyleSheet("QWitget{background:yellow;}");
    SearchWidget *search = new SearchWidget(this);

    SelfTabWidget *tab = new SelfTabWidget(mTray, this);
    // 增加设置和表情管理按钮
    // 增加全局通知label，以后改为插件中心
    SwingLabel *notifyico = new SwingLabel(this);
    notifyico->setToolTip(QStringLiteral("哈啤提醒"));
//    notifyico->setStyleSheet("QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}");
    notifyico->setFixedSize(15, 15);
    notifyico->setImagePath(":res/ui3/notify.png");
//    notifyico->setPixmap(QPixmap(":res/ui3/notify.png").scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel *notify = new QLabel(QStringLiteral("欢迎使用哈啤3.0"), this);
    notify->setContentsMargins(0, 0, 0, 0);
    notify->setAlignment(Qt::AlignLeft);
    notify->setFixedWidth(160);
    notify->setStyleSheet(cns("QLabel{font-family:微软雅黑;font:12px;color:white;"
                            "}") /*+ "QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}"*/);
    notify->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    CalendarLabel *calendar = new CalendarLabel(this);
    calendar->setText("00:00:00");
//    calendar->setStyleSheet("QLabel{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}");
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(10, 0, 0, 0);
    h1->setSpacing(1);
    h1->addWidget(notifyico, 0, Qt::AlignLeft | Qt::AlignCenter);
    h1->addWidget(notify, 0, Qt::AlignLeft | Qt::AlignCenter);
    h1->addWidget(calendar, 0, Qt::AlignLeft | Qt::AlignCenter);
    h1->addStretch();
    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(3, 0, 0, 0);
    QPushButton *options = new QPushButton(this);
    options->setToolTip(QStringLiteral("设置"));
    options->setFixedSize(25, 25);
    QPushButton *emojis = new QPushButton(this);
    emojis->setToolTip(QStringLiteral("表情管理"));
    emojis->setFixedSize(25, 25);
    QPushButton *resource = new QPushButton(this);
    resource->setToolTip(QStringLiteral("资源中心"));
    resource->setFixedSize(25, 25);
    QPushButton *plug = new QPushButton(this);
    plug->setToolTip(QStringLiteral("扩展管理"));
    plug->setFixedSize(25, 25);
    RotateButton *fresh = new RotateButton(this);
    fresh->setImagePath(":res/ui3/refresh");
    fresh->setToolTip(QStringLiteral("刷新列表"));
    fresh->setFixedSize(25, 25);
    QPushButton *lock = new QPushButton(this);
    lock->setToolTip(QStringLiteral("锁定"));
    lock->setFixedSize(25, 25);
    about = new QPushButton(this);
    about->setToolTip(QStringLiteral("关于"));
    about->setFixedSize(25, 25);
    QFile file(":res/css/button.css");
    QString str;
    if (file.open(QIODevice::ReadOnly))
    {
        str = file.readAll();
    }
    file.close();
    options->setStyleSheet(str.arg("options").arg("options_hover").arg("options_press"));
    emojis->setStyleSheet(str.arg("image").arg("image_hover").arg("image_press"));
    resource->setStyleSheet(str.arg("res").arg("res_hover").arg("res_press"));
    plug->setStyleSheet(str.arg("plug").arg("plug_hover").arg("plug_press"));
//    fresh->setStyleSheet(str.arg("refresh").arg("refresh_hover").arg("refresh_press"));
    lock->setStyleSheet(str.arg("lock").arg("lock_hover").arg("lock_press"));
    about->setStyleSheet(str.arg("about").arg("about_hover").arg("about_press"));
    connect(fresh, &QPushButton::clicked, this, [=] () {


    });

//    h2->addStretch();
    h2->addWidget(options);
    h2->addWidget(emojis);
    h2->addWidget(resource);
    h2->addWidget(plug);
    h2->addWidget(fresh);
    h2->addWidget(lock);
    h2->addWidget(about);
//    h2->addStretch();

    resource->setVisible(false);
    plug->setVisible(false);

    v->addSpacerItem(new QSpacerItem(250, 30));
    v->addWidget(photo, 0, Qt::AlignTop | Qt::AlignHCenter);
    v->addWidget(search, 0, Qt::AlignTop | Qt::AlignHCenter);
    v->addWidget(tab, 1, Qt::AlignTop | Qt::AlignHCenter);
    v->addLayout(h1, 0);
    v->addLayout(h2, 0);
    v->addSpacerItem(new QSpacerItem(250, 13));
    setLayout(v);

    setFixedSize(250, 630);
    setTitleBarWidth(250);

    ConfigureData *conf = ConfigureData::getInstance();

    connect(mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayActivated(QSystemTrayIcon::ActivationReason)));
    mTray->setIcon(QIcon(":/res/happy.png"));
    mTray->setToolTip(cns("来啊,一起哈啤"));
    SelfMenu *menu = new SelfMenu(this);
    QAction *quita = new QAction(cns("退出"), this);
    connect(quita, &QAction::triggered, this, [=] () {
        emit bar->signalClose();
    });
    QAction *seta = new QAction(cns("设置"), this);
    connect(seta, &QAction::triggered, this, [=] () {
        emit options->clicked();
    });
//    QAction *cleara = new QAction(cns("清空缓存"), this);
//    connect(cleara, SIGNAL(triggered()), this, SLOT(clearTemp()));
    QAction *abouta = new QAction(cns("关于"), this);
    connect(abouta, &QAction::triggered, this, [=] () {
        emit about->clicked();
//        THWidgetBase *test = new THWidgetBase;
//        test->hideTitleBar();
//        test->setFixedSize(400, 300);
//        test->bezierEnable = true;
//        test->bezierTime->setFrameRange(0, 400);
//        test->bezierTime->start();
//        test->show();
    });
    QAction *update = new QAction(cns("检查更新"), this);
    connect(update, &QAction::triggered, this, [=] () {
        // check update
        tab->updateCheck();
    });

    QAction *restart = new QAction(cns("重启哈啤"), this);
    connect(restart, &QAction::triggered, this, [=] () {
        // check update
        qApp->exit(100);
    });

    menu->addAction(update);
//    menu->addAction(cleara);
    menu->addAction(restart);
    menu->addAction(seta);
    menu->addAction(abouta);
    menu->addAction(quita);
    mTray->setContextMenu(menu);
    file.setFileName(":res/css/menu.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString cssmenu = file.readAll();
        menu->setStyleSheet(cssmenu);
    }
    file.close();
    mTray->show();
    setAcceptDrops(false);

    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(cn("哈啤") +
                       QString::number(HAPPY_VERSON));

    connect(bar, &HappyTitleBar::signalMinimize, this, &QWidget::hide);

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        MsgBox *box = new MsgBox();
        box->setTitle(QStringLiteral("哈啤提示"));
        box->setContent(QStringLiteral("确定关闭哈啤么"));
        int i = box->exec();
        if (i == 0)
            qApp->exit();
    });

    connect(bar, &HappyTitleBar::signalExtra, this, [=] () {
        ThemePicker *pk = new ThemePicker();
        pk->show();
        pk->activateWindow();
    });

    connect(options, &QPushButton::clicked, this, [=] () {
        OptionsUI *ui = nullptr;
        if (!WidgetManager::GetInstance()->isExist(&ui))
        {
            ui = new OptionsUI();
            connect(ui, &OptionsUI::notify, this, [=] () {
                photo->setPhoto(conf->getIni("photo"));
                photo->updateResource();
            });
        }
        ui->show();
        ui->activateWindow();
    });

    connect(emojis, &QPushButton::clicked, this, [=] () {
        EmotionManager *manager = nullptr;
        if (!WidgetManager::GetInstance()->isExist(&manager))
        {
            manager = new EmotionManager();
        }
        manager->show();
        manager->activateWindow();
    });

    connect(fresh, &QPushButton::clicked, this, [=] () {
        tab->clearItems();
    });

    connect(lock, &QPushButton::clicked, this, [=] () {
        bool ok;
        QString str = InputBox::GetInput(cns("输入密码"), &ok,
                                         QLineEdit::Password);
        if (!ok)
            return;
        if (str != conf->getPassword())
        {
            MsgBox::ShowMsgBox(cns("错误"), cns("密码错误"), cns("确定"));
            return;
        }
        LockWidget *lock = new LockWidget();
        lock->show();
        lock->activateWindow();
    });

    connect(about, &QPushButton::clicked, this, [=] () {
        AboutHP *hp = new AboutHP;
        hp->show();
        hp->activateWindow();
//        MuiltProgressBar *muilt = new MuiltProgressBar;
//        muilt->setFixedWidth(165);
//        muilt->setRange(0, 100);
//        muilt->setValue(30);
//        muilt->show();
//        muilt->activateWindow();
    });

    connect(search, &SearchWidget::searchTxtChanged,
            this, [=] (QString str) {
        if (str.trimmed().isEmpty())
            return;
        PupupWidget *pup = new PupupWidget;
        QListWidget *list = pup->list();
        QPoint p = search->mapToGlobal(search->pos());
        QHash<QString, ItemEditor *> *items = tab->returnItems();
        auto it = items->begin();
        while (it != items->end())
        {
            QString uid = it.key();
            ItemEditor *item = it.value();
            PersonItem *pie = dynamic_cast<PersonItem *>(item);
            QString name = pie->getNickname();
            QString memoname = pie->getMemoname();
            if (name.contains(str) || memoname.contains(str))
            {
                QListWidgetItem *it = new QListWidgetItem(list);
                it->setText(item->getName());
                it->setData(1, uid);
            }
            ++it;
        }
        connect(list, &QListWidget::itemClicked,
                this, [=] (QListWidgetItem *item) {
            QString uid = item->data(1).toString();
            if (items->contains(uid))
            {
                ItemEditor *item = items->value(uid);
                tab->signalPersonDClicked(item);
            }
            pup->close();
        });
        if (list->count() == 0)
        {
            pup->close();
            return;
        }
        pup->show();
        pup->setFixedHeight(100);
        pup->move(QPoint(p.x() - list->width() - 17, p.y() - 110));
    });

    connect(photo, SIGNAL(selfStatusChanged(int)),
            tab, SIGNAL(selfStatusChanged(int)));
    connect(photo, SIGNAL(selfFeelChanged(QString)),
            tab, SIGNAL(selfFeelChanged(QString)));

    int x = qApp->desktop()->width() - width();
    int y = 0;
    move(x, y);

    // load data
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

    HotkeyRegiter *hot = HotkeyRegiter::GetInstance(this);
    if (!hot->RegisterHotkey(0, conf->getIni("msg_key")))
    {
        NotifyUI::Show3sNotify(cn("弹出消息快捷键注册失败"));
    }
    if (!hot->RegisterHotkey(1, conf->getIni("shot_key")))
    {
        NotifyUI::Show3sNotify(cn("截图快捷键注册失败"));
    }
    if (!hot->RegisterHotkey(2, conf->getIni("lock_key")))
    {
        NotifyUI::Show3sNotify(cn("锁屏快捷键注册失败"));
    }

    QString defaultkey = conf->getIni("close_key");
    if (defaultkey.isEmpty())
    {
        defaultkey = "ALT+C";
        conf->setIni("close_key", defaultkey);
    }
    if (!hot->RegisterHotkey(3, defaultkey))
    {
        NotifyUI::Show3sNotify(cn("关闭窗口快捷键注册失败"));
    }

    defaultkey = conf->getIni("show_need_replay");
    if (defaultkey.isEmpty())
    {
        defaultkey = "ALT+S";
        conf->setIni("show_need_replay", defaultkey);
    }
    if (!hot->RegisterHotkey(4, defaultkey))
    {
        NotifyUI::Show3sNotify(cn("显示回复窗口快捷键注册失败"));
    }

    defaultkey = conf->getIni("hide_active");
    if (defaultkey.isEmpty())
    {
        defaultkey = "ALT+A";
        conf->setIni("hide_active", defaultkey);
    }
    if (!hot->RegisterHotkey(5, defaultkey))
    {
        NotifyUI::Show3sNotify(cn("隐藏聊天窗口快捷键注册失败"));
    }

    connect(hot, &HotkeyRegiter::hotkeyClicked, this, [=] () {
        hotkeyclicked();
        emit mTray->activated(QSystemTrayIcon::DoubleClick);
    });
    connect(hot, SIGNAL(screenshotClicked()), this, SLOT(screenshot()));

    connect(hot, &HotkeyRegiter::hotkeyLocked, this, [=] () {
        emit lock->clicked();
    });

    connect(hot, &HotkeyRegiter::hotKeyClose, this, [=] () {
        Chat *ui = nullptr;
        ui = dynamic_cast<Chat*>(qApp->activeWindow());
        if (ui)
        {
            ui->close();
        }
        else
        {
            if (WidgetManager::GetInstance()->popWidget(&ui))
            {
                if (ui)
                    ui->close();
            }
        }
    });

    connect(hot, &HotkeyRegiter::hotKeyShow, this, [=] () {
        if (!WidgetManager::GetInstance()->showReplayChat())
            WidgetManager::GetInstance()->showReplayChat(false);
    });

    connect(hot, &HotkeyRegiter::hotKeyHide, this, [=] () {
        Chat *ui = nullptr;
        ui = dynamic_cast<Chat*>(qApp->activeWindow());
        if (ui)
        {
            ui->showMinimized();
        }
        else
        {
            if (WidgetManager::GetInstance()->popWidget(&ui))
            {
                if (ui)
                    ui->showMinimized();
            }
        }
    });

    connect(this, &MainUI::updateUpdateInfo, this, [=] (QString info) {
        MsgBox::ShowBRbox(cn("警告"), cn("哈啤更新程序升级失败，可能会影响自动升级\n") + info);
    });

    // 10s write accept update info
    QTimer::singleShot(30000, this, [=] () {
        conf->setIni("accept_update", true);

    });
    QTimer::singleShot(1000, this, [=] () {
        QtConcurrent::run(this, &MainUI::updateUpdate);
    });

    bezierEnable = true;
    if (conf->IsTrue("bezier"))
    {
        bezierTime->setFrameRange(0, 250);
        bezierTime->start();
    }
}


void MainUI::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}


void MainUI::hideEvent(QHideEvent *)
{
    this->hide();
}


void MainUI::paintEvent(QPaintEvent *e)
{
    THWidgetBase::paintEvent(e);
}
