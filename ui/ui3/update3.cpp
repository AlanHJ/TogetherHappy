#include "update3.h"
#include "progressbar.h"
#include "../../util/thmethod.h"
#include "../../data/db/configdata.h"
#include "../../data/db/emoticondata.h"
#include "../../data/db/emoticongroupdata.h"
#include "../../data/db/groupdata.h"
#include "../../sql/sql.h"
#include "msgbox.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QFileInfo>
#include <QtConcurrent/QtConcurrent>

Update3::Update3(QWidget *parent) : THWidgetBase(parent)
{
    id = -1;
    timerId = -1;
    gradient = 0;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    hideTitleBar();

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(10, 20, 10, 20);
    v->setSpacing(0);

    QLabel *titleLbl = new QLabel(QStringLiteral("哈啤3.0更新"), this);
    titleLbl->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:14px;color:white;"));
    QLabel *contentLbl = new QLabel(this);
    contentLbl->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    contentLbl->setWordWrap(true);
    contentLbl->setText(QStringLiteral("哈啤3.0更新了大量内容，重做了全部UI，现在将对您的原有设置进行升级，并整理所有资源，请稍等片刻"));

    QLabel *tipsLbl = new QLabel(this);
    tipsLbl->setText(QStringLiteral("请稍后..."));
    tipsLbl->setFixedWidth(260);
    tipsLbl->setAlignment(Qt::AlignCenter);
    tipsLbl->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:10px;color:white;"));
    tipsLbl->setWordWrap(true);

    ProgressBar *bar = new ProgressBar(this);
    bar->setTextVisible(false);
    bar->setRange(0, 2000);
    bar->setValue(0);
    bar->setFixedSize(260, 12);

    v->addWidget(titleLbl, 0, Qt::AlignCenter);
    v->addWidget(contentLbl, 4, Qt::AlignCenter);
    v->addWidget(tipsLbl, 0, Qt::AlignCenter);
    v->addWidget(bar, 0, Qt::AlignCenter);

    setFixedSize(300, 200);

    QFile file;
    file.setFileName(":res/css/progressbar.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        bar->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    connect(this, &Update3::updateProgress,
            bar, [=] (int value) {
        bar->setValue(value);
        emit updateInformation(QString::number(value / 2000.0 * 100, 'f', 1) + "%");
    });
    connect(this, SIGNAL(updateInformation(QString)),
            tipsLbl, SLOT(setText(QString)));
    connect(this, &Update3::updateSuccess, this, [=] () {

        // delete old resource?

        id = 0;
        loop.exit();
        this->close();
        this->deleteLater();
    });
    connect(this, &Update3::updateFail, this, [=] (QString error) {
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        MsgBox *box = new MsgBox();
        box->setTitle(QStringLiteral("升级失败"));
        box->setContent(error);
        box->setButtons(QStringLiteral("退出"));
        box->raise();
        box->exec();
        loop.exit();
        this->close();
        this->deleteLater();
    });



    bar->showAnimation();
    QTimer::singleShot(1500, [=] () {
        // start update
        timerId = startTimer(60);
        QtConcurrent::run(this, &Update3::startUpdate);
    });

    qDebug() << QStringLiteral("升级哈啤");
}

Update3::~Update3()
{
    qDebug() << "Destruct Update3";
}

int Update3::exec()
{
    show();
    activateWindow();
    loop.exec(QEventLoop::DialogExec);
    return id;
}

