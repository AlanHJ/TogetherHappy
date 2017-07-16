#ifndef THMETHOD
#define THMETHOD

#include <QDir>
#include <QUrl>
#include <QUuid>
#include <QFile>
#include <QColor>
#include <QTime>
#include <QWidget>
#include <QString>
#include <QFileInfo>
#include <QSettings>
#include <QSqlQuery>
#include <QTextCodec>
#include <QApplication>
#include <QSqlDatabase>
#include <QDomDocument>
#include <QStandardPaths>
#include <QDesktopWidget>
#include <QRegularExpression>

#define VERSION "哈啤3.20"
#define HAPPYURL "www.togetherhappy.cn"
#define HAPPY_VERSON 3.20f
#define BroadcastInterval = 2000;
#define HAPPY_DEFULT_BROADCASR_ADDRESS "239.110.120.119"
#define HAPPY_DEFULT_BROADCASR_PORT 24668
#define HAPPY_GROUP_BROADCASR_ADDRESS "239.110.119.1"
#define HAPPY_GROUP_BROADCASR_PORT 24661
#define HAPPY_GROUP_BROADCASR_UID "5f0f7705fc4a446291180639a21c13a3"
#define HAPPY_LISTEN_PORT 24666
#define HAPPY_GOASP_PORT 24569

#define FRAME_START_TAG 0xEEEEEEEE  //帧头
#define FRAME_MAX_LENGTH (1024 * 1024 * 2)     //数据帧的最大长度
#define UDP_SEND_DATA_SIZE 8192
#define ShadowWidth 5       // 绘制阴影的宽度


#define UI_DESIGN

#ifndef		IN
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef		INOUT
#define		INOUT
#endif

typedef char 					I8;
typedef unsigned char           U8;
typedef short int				I16;
typedef unsigned short int      U16;
typedef int						I32;
typedef unsigned int			U32;
typedef __int64 				I64;
typedef unsigned __int64        U64;
typedef float					F32;
typedef double					F64;
typedef unsigned char           byte;

#define MAX_QUEUE_LENGTH        (1024 * 1024 * 10)

#define cn(str) QTextCodec::codecForLocale()->toUnicode(str)

static QTime StartTime = QTime(8, 00);
static QTime EndTime = QTime(17, 30);

enum MessageType
{
    undefine,
    Hello,
    Greeting,
    Ping,
    Pong,
    RichText,
    FileSendType,
    FileHead,
    Invite,
    update,
    Json,
    FilePrev,
    commonStr,
    FileInfo,
};

static QSize DefaultSize = QSize(22, 22);

struct ParserImage
{
    qint16 position;
    QString path;
};

static QSize CalcStrByFont(QString str, int fs = 14,
                           QString ff = QTextCodec::codecForLocale()->toUnicode("微软雅黑"))
{
    QSize size;
    QFont font(ff);
    font.setPointSize(fs);
    QFontMetrics metrics(font);
    size = metrics.size(Qt::TextExpandTabs , str);
    return size;
}

static QString GetWorkPath()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dataPath);
    dir.cdUp();
    QString ininame = dir.absolutePath() + "/TogetherHappy/config.ini";
    QSettings set(ininame, QSettings::IniFormat);
    set.setIniCodec("UTF-8");
    return set.value("run/work").toString();
}

static bool IsNeedFristRun()
{
    bool b = true;
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dataPath);
    dir.cdUp();
    QString ininame = dir.absolutePath() + "/TogetherHappy/config.ini";
    if (QFile::exists(ininame))
    {
        QSettings set(ininame, QSettings::IniFormat);
        set.setIniCodec("UTF-8");
        QString str = set.value("run/work").toString();
        b = !QFileInfo(str).isDir();
    }
    return b;
}

static bool IsNeedUpdate()
{
    bool b = true;
    QString path = GetWorkPath() + "/database/data.db";
    if (QFile::exists(path))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"/*, "staticdb1"*/);
        db.setDatabaseName(path);
        if (db.open())
        {
            QString sql = "SELECT value From config WHERE key='update'";
            QSqlQuery query(db);
            if (query.exec(sql))
            {
                if (query.next())
                {
                    int i = query.value(0).toString().toInt();
                    b = !(bool)i;
                }
            }
        }
        db.close();
//        QSqlDatabase::removeDatabase("staticdb1");
    }
    return b;
}

static QStringList ParserHtmlImgSrc(QString html)
{
    QStringList srclst;
    QDomDocument doc;
    if(!doc.setContent(html.toUtf8(), false))//html 存储在byteArr中，QByteArray类型。
    {
        return srclst;
    }

    QDomElement root = doc.documentElement();
    QDomNodeList nodelist=root.elementsByTagName("img");//调用Qt自带的函数提取全部的链接
    if (nodelist.isEmpty())
    {
        return srclst;
    }
    for (int i=0; i<nodelist.size(); i++)
    {
        QString src = nodelist.at(i).toElement().attribute("src");
        QString temp = QUrl(src).path();
        QString text = src;
        if (temp.contains(":/"))
        {
            text = temp.right(temp.length() - 1);
        }
        srclst << text;
    }
    return srclst;
}

