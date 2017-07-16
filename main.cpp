#include "ui/mainui.h"
#include "config/configuredata.h"
#include "util/thmethod.h"
#include "application/hlapplication.h"
#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QTranslator>
#include "ui/initui.h"
#include "sql/sql.h"
#include "manager/setmanager.h"
#include "ui/moveemojicustom.h"
#include "ui/ui3/msgbox.h"
#include "ui/ui3/emotionsbox.h"
#include "log/logout.h"
#include <QStyleFactory>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <TlHelp32.h>
#endif

bool killApp(QString app)
{
    bool ret = false;
#ifdef Q_OS_WIN
    PROCESSENTRY32 p32;
    p32.dwSize = sizeof(p32);
    HANDLE hProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcess == INVALID_HANDLE_VALUE)
    {
        qDebug() << "INVALID_HANDLE_VALUE";
    }
    BOOL b = ::Process32First(hProcess, &p32);
    while (b)
    {
        QString processName = QString::fromWCharArray(p32.szExeFile);
        if (app.compare(processName, Qt::CaseInsensitive) == 0)
        {
            HANDLE handle = ::OpenProcess(PROCESS_TERMINATE, 0, p32.th32ProcessID);
            BOOL iskill = ::TerminateProcess(handle, 0);
            ret = (bool)iskill;
            CloseHandle(handle);
            break;
        }
        b = ::Process32Next(hProcess, &p32);
    }
    CloseHandle(hProcess);
#endif
    return ret;
}

void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
//    QByteArray dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz").toLocal8Bit();
    QString stype = "Info";
    switch (type)
    {
    case QtDebugMsg:
        stype = "Info";
        break;
    case QtWarningMsg:
        stype = "Warning";
        break;
    case QtCriticalMsg:
        stype = "Critical";
        break;
    case QtFatalMsg:
        stype = "Fatal";
    }

    {
        fprintf(stderr, "%s: %s (%s:%u, %s)\n", stype.toLocal8Bit().constData(), localMsg.constData(), context.file, context.line, context.function);
//        fprintf(stdout, "%s -> %s: %s\n", dateTime.constData(), stype.toLocal8Bit().constData(), localMsg.constData());
//        fflush(stdout);
        Logout::GetInstance()->write(stype, msg);
//        Log(dateTime, stype, msg, context.file, context.line, context.function);

    }
    if (stype == "Fatal")
    {
        abort();
    }
}

#ifdef UI_DESIGN
#include "cutImage/cutimagewidget.h"
#include "ui/happytitlebar.h"
#include "ui/notifyui.h"
#include "cutImage/calcfromrect.h"
#include "thwidgets/model/itemeditor.h"
#include <QTreeWidget>
#include "ui/ui3/headerphotoui.h"
#include "ui/ui3/selftoolbutton.h"
#include "ui/ui3/selftabwidget.h"
#include "ui/ui3/topwidget.h"
#include "ui/ui3/update3.h"
#include "ui/ui3/firstrun.h"
#include "ui/ui3/progressui.h"
#endif

int main(int argc, char *argv[])
{
    HLApplication a(argc, argv, "happy3.0");
    a.setQuitOnLastWindowClosed(false);

#ifndef QT_NO_TRANSLATION
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&a);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translator);
#endif

#ifdef QT_NO_DEBUG
    if (a.isRunning())
    {
        a.sendMessage("message from other instance.");
        MsgBox::ShowMsgBox(cn("哈啤"), cn("哈啤不能重复开启"), cn("好的"));
        return 0;
    }
#endif
    killApp("UpdateHappy.exe");
    // 是否存在工作目录
    if (IsNeedFristRun())
    {
        FirstRun *run = new FirstRun;
        int id = run->exec();
        if (id != 0)
        {
            qApp->quit();
            return 0;
        }
    }
    // 是否需要升级数据
    bool showabout = false;
    if (IsNeedUpdate())
    {
        Update3 *update = new Update3;
        int id = update->exec();
        if (id != 0)
        {
            qApp->quit();
            return 0;
        }
        showabout = true;
    }
    else
    {
        // connect database
        Sql::instance()->setDatabaseInfo("QSQLITE", "127.0.0.1", 3306,
                                         GetWorkPath() + "/database/data.db", "user", "pw",
                                         2, 1, "SELECT UUID()");
        QString error;
        SqlDB *db = Sql::instance()->open(error);
        if (!db)
        {
            MsgBox *box = new MsgBox();
            box->setTitle(QStringLiteral("数据库打开失败"));
            box->setContent(error);
            box->setButtons(QStringLiteral("退出"));
            box->exec();
            qApp->quit();
            return 0;
        }
        db->close();
    }

    // init config and delete resource
    ConfigureData *config = ConfigureData::getInstance();
    config->initConfig();
    // log
    if (config->IsTrue("log"))
    {
#ifdef QT_NO_DEBUG
        qInstallMessageHandler(MessageOutput);
#endif
    }
    // check uid
    QString uid = config->getUuid();
    if (uid.isEmpty() || uid.isNull())
    {
        MsgBox::ShowMsgBox(QStringLiteral("严重错误"),
                           QStringLiteral("哈啤无法运行\n没有获取到用户识别码，请删除文件重新升级"),
                           QStringLiteral("确定"));
        qApp->quit();
        return 0;
    }
    // set auto start
    if (config->IsTrue("auto_start"))
    {
        SetAutoStart(true);
    }
    MainUI w;
    w.show();
    w.activateWindow();
    QThread::msleep(100);

    double dbversion = config->getIni("version").toDouble();
    if (showabout || dbversion < QString::number(HAPPY_VERSON).toDouble())
    {
        MsgBox::ShowBRbox(cn("升级提示"), cn("你的哈啤已经升级到了") + QString::number(HAPPY_VERSON));
        w.abouthappy();
    }
    if (config->getIni("download").isEmpty())
    {
        config->setIni("download", GetWorkPath() + "/dowmload");
    }
    config->setIni("version", HAPPY_VERSON);

    int ret = a.exec();

    if (ret == 100)
    {

        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }

    return ret;
}