void Update3::startUpdate()
{
    emit updateInformation(QStringLiteral("开始"));
    thread()->msleep(1000);

    QString error;
    // check database
    QString path = GetWorkPath() + "/database/data.db";
    if (!QFile::exists(path))
    {

        // copy a database to work path
        if (QDir().mkpath(GetWorkPath() + "/database") && !CpFile(":res/db/data.db", path, error))
        {
            emit updateInformation(QStringLiteral("数据库创建失败"));
            emit updateFail(QStringLiteral("数据库创建失败"));
            return;
        }

        // remove only read
//        QString cmd = "attrib -r " + QString("\"") + path + QString("\"");
//        system(cmd.toLocal8Bit().constData());
    }

    // create default values
    QHash<QString, QString> values;

    // update flag
    values.insert("update", "0");
    // current version
    values.insert("version", QString::number(HAPPY_VERSON));
    // default color1
    values.insert("color1", "255,118,118,230");
    // default color2
    values.insert("color2", "33,150,243,230");
    // default name
    QString username = cns("小白");
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables) {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1) {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2) {
                username = stringList.at(1).toUtf8();
                break;
            }
        }
    }
    // create uuid
    values.insert("uid", getUuid());
    values.insert("nickname", username);
    // default say
    values.insert("feel", "");
    // default status
    values.insert("status", QStringLiteral("在线"));
    // default chat ui size
    values.insert("chat_w", "586");
    values.insert("chat_h", "510");
    // default font
    values.insert("font_name", QStringLiteral("微软雅黑"));
    values.insert("font_size", "8");
    values.insert("font_bold", "0");
    values.insert("font_italic", "0");
    values.insert("font_underline", "0");
    values.insert("font_color", "255,255,255");
    // default auto start
    values.insert("auto_start", "1");
    // default local ip
    values.insert("ip_get", "1");

    // auto get ip
    QString ipStr = "";
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
                        qDebug() << face.hardwareAddress(); // MAC address
                        ipStr = entry.ip().toString();
                        break;
                    }
                }
                break;
            }
        }
    }
    values.insert("ip", ipStr);
    // default notify online
    values.insert("notify_online", "0");
    // default photo
    values.insert("photo", ":res/baidu/15.png");
    // default image
    values.insert("image", ":res/ui3/show_default.gif");
    // default show what name
    values.insert("show_name", "0");
    // default show myself
    values.insert("show_self", "1");
    // default open emoticon preview
    values.insert("open_preview", "1");
    // default shield shake
    values.insert("shake_shield", "0");
    // default shield shake time /s
    values.insert("shake_shield_time", "30");
    // default shortcut key
    // 1.send key
    values.insert("send_key", "enter");
    // 2.screenshot key
    values.insert("shot_key", "ctrl+alt+z");
    // 3.msg show key
    values.insert("msg_key", "alt+z");
    // 4.lock key
    values.insert("lock_key", "alt+l");
    // key enable
    values.insert("key_enable", "1");
    // open log
    values.insert("log", "1");
    // load status
    values.insert("load_status", "0");
    // filter image size Kb
    values.insert("filter_image", "5");
    // delete image
    values.insert("delete_image", "0");
    // lock password
    QString pw = "123456";
    values.insert("pw", QString::fromLocal8Bit(pw.toLocal8Bit().toBase64()));

    emit updateProgress(10);

    // check exists resource
    QString emoticonPath;
    QString oldIni = "D:/Program Files/happy/config.ini";
    if (QFile::exists(oldIni))
    {
        QSettings set(oldIni, QSettings::IniFormat);
        set.setIniCodec("UTF-8");
        for(QString k : set.allKeys())
        {
            QString v = set.value(k).toString();
            if (k.contains("name"))
            {
                values.insert("nickname", v);
            }
            else if (k.contains("ipset"))
            {
                if (v == "1")
                {
                    values.insert("ip_set", "1");
                    values.insert("ip", set.value("run/ip").toString());
                }
            }
            else if (k.contains("path"))
            {
                emoticonPath = v;
            }
        }
    }

    bool updateb = true;

    // check emoticon
    QList<EmoticonData> emots;
    QList<EmoticonGroupData> groups;
    QString datetimestr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if (QFileInfo(emoticonPath).isDir())
    {
        // add emoticon deal

        //1.add all custom emoticon path
        QFile emoticonFile("D:/Program Files/happy/emoji.data");
        QStringList paths;
        if (emoticonFile.open(QIODevice::ReadOnly))
        {
            paths = QString::fromLocal8Bit(emoticonFile.readAll()).split(";");
        }
        emoticonFile.close();
        // calc MD5
        QHash<QString, QString> MD5s;
        for (int i = 0; i < paths.size(); i++)
        {
            QString path = paths.at(i);
            emit updateProgress(10 + i / (float)paths.size() * 340);
            if (path.isEmpty())
            {
                continue;
            }
            QFile f(path);
            if (!f.open(QIODevice::ReadOnly))
            {
                continue;
            }
            QByteArray ba = QCryptographicHash::hash(f.readAll(), QCryptographicHash::Md5);
            QString md5 = ba.toHex();
            if (MD5s.contains(md5))
            {
                // exists
                qDebug() << "exists:" << md5;
            }
            else
            {
                MD5s.insert(md5, path);
            }
            f.close();
        }

        // move image and scaled image create index
        // check custom file exists


        QString customPath = GetWorkPath() + "/custom";
        if (QDir().mkpath(customPath))
        {
            int counts = 0;
            auto it = MD5s.begin();
            while (it != MD5s.end())
            {
                emit updateProgress(350 + counts / (float)MD5s.size() * 200);
                counts++;
                QString md5 = it.key();
                QString path = it.value();
                QFileInfo f(path);
                qint64 size = f.size();
                EmoticonData data;
                if (size >= 1024 * 10)
                {
                    QImage image;
                    QImageReader reader(path);
                    if (reader.canRead())
                    {
                        image = reader.read();
                        image = image.scaled(50, 50);
                        QString suffix = f.suffix().compare("gif", Qt::CaseInsensitive) == 0 ? "jpg" : f.suffix();
                        QString thumbPath = customPath + "/" + md5 + "_50_50." + suffix;
                        bool b = image.save(thumbPath);
                        updateb &= b;
                        if (b)
                        {
                            // success
                            data.setThumb(thumbPath);
                        }
                        else
                        {
                            qDebug() << "scaled save failed! " << path;
                        }
                    }
                }
                QString newPath = customPath + "/" + md5 + "." + f.suffix();
                if (QFile::exists(newPath))
                {
                    data.setPath(newPath);
                    data.setName(md5);
                    data.setCounts(0);
                    data.setInserttime(datetimestr);
                    data.setLasttime(datetimestr);
                    data.setGroup(QStringLiteral("默认"));
                    data.setDeleteable(true);
                    data.setEditable(true);
                    data.setReadable(true);
                    emots.push_back(data);
                }
                else
                {
                    bool b = QFile::copy(path, newPath);
                    updateb &= b;
                    if (b)
                    {
                        // success
                        data.setPath(newPath);
                        data.setName(md5);
                        data.setCounts(0);
                        data.setInserttime(datetimestr);
                        data.setLasttime(datetimestr);
                        data.setGroup(QStringLiteral("默认"));
                        data.setDeleteable(true);
                        data.setEditable(true);
                        data.setReadable(true);
                        emots.push_back(data);
                    }
                    else
                    {
                        qDebug() << "scaled save failed! " << path;
                    }

                }
                ++it;
            }
        }
    }
    EmoticonGroupData group;
    group.setGroup(QStringLiteral("默认"));
    group.setDeleteable(false);
    group.setEditable(true);
    group.setReadable(true);
    groups.push_back(group);

    // add build-in resource
    QString emtPathStr = GetWorkPath() + "/custom/%1";
    QString emtNameStr = ":res/%1/%2.%3";
    QString newNameStr = "/%1.%2";
    // qq
    if (QDir().mkpath(emtPathStr.arg("qq1")))
    {
        for (int i = 0; i < 132; i++)
        {
            QString str1 = emtNameStr.arg("qqemoji").arg(i + 1).arg("gif");
            QString str2 = emtPathStr.arg("qq1") + newNameStr.arg(i + 1).arg("gif");
            bool b = QFile::copy(str1, str2);
            updateb &= b;
            if (b)
            {
                EmoticonData ed;
                ed.setPath(str2);
                ed.setName(QString("qq1_%1").arg(i + 1));
                ed.setCounts(0);
                ed.setInserttime(datetimestr);
                ed.setLasttime(datetimestr);
                ed.setGroup(QStringLiteral("QQ"));
                ed.setDeleteable(false);
                ed.setEditable(false);
                ed.setReadable(true);
                emots.push_back(ed);
            }
            emit updateProgress(550 + i / (float)132 * 100);
        }
        EmoticonGroupData group;
        group.setGroup(QStringLiteral("QQ"));
        group.setDeleteable(false);
        group.setEditable(false);
        group.setReadable(true);

        groups.push_back(group);
    }
    // mhxy
    if (QDir().mkpath(emtPathStr.arg("mhxy")))
    {
        for (int i = 0; i < 210; i++)
        {
            QString str1 = emtNameStr.arg("mhxy").arg(i + 1).arg("gif");
            QString str2 = emtPathStr.arg("mhxy") + newNameStr.arg(i + 1).arg("gif");
            bool b = QFile::copy(str1, str2);
            updateb &= b;
            if (b)
            {
                EmoticonData ed;
                ed.setPath(str2);
                ed.setName(QString("mhxy_%1").arg(i + 1));
                ed.setCounts(0);
                ed.setInserttime(datetimestr);
                ed.setLasttime(datetimestr);
                ed.setGroup(QStringLiteral("梦幻"));
                ed.setDeleteable(false);
                ed.setEditable(false);
                ed.setReadable(true);
                emots.push_back(ed);
            }
            emit updateProgress(650 + i / (float)210 * 100);
        }
        EmoticonGroupData group;
        group.setGroup(QStringLiteral("梦幻"));
        group.setDeleteable(false);
        group.setEditable(false);
        group.setReadable(true);
        groups.push_back(group);
    }
    // baidu
    if (QDir().mkpath(emtPathStr.arg("baidu")))
    {
        for (int i = 0; i < 72; i++)
        {
            QString str1 = emtNameStr.arg("baidu").arg(i + 1).arg("png");
            QString str2 = emtPathStr.arg("baidu") + newNameStr.arg(i + 1).arg("png");
            bool b = QFile::copy(str1, str2);
            updateb &= b;
            if (b)
            {
                EmoticonData ed;
                ed.setPath(str2);
                ed.setName(QString("baidu_%1").arg(i + 1));
                ed.setCounts(0);
                ed.setInserttime(datetimestr);
                ed.setLasttime(datetimestr);
                ed.setGroup(QStringLiteral("百度"));
                ed.setDeleteable(false);
                ed.setEditable(false);
                ed.setReadable(true);
                emots.push_back(ed);
            }
            emit updateProgress(750 + i / (float)72 * 100);
        }
        EmoticonGroupData group;
        group.setGroup(QStringLiteral("百度"));
        group.setDeleteable(false);
        group.setEditable(false);
        group.setReadable(true);
        groups.push_back(group);
    }
    // huaji
    if (QDir().mkpath(emtPathStr.arg("huaji")))
    {
        for (int i = 0; i < 64; i++)
        {
            QString str1 = emtNameStr.arg("huaji").arg(i + 1).arg("png");
            QString str2 = emtPathStr.arg("huaji") + newNameStr.arg(i + 1).arg("png");
            bool b = QFile::copy(str1, str2);
            updateb &= b;
            if (b)
            {
                EmoticonData ed;
                ed.setPath(str2);
                ed.setName(QString("huaji_%1").arg(i + 1));
                ed.setCounts(0);
                ed.setInserttime(datetimestr);
                ed.setLasttime(datetimestr);
                ed.setGroup(QStringLiteral("滑稽"));
                ed.setDeleteable(false);
                ed.setEditable(false);
                ed.setReadable(true);
                emots.push_back(ed);
            }
            emit updateProgress(850 + i / (float)64 * 100);
        }
        EmoticonGroupData group;
        group.setGroup(QStringLiteral("滑稽"));
        group.setDeleteable(false);
        group.setEditable(false);
        group.setReadable(true);
        groups.push_back(group);
    }
    // new QQ
    if (QDir().mkpath(emtPathStr.arg("newqq")))
    {
        for (int i = 0; i < 109; i++)
        {
            QString str1 = emtNameStr.arg("newqq").arg(i + 1).arg("png");
            QString str2 = emtPathStr.arg("newqq") + newNameStr.arg(i + 1).arg("png");
            bool b = QFile::copy(str1, str2);
            updateb &= b;
            if (b)
            {
                EmoticonData ed;
                ed.setPath(str2);
                ed.setName(QString("newqq_%1").arg(i + 1));
                ed.setCounts(0);
                ed.setInserttime(datetimestr);
                ed.setLasttime(datetimestr);
                ed.setGroup(QStringLiteral("新QQ"));
                ed.setDeleteable(false);
                ed.setEditable(false);
                ed.setReadable(true);
                emots.push_back(ed);
            }
            emit updateProgress(950 + i / (float)109 * 100);
        }
        EmoticonGroupData group;
        group.setGroup(QStringLiteral("新QQ"));
        group.setDeleteable(false);
        group.setEditable(false);
        group.setReadable(true);
        groups.push_back(group);
    }
    // money
    if (QDir().mkpath(emtPathStr.arg("money")))
    {
        for (int i = 0; i < 20; i++)
        {
            QString str1 = emtNameStr.arg("money").arg(i + 1).arg("png");
            QString str2 = emtPathStr.arg("money") + newNameStr.arg(i + 1).arg("png");
            bool b = QFile::copy(str1, str2);
            updateb &= b;
            if (b)
            {
                EmoticonData ed;
                ed.setPath(str2);
                ed.setName(QString("money_%1").arg(i + 1));
                ed.setCounts(0);
                ed.setInserttime(datetimestr);
                ed.setLasttime(datetimestr);
                ed.setGroup(QStringLiteral("PY"));
                ed.setDeleteable(false);
                ed.setEditable(false);
                ed.setReadable(true);
                emots.push_back(ed);
            }
            emit updateProgress(1050 + i / (float)20 * 100);
        }
        EmoticonGroupData group;
        group.setGroup(QStringLiteral("PY"));
        group.setDeleteable(false);
        group.setEditable(false);
        group.setReadable(true);
        groups.push_back(group);
    }

     int counts = 0;
    QList<ConfigData> confs;
    values.insert("update", "1");
    auto it = values.begin();
    while (it != values.end())
    {
        QString k = it.key();
        QString v = it.value();
        ConfigData cd;
        cd.setKey(k);
        cd.setValue(v);
        confs.push_back(cd);
        emit updateProgress(1150 + counts / (float)values.size() * 50);
        counts++;
        it++;
    }

    // write to database

    // connect database
    Sql::instance()->setDatabaseInfo("QSQLITE", "127.0.0.1", 3306,
                                     path, "user", "pw",
                                     2, 1, "SELECT UUID()");

    SqlDB *db = Sql::instance()->open(error);
    if (!db)
    {
        emit updateInformation(QStringLiteral("无法打开数据库"));
        emit updateFail(QStringLiteral("无法打开数据库\n") + error);
        return;
    }

    // insert configdata
    counts = 0;
    auto cit = confs.begin();
    while (cit != confs.end())
    {
        ConfigData cd = *cit;
        emit updateProgress(1200 + counts / (float)confs.size() * 50);
        counts++;
        ++cit;
        bool b = db->insert(&cd, error);
        updateb &= b;
        if (!b)
        {
            qDebug() << QStringLiteral("插入数据失败：") << error << endl
                     << cd.getKey() << " " << cd.getValue();
        }
    }

    // insert emoticon
    counts = 0;
    auto eit = emots.begin();
    while (eit != emots.end())
    {
        EmoticonData ed = *eit;
        emit updateProgress(1250 + counts / (float)emots.size() * 700);
        counts++;
        ++eit;
        bool b = db->insert(&ed, error);
        updateb &= b;
        if (!b)
        {
            qDebug() << QStringLiteral("插入数据失败：") << error << endl
                     << ed.getName() << " " << ed.getPath();
        }
    }

    // insert group
    counts = 0;
    auto git = groups.begin();
    while (git != groups.end())
    {
        EmoticonGroupData gd = *git;
        emit updateProgress(1950 + counts / (float)groups.size() * 50);
        counts++;
        ++git;
        bool b = db->insert(&gd, error);
        updateb &= b;
        if (!b)
        {
            qDebug() << QStringLiteral("插入数据失败：") << error << endl
                     << gd.getGroup();
        }
    }

    // inser chat group for all
    GroupData god;
    god.setUid(HAPPY_GROUP_BROADCASR_UID);
    god.setName(cn("全体成员"));
    god.setPhoto(":res/ui3/group_default.gif");
    god.setDeleteable(false);
    god.setEditable(false);
    god.setCreationtime(datetimestr);
    god.setUpdatetime(0);
    god.setCreator(cn("哈啤3.0"));
    god.setGroupdescribe(cn("来啊，一起哈啤!"));
    god.setGroupmember("");  // empty 表示all
    god.setAttr(cn("讨论组"));
    god.setNetmask(HAPPY_GROUP_BROADCASR_ADDRESS);
    god.setPort(HAPPY_GROUP_BROADCASR_PORT);
    bool b = db->insert(&god, error);
    if (!b)
    {
        qDebug() << QStringLiteral("插入数据失败：") << error << endl
                 << god.getName();
    }
    updateb &= b;

    db->close();