static void ResetHtmlImgSrc(QString &html, QList<ParserImage> lst)
{
    QDomDocument doc;
    QList<ParserImage> srclst;
    if(!doc.setContent(html.toUtf8(), false))//html 存储在byteArr中，QByteArray类型。
    {
        return ;
    }

    QDomElement root = doc.documentElement();
    QDomNodeList nodelist=root.elementsByTagName("img");//调用Qt自带的函数提取全部的链接
    if (nodelist.isEmpty())
    {
        return;
    }
    for (int i=0; i<nodelist.size(); i++)
    {
        QString src = nodelist.at(i).toElement().attribute("src");
//        src = src.right(src.length() - 8);
        ParserImage imagesrc;
        imagesrc.position = i;
        imagesrc.path = src;
        srclst.push_back(imagesrc);
    }
    for (int i = 0; i < srclst.size(); i++)
    {
        ParserImage src = srclst.at(i);
        for (int j = 0; j < lst.size(); j++)
        {
            ParserImage replace = lst.at(j);
            if (src.position == replace.position)
            {
                // 替换
                html.replace(src.path, replace.path);
            }
        }
    }
}

static QString ImagePathToHtml(QString path)
{
    if (path.contains("://"))
    {
        path = path.replace("://", ":/");
    }
    QString html = QString("<html><img src=\"%1\"/></html>").arg(path);
    return html;
}

static QString ImagePathToFileHtml(QString path)
{
    QString html = QString("<html><img src=\"file:///%1\"/></html>").arg(path);
    return html;
}


static QString cns(const char *c)
{
    return QTextCodec::codecForLocale()->toUnicode(c);
}

static void movecenter(QWidget *wg)
{
    QDesktopWidget desk;
    int w = desk.width();
    int h = desk.height();
    int x1 = (w - wg->width()) / 2;
    int y1 = (h - wg->height()) / 2;
    wg->move(x1, y1);
}

static void MovePcenter(QWidget *p, QWidget *c)
{
    int w = p->width();
    int h = p->height();
    int x1 = p->x() + (w - c->width()) / 2;
    int y1 = p->y() + (h - c->height()) / 2;
    c->move(x1, y1);
}

static QString getUuid()
{
    QString uuid =  QUuid::createUuid().toString()
            .remove(QRegularExpression("[{}]"));
    return uuid.remove("-");
}

static QByteArray WStringDecode(std::wstring wstr)
{
    QString temp = QString::fromStdWString(wstr);

//    QByteArray ba;
//    for(int i=0;i<temp.length()/2;i++)
//    {
//        QString str = temp.mid(i*2,2);
//        bool ok=false;
//        char a = str.toShort(&ok,16) & 0xFF;
//        if( ok == false )
//        {
//        }
//        ba.append(a);
//    }
//    return ba;

    // 内存增长
    QByteArray ba = temp.toUtf8();
    QByteArray ba64 = QByteArray::fromBase64(ba);
    return ba64;
}

static QString QByteArrayCode(QByteArray ba)
{
//    QString temp("");
//    int len1 = ba.length();
//    char* buff = new char[len1*2 + 2];
//    memset(buff, 0, sizeof(len1 * 2 + 2));
//    for(int i=0;i<len1;i++ )
//    {
//        sprintf(&buff[i*2],"%02X",(unsigned char)ba.at(i));
//    }
//    temp = QString(buff);
//    return temp;

    // 内存增长
    QByteArray ba64 = ba.toBase64();
    return QString::fromUtf8(ba64);
}

#include <QFileInfo>
static QString GetFileFormat(QString path)
{
    QString temp = path.right(path.length() - 1);
    QFileInfo info(path);
    return info.suffix();
    if (!info.isFile())
        return "";
     QStringList slst = temp.split(".");
     if (slst.size() < 2)
         return "";
     return slst.last().toLower();
}
static const QColor DefaultColor[2][13] =
{
    {QColor(231,0,11), QColor(236,99,0), QColor(244,149,0), QColor(255,240,1),
     QColor(144,196,31), QColor(33,174,53), QColor(0,159,149), QColor(0,161,236),
     QColor(0,70,157), QColor(26,31,133), QColor(95,26,135), QColor(146,7,128), QColor(229,0,119)},
    {QColor(126,0,62), QColor(210,192,165), QColor(90,73,61), QColor(106,57,5),
     QColor(112,112,112), QColor(0,0,0), QColor(247,135,80), QColor(241,158,187),
     QColor(135,57,160), QColor(68,59,46), QColor(141,165,232), QColor(30,189,223), QColor(216,165,7)}
};

static const QString MDColor[2][9] =
{
    {"#F44336", "#E91E63", "#9C27B0", "#673AB7", "#3F51B5", "#2196F3", "#03A9F4", "#00BCD4", "#795548"},
    {"#009688", "#4CAF50", "#8BC34A", "#CDDC39", "#FFC107", "#FF9800", "#FF5722", "#9E9E9E", "#607D8B"}
};

static void SetAutoStart(bool isStart)
{
    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                                   QSettings::NativeFormat);
    if (isStart)
    {
        QString path = qApp->applicationFilePath();
        path = path.replace("/", "\\");
        reg->setValue("happy", path);
    }
    else
    {
        reg->remove("happy");
    }
    delete reg;
}

static const QString StatusStr[4] ={cn("在线"),
                                   cn("勿扰"),
                                   cn("离开"),
                                   cn("关闭")};

static bool CpFile(QString path, QString newname, QString &error)
{
    bool b = true;
    QFile file(path);
    if (!file.exists())
    {
        error = cn("文件不存在");
        return false;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        error = cn("读取文件失败");
        return false;
    }
    QByteArray ba = file.readAll();
    file.close();
    file.setFileName(newname);
    if (file.exists())
    {
        if (!file.remove())
        {
            error = cn("文件已存在");
            return false;
        }
    }
    if (!file.open(QIODevice::WriteOnly))
    {
        error = cn("文件写入失败");
        return false;
    }
    file.write(ba);
    file.close();
    return b;
}

#endif // THMETHOD

