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
        MsgBox::ShowMsgBox(cn("����"),
                           cn("�޷�����"),
                           cn("ȷ��"));
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
    // �ӷ�������ȡ�°汾
    mTray->showMessage(cns("������Ϣ"), cns("��ʼ���"), QSystemTrayIcon::Information, 1000);
}

void MainUI::updateInfo(QString info)
{
    mTray->showMessage(cns("������Ϣ"), info, QSystemTrayIcon::Information, 1000);
    if (info == cns("���³ɹ�"))
    {
        // ������Ӧ����ɸ���
        mTray->showMessage(cns("������Ϣ"), cns("���³ɹ�, 5����������"), QSystemTrayIcon::Information, 1000);
        thread()->msleep(1000);

//        QString path = "D:/Program Files/happy/";
//        path.replace("/", "\\");

        bool b = QProcess::startDetached("UpdateHappy.exe");
        if (b == false)
        {
            mTray->showMessage(cns("������Ϣ"), cns("����ʧ��"), QSystemTrayIcon::Information, 1000);
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
        emit updateUpdateInfo(cn("�ͷ��µ���������ʧ��"));
    }
}

void MainUI::clearTemp()
{
    // ��ջ���
    HappyProgressUI *ui = new HappyProgressUI(this);
    ClearTempOpt *opt = new ClearTempOpt();
    ui->setShowName(cns("����ɾ����Ļ����ļ�"));
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
    // ������ʾ
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
//            QMessageBox::information(this, cns("���ڹ�ơ") + QString::number(HAPPY_VERSON),
//                                     QString::fromLocal8Bit(ba).arg(HAPPY_VERSON), cns("��"));
//        }
//    }
}

void MainUI::init()
{
    // ��ʼ������
    // ����dock��ͼ��
    mTray = new QSystemTrayIcon(this);

    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    photo = new HeaderPhotoUI(this);
    photo->setStyleSheet("QWitget{background:yellow;}");
    SearchWidget *search = new SearchWidget(this);

    SelfTabWidget *tab = new SelfTabWidget(mTray, this);
    // �������úͱ������ť
    // ����ȫ��֪ͨlabel���Ժ��Ϊ�������
    SwingLabel *notifyico = new SwingLabel(this);
    notifyico->setToolTip(QStringLiteral("��ơ����"));
//    notifyico->setStyleSheet("QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}");
    notifyico->setFixedSize(15, 15);
    notifyico->setImagePath(":res/ui3/notify.png");
//    notifyico->setPixmap(QPixmap(":res/ui3/notify.png").scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel *notify = new QLabel(QStringLiteral("��ӭʹ�ù�ơ3.0"), this);
    notify->setContentsMargins(0, 0, 0, 0);
    notify->setAlignment(Qt::AlignLeft);
    notify->setFixedWidth(160);
    notify->setStyleSheet(cns("QLabel{font-family:΢���ź�;font:12px;color:white;"
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
    options->setToolTip(QStringLiteral("����"));
    options->setFixedSize(25, 25);
    QPushButton *emojis = new QPushButton(this);
    emojis->setToolTip(QStringLiteral("�������"));
    emojis->setFixedSize(25, 25);
    QPushButton *resource = new QPushButton(this);
    resource->setToolTip(QStringLiteral("��Դ����"));
    resource->setFixedSize(25, 25);
    QPushButton *plug = new QPushButton(this);
    plug->setToolTip(QStringLiteral("��չ����"));
    plug->setFixedSize(25, 25);
    RotateButton *fresh = new RotateButton(this);
    fresh->setImagePath(":res/ui3/refresh");
    fresh->setToolTip(QStringLiteral("ˢ���б�"));
    fresh->setFixedSize(25, 25);
    QPushButton *lock = new QPushButton(this);
    lock->setToolTip(QStringLiteral("����"));
    lock->setFixedSize(25, 25);
    about = new QPushButton(this);
    about->setToolTip(QStringLiteral("����"));
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
    mTray->setToolTip(cns("����,һ���ơ"));
    SelfMenu *menu = new SelfMenu(this);
    QAction *quita = new QAction(cns("�˳�"), this);
    connect(quita, &QAction::triggered, this, [=] () {
        emit bar->signalClose();
    });
    QAction *seta = new QAction(cns("����"), this);
    connect(seta, &QAction::triggered, this, [=] () {
        emit options->clicked();
    });
//    QAction *cleara = new QAction(cns("��ջ���"), this);
//    connect(cleara, SIGNAL(triggered()), this, SLOT(clearTemp()));
    QAction *abouta = new QAction(cns("����"), this);
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
    QAction *update = new QAction(cns("������"), this);
    connect(update, &QAction::triggered, this, [=] () {
        // check update
        tab->updateCheck();
    });

    QAction *restart = new QAction(cns("������ơ"), this);
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
    bar->setBarContent(cn("��ơ") +
                       QString::number(HAPPY_VERSON));

    connect(bar, &HappyTitleBar::signalMinimize, this, &QWidget::hide);

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        MsgBox *box = new MsgBox();
        box->setTitle(QStringLiteral("��ơ��ʾ"));
        box->setContent(QStringLiteral("ȷ���رչ�ơô"));
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
        QString str = InputBox::GetInput(cns("��������"), &ok,
                                         QLineEdit::Password);
        if (!ok)
            return;
        if (str != conf->getPassword())
        {
            MsgBox::ShowMsgBox(cns("����"), cns("�������"), cns("ȷ��"));
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
        NotifyUI::Show3sNotify(cn("������Ϣ��ݼ�ע��ʧ��"));
    }
    if (!hot->RegisterHotkey(1, conf->getIni("shot_key")))
    {
        NotifyUI::Show3sNotify(cn("��ͼ��ݼ�ע��ʧ��"));
    }
    if (!hot->RegisterHotkey(2, conf->getIni("lock_key")))
    {
        NotifyUI::Show3sNotify(cn("������ݼ�ע��ʧ��"));
    }

    QString defaultkey = conf->getIni("close_key");
    if (defaultkey.isEmpty())
    {
        defaultkey = "ALT+C";
        conf->setIni("close_key", defaultkey);
    }
    if (!hot->RegisterHotkey(3, defaultkey))
    {
        NotifyUI::Show3sNotify(cn("�رմ��ڿ�ݼ�ע��ʧ��"));
    }

    defaultkey = conf->getIni("show_need_replay");
    if (defaultkey.isEmpty())
    {
        defaultkey = "ALT+S";
        conf->setIni("show_need_replay", defaultkey);
    }
    if (!hot->RegisterHotkey(4, defaultkey))
    {
        NotifyUI::Show3sNotify(cn("��ʾ�ظ����ڿ�ݼ�ע��ʧ��"));
    }

    defaultkey = conf->getIni("hide_active");
    if (defaultkey.isEmpty())
    {
        defaultkey = "ALT+A";
        conf->setIni("hide_active", defaultkey);
    }
    if (!hot->RegisterHotkey(5, defaultkey))
    {
        NotifyUI::Show3sNotify(cn("�������촰�ڿ�ݼ�ע��ʧ��"));
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
        MsgBox::ShowBRbox(cn("����"), cn("��ơ���³�������ʧ�ܣ����ܻ�Ӱ���Զ�����\n") + info);
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