//    for (int i = 0; i <= 1000; i++)
//    {
//        emit updateInformation(QString::number(i / 1000.0 * 100, 'f', 1) + "%");
//        emit updateProgress(i);
//        thread()->msleep(10);
//    }

    emit updateProgress(2000);

    QString str = QStringLiteral("升级成功");
    if (!updateb)
        str = QStringLiteral("升级完成，但并没有完全成功");
    emit updateInformation(str + QStringLiteral("，3秒后进入主界面"));
    thread()->msleep(1000);
    emit updateInformation(str + QStringLiteral("，2秒后进入主界面"));
    thread()->msleep(1000);
    emit updateInformation(str + QStringLiteral("，1秒后进入主界面"));
    thread()->msleep(1000);
    emit updateInformation(str + QStringLiteral("，0秒后进入主界面"));
    emit updateSuccess();
}



void Update3::paintEvent(QPaintEvent *e)
{
    THWidgetBase::paintEvent(e);
    QPainter p(this);
    QPainterPath path;
    int w = this->width();
    int h = this->height();
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
    path.addRoundedRect(QRect(5, 5, w - 10, h - 10), 3, 3);
    QColor color(Qt::transparent);
    QPoint p1(5, 5);
    QPoint p2(w - 10, h - 10);
    QLinearGradient linecolor(p1, p2);
    linecolor.setColorAt(0, color);
    linecolor.setColorAt(1, color);
    linecolor.setColorAt(gradient - 0.15 <= 0 ? 0 : gradient - 0.15, color);
    linecolor.setColorAt(gradient >= 1 ? 1 : gradient, QColor(230, 230, 230, 50));
    linecolor.setColorAt(gradient + 0.15 >= 1 ? 1 : gradient + 0.15, color);
    p.setPen(Qt::transparent);
    p.fillPath(path, QBrush(linecolor));
}


void Update3::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerId)
    {
        if (gradient >= 1)
        {
            gradient = 0;
        }
        update();
        gradient = gradient + 0.05;
    }
}
