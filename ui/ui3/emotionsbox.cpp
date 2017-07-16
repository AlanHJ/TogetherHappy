#include "emotionsbox.h"
#include "emotiontab.h"
#include "../happytitlebar.h"
#include "../../util/thmethod.h"
#include "../../sql/sql.h"
#include "../../config/configuredata.h"
#include "../../thwidgets/emoji/emojitable.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "progressui.h"
#include <QTabWidget>
#include <Windows.h>
#include <QEvent>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QtWidgets>

EmotionsBox *EmotionsBox::box = nullptr;
EmotionsBox::EmotionsBox(QWidget *parent) : THWidgetBase(parent)
{
    qRegisterMetaType<EmoticonData>("EmoticonData");
    qRegisterMetaType<QList<EmoticonData>>("QList<EmoticonData>");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
//    setWindowModality(Qt::ApplicationModal);
    setFixedSize(450, 320);
    setTitleBarWidth(450);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/ui3/emoticon.png");
    bar->setBarContent(QStringLiteral("哈啤表情"));

    prew = new EmojiLabel();
    prew->setMovieable(true);
    prew->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    prew->setAttribute(Qt::WA_TranslucentBackground);
    prew->hide();

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(8, 30, 8, 8);
    v->setSpacing(0);
    EmotionTab *tab = new EmotionTab(this);
    tab->setTabPosition(QTabWidget::South);
    tab->setFixedWidth(450);
    v->addWidget(tab, 1, Qt::AlignCenter);

    QTabBar *tabbar = tab->tabBar();

    ConfigureData *conf = ConfigureData::getInstance();

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        this->hide();
    });

    connect(this, &EmotionsBox::addEmotion,
            this, [=] (QString tabstr, QList<EmoticonData> eds) {
        // add emotion
        QWidget *w = tab->widget(tab->indexOfByText(tabstr));
        EmojiTable *table = dynamic_cast<EmojiTable *>(w);
        if (!table)
            return;
        table->insertEmoji(eds);
    });

    connect(this, &EmotionsBox::addTab, this, [=] (EmoticonGroupData egd) {
        // add tab
        EmojiTable *table = new EmojiTable(this);
        table->setFixedSize(430, 250);
        table->initEmojiTable();
        tab->addTab(table, egd.getGroup());
        tables.append(table);
        qDebug() << "add table:" << tab->count();
        if (egd.getDeleteable())
        {
            tabbar->moveTab(tabbar->count() - 1, 0);
        }
        tab->setCurrentIndex(0);
        connect(table, &EmojiTable::enterEmoji, this, [=] (QString str) {
            if (!conf->IsTrue("open_preview"))
                return;
            prew->setMoiveRes(str);
            prew->show();
            prew->move(this->x() + this->width(), this->y() + 5);
        });
        connect(table, &EmojiTable::leaveEmoji, this, [=] () {
            if (!conf->IsTrue("open_preview"))
                return;
            prew->hide();
        });
        connect(table, &EmojiTable::emojiClicked, this, [=] (EmoticonData ed) {
            emit select(ed.getPath());
            QtConcurrent::run(this, &EmotionsBox::writedb, ed);
            this->hide();
        });
    });

    connect(this, &EmotionsBox::reload, this, [=] () {
        QTimer::singleShot(100, this, [=] () {
            connect(this, &EmotionsBox::initProgress,
                    ProgressUI::showProgerss(cns("重新加载资源, 请稍后..."), this), &ProgressUI::setValue);
            auto it = tables.begin();
            while (it != tables.end())
            {
                EmojiTable *tb = *it;
                if (tb)
                {
                    delete tb;
                    tb = nullptr;
                }
                ++it;
            }
            tables.clear();
            tab->clear();
            QtConcurrent::run(this, &EmotionsBox::loadData);
        });
    });

    // load data

    QFile file;
    file.setFileName(":res/css/tab.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        tab->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

    // read sql data
    QTimer::singleShot(500, this, [=] () {
        ProgressUI *ui = new ProgressUI();
        ui->setText(QStringLiteral("正在初始化载入资源, 请稍后..."));
        ui->show();
        MovePcenter(this, ui);
        ui->activateWindow();
        EmotionsBox * box = EmotionsBox::GetInstance();
        connect(box, &EmotionsBox::initProgress,
                         ui, &ProgressUI::setValue);
        QtConcurrent::run(this, &EmotionsBox::loadData);
    });
}

