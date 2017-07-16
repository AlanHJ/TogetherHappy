#include "setmanager.h"
#include "../config/configuredata.h"
#include <QStandardPaths>
#include "../sql/sql.h"
#include "../util/thmethod.h"
#include "../data/db/configdata.h"
#include "../data/db/emoticondata.h"

SetManager::SetManager()
{

}

SetManager::~SetManager()
{

}



void SetManager::run()
{
    // 检查是否存在数据库文件
    // 数据库文件名：thdata.db 使用路径为用户的AppData目录下
    QString datapath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir;
    dir.mkpath(datapath);

    if (QFile::exists(datapath + "/thdata.db") == false)
    {
        // thdata.db is not exists
        // save new thdata.db
        QFile file(":/res/db/thdata.db");
        if (file.copy(datapath + "/thdata.db") == false)
        {
            emit signalResouceError("save new database fail");
            return;
        }
    }
    // 设置数据库为读写权限
    QString cmd = "attrib -r " + datapath + "/thdata.db";

    system(cmd.toLocal8Bit().constData());
    Sql::instance()->setDatabaseInfo("QSQLITE", "127.0.0.1", 3306,
                                     datapath + "/thdata.db", "user", "pw",
                                     2, 1, "SELECT UUID()");
        // get db information

        // update information

    // if ?D:\Program Files\happy\config.ini exists and version level lower
    QString inipath = "D:/Program Files/happy/config.ini";
    if (QFile::exists(inipath) && HAPPY_VERSON <= 2.47)
    {
        QList<ConfigData*> lst;
        QSettings set(inipath, QSettings::IniFormat);
        set.setIniCodec("UTF-8");
        foreach (QString k, set.allKeys())
        {
            if (k.contains("path") ||
                k.contains("app") ||
                k.contains("update"))
            {
                continue;
            }
            QString v = set.value(k).toString();
            ConfigData *cd = new ConfigData();
            cd->setKey(k.remove("RUN/"));
            cd->setValue(v);
            lst.append(cd);
        }
        QString error;
        SqlDB *db = Sql::instance()->open(error);
        if (nullptr == db)
        {
            emit signalResouceError(error);
            return;
        }
        auto it = lst.begin();
        while (it != lst.end())
        {
            ConfigData *cd = *it;
            if (!db->insert(cd, error))
            {
                emit signalResouceError(error);
            }
            delete cd;
            cd = nullptr;
            ++it;
        }
        db->close();
        lst.clear();
        if (QFile::remove(inipath))
        {
            emit signalResouceError("remove error");
        }
    }


    // 设置默认自启动
//    int autoset;
//    ConfigureData::getInstance()->getIni("autoset", autoset);
//    if (autoset != 1)
    {
//        ConfigureData::getInstance()->writeIni("autoset", 1);
//        ConfigureData::getInstance()->setAutoStart(true);
    }

    // 解压更新程序

    QFile file(":/res/update/UpdateHappy.exe");
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();
    QFile file2("UpdateHappy.exe");
    if (file2.exists())
    {
        file2.remove();
    }
    file2.open(QIODevice::WriteOnly);
    file2.write(ba);
    file2.close();


    // 是否解压表情包


    // QQ
    int ret = 0;
//    ConfigureData::getInstance()->getIni("qq", ret);
    QString path;
//    ConfigureData::getInstance()->getIni("path", path);
    if (ret != 1)
    {
        // 解压表情包
        QDir dir;
        dir.mkpath(path + "/" + "images/res/qqemoji");
        QString savepath = path + "/images/res/qqemoji/%1.gif";
        QString emojipath = ":/res/qqemoji/%1.gif";
        for (int i = 0; i < 132; i++)
        {
            QString savename = savepath.arg(i + 1);
            QString filename = emojipath.arg(i + 1);
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = file.readAll();
                file.close();
                QFile newfile(savename);
                if (newfile.open(QIODevice::WriteOnly))
                {
                    newfile.write(ba);
                    newfile.close();
                }
            }
        }
//        ConfigureData::getInstance()->writeIni("qq", 1);
    }
    // 梦幻西游
    ret = 0;