EmotionsBox::~EmotionsBox()
{
    if (prew)
    {
        prew->close();
        delete prew;
        prew = nullptr;
    }
    auto it = tables.begin();
    while (it != tables.end())
    {
        EmojiTable *tb = *it;
        if (tb)
        {
            delete tb;
            tb = nullptr;
        }
        ++it;
    }
    tables.clear();
}

EmotionsBox *EmotionsBox::GetInstance()
{
    if (!box)
    {
        if (ConfigureData::getInstance()->IsTrue("need_reload"))
        {
            ConfigureData::getInstance()->setIni("need_reload", 0);
        }
        box = new EmotionsBox();
    }
    return box;
}

void EmotionsBox::Destruct()
{
    if (box)
    {
        box->close();
        box->deleteLater();
    }
}

void EmotionsBox::loadData()
{
    thread()->msleep(100);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
        return;
    int rlt = 0;
    QString sql = "SELECT * FROM emoticon_group";
    QList<QObject *> objs = db->executeQuery(sql, EmoticonGroupData().metaObject(),
                                             rlt, er);
    if (objs.isEmpty())
    {
        db->close();
        return;
    }
    QStringList lst;
    auto it = objs.begin();
    while (it != objs.end())
    {
        QObject *obj = *it;
        EmoticonGroupData *egd = qobject_cast<EmoticonGroupData *>(obj);
        if (egd)
        {
            QString tab = egd->getGroup();
            lst << tab;
            emit addTab(*egd);
        }
        if (obj)
            delete obj;
        obj = nullptr;
        ++it;
    }
    emit initProgress(2);
    objs.clear();
//    thread()->msleep(1000);

    float fp = 96 / (float)lst.size();
    for (int i = 0; i < lst.size(); i++)
    {
        QList<EmoticonData> eds;
        QString group = lst.at(i);
        sql = "SELECT * FROM emoticon WHERE tab = '" + group + "'";
        QList<QObject *> temps = db->executeQuery(sql, EmoticonData().metaObject(), rlt, er);
        int counts = 0;
        auto it2 = temps.begin();
        while (it2 != temps.end())
        {
            QObject *obj = *it2;
            EmoticonData *ed = qobject_cast<EmoticonData *>(obj);
            if (ed)
            {
                eds.append(*ed);
            }
            if (obj)
                delete obj;
            obj = nullptr;
            emit initProgress(fp * counts / (float)temps.size() + fp * i + 2);
            ++it2;
            ++counts;
//            thread()->msleep(1);
        }
        emit addEmotion(group, eds);
    }
    emit initProgress(100);
    db->close();
    ConfigureData::getInstance()->setIni("need_reload", 0);
}

void EmotionsBox::writedb(EmoticonData ed)
{
    QMutexLocker lock(&mutex);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
        return;
    ed.setCounts(ed.getCounts() + 1);
    ed.setLasttime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    if (ed.getName().isEmpty())
        return;
    db->update(&ed, "name", er);
    db->close();
}

void EmotionsBox::mousePressEvent(QMouseEvent *e)
{
    return QWidget::mousePressEvent(e);
}

void EmotionsBox::mouseReleaseEvent(QMouseEvent *e)
{
    return QWidget::mouseReleaseEvent(e);
}

void EmotionsBox::mouseMoveEvent(QMouseEvent *e)
{
    return QWidget::mouseMoveEvent(e);
}

void EmotionsBox::hideEvent(QHideEvent *e)
{
    prew->hide();
    QTimer::singleShot(250, [=] () {
         emit signalHide();
    });
    return THWidgetBase::hideEvent(e);
}



bool EmotionsBox::event(QEvent *event)
{
    // class_ameneded 不能是custommenu的成员, 因为winidchange事件触发时, 类成员尚未初始化
        static bool class_amended = false;
        if (event->type() == QEvent::WinIdChange)
        {
            HWND hwnd = reinterpret_cast<HWND>(winId());
            if (class_amended == false)
            {
                class_amended = true;
                DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
                class_style &= ~CS_DROPSHADOW;
                ::SetClassLong(hwnd, GCL_STYLE, class_style); // windows系统函数
            }
        }
        return QWidget::event(event);
}


void EmotionsBox::showEvent(QShowEvent *e)
{
    if (ConfigureData::getInstance()->IsTrue("need_reload"))
    {
        emit reload();
    }
    return THWidgetBase::showEvent(e);
}