//    ConfigureData::getInstance()->getIni("mhxy", ret);
    if (ret != 1)
    {
        // 解压表情包
        QDir dir;
        dir.mkpath(path + "/" + "images/res/mhxy");
        QString savepath = path + "/images/res/mhxy/%1.gif";
        QString emojipath = ":/res/mhxy/%1.gif";
        for (int i = 1; i <= 210; i++)
        {
            QString savename = savepath.arg(i);
            QString filename = emojipath.arg(i);
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = file.readAll();
                file.close();
                QFile newfile(savename);
                if (newfile.open(QIODevice::WriteOnly))
                {
                    newfile.write(ba);
                    newfile.close();
                }
            }
        }
//        ConfigureData::getInstance()->writeIni("mhxy", 1);
    }

    // 贴吧
    ret = 0;
//    ConfigureData::getInstance()->getIni("tieba", ret);
    if (ret != 1)
    {
        // 解压表情包
        QDir dir;
        dir.mkpath(path + "/" + "images/res/baidu");
        QString savepath = path + "/images/res/baidu/%1.png";
        QString emojipath = ":/res/baidu/%1.png";
        for (int i = 1; i <= 72; i++)
        {
            QString savename = savepath.arg(i);
            QString filename = emojipath.arg(i);
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = file.readAll();
                file.close();
                QFile newfile(savename);
                if (newfile.open(QIODevice::WriteOnly))
                {
                    newfile.write(ba);
                    newfile.close();
                }
            }
        }
//        ConfigureData::getInstance()->writeIni("tieba", 1);
    }

    // 滑稽
    ret = 0;
//    ConfigureData::getInstance()->getIni("huaji", ret);
    if (ret != 1)
    {
        // 解压表情包
        QDir dir;
        dir.mkpath(path + "/" + "images/res/huaji");
        QString savepath = path + "/images/res/huaji/%1.png";
        QString emojipath = ":/res/huaji/%1.png";
        for (int i = 1; i <= 64; i++)
        {
            QString savename = savepath.arg(i);
            QString filename = emojipath.arg(i);
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = file.readAll();
                file.close();
                QFile newfile(savename);
                if (newfile.open(QIODevice::WriteOnly))
                {
                    newfile.write(ba);
                    newfile.close();
                }
            }
        }
//        ConfigureData::getInstance()->writeIni("huaji", 1);
    }

    // 新QQ
    ret = 0;
//    ConfigureData::getInstance()->getIni("newqq", ret);
    if (ret != 1)
    {
        // 解压表情包
        QDir dir;
        dir.mkpath(path + "/" + "images/res/newqq");
        QString savepath = path + "/images/res/newqq/%1.png";
        QString emojipath = ":/res/newqq/%1.png";
        for (int i = 1; i <= 109; i++)
        {
            QString savename = savepath.arg(i);
            QString filename = emojipath.arg(i);
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = file.readAll();
                file.close();
                QFile newfile(savename);
                if (newfile.open(QIODevice::WriteOnly))
                {
                    newfile.write(ba);
                    newfile.close();
                }
            }
        }
//        ConfigureData::getInstance()->writeIni("newqq", 1);
    }

    // 打赏
    ret = 0;
//    ConfigureData::getInstance()->getIni("money", ret);
    if (ret != 1)
    {
        // 解压表情包
        QDir dir;
        dir.mkpath(path + "/" + "images/res/money");
        QString savepath = path + "/images/res/money/%1.png";
        QString emojipath = ":/res/money/%1.png";
        for (int i = 1; i <= 20; i++)
        {
            QString savename = savepath.arg(i);
            QString filename = emojipath.arg(i);
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray ba = file.readAll();
                file.close();
                QFile newfile(savename);
                if (newfile.open(QIODevice::WriteOnly))
                {
                    newfile.write(ba);
                    newfile.close();
                }
            }
        }
//        ConfigureData::getInstance()->writeIni("money", 1);
    }
}
