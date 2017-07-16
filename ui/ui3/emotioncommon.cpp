#include "emotioncommon.h"
#include "../happytitlebar.h"
#include "../../config/configuredata.h"
#include "../../sql/sql.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>

EmotionCommon::EmotionCommon(QWidget *parent) : THWidgetBase(parent)
{
    qRegisterMetaType<EmoticonData>("EmoticonData");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/ui3/emoticon.png");
    bar->setBarContent(QStringLiteral("最近/常用表情"));
    setFixedSize(396, 200);
    setTitleBarWidth(396);

    prew = new EmojiLabel();
    prew->setMovieable(true);
    prew->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    prew->setAttribute(Qt::WA_TranslucentBackground);
    prew->hide();
    // 从数据库获取前CommonEmotionView16和16的表情路径
    QGridLayout *grid = new QGridLayout(this);
    grid->setContentsMargins(5, 30, 5, 5);
    grid->setSpacing(0);
//    grid->setMargin(0);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            EmojiLabel *elbl = new EmojiLabel(this);
            connect(elbl, &EmojiLabel::enterEmoji, this, [=] () {
                if (!conf->IsTrue("open_preview"))
                    return;
                elbl->setContentsMargins(3, 3, 3, 3);
                prew->setMoiveRes(elbl->ImagePath());
                prew->show();
                prew->move(this->x() + this->width(), this->y() + 5);
            });
            connect(elbl, &EmojiLabel::leaveEmoji, this, [=] () {
                if (!conf->IsTrue("open_preview"))
                    return;
                elbl->setContentsMargins(0, 0, 0, 0);
                prew->hide();
            });
            connect(elbl, &EmojiLabel::clicked, this, [=] (EmoticonData ed) {
                emit select(ed.getPath());
                QtConcurrent::run(this, &EmotionCommon::writedb, ed);
                this->close();
                this->deleteLater();
            });
            elbl->setFixedSize(38, 38);
            grid->addWidget(elbl, i, j/*, Qt::AlignCenter*/);
            lst.push_back(elbl);
        }
    }
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        this->close();
        this->deleteLater();
    });

    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

    QtConcurrent::run(this, &EmotionCommon::loadData);

    connect(this, &EmotionCommon::loadPath, this, [=] (int i, EmoticonData d) {
        lst.at(i)->setData(d);
    });

}

EmotionCommon::~EmotionCommon()
{
    if (prew)
    {
        delete prew;
        prew = nullptr;
    }
}

void EmotionCommon::closeEmotion()
{
    QPoint p = mapFromGlobal(QCursor::pos());
    if (this->rect().contains(p))
        return;
    this->close();
    this->deleteLater();
}

void EmotionCommon::loadData()
{
    // read emoji
    QString error;
    SqlDB *db = Sql::instance()->open(error);
    if (!db)
        return;
    QString sql = "SELECT * FROM CommonEmotionView";
    int rlt = 0;
    QList<QObject *> objs = db->executeQuery(sql, EmoticonData().metaObject(), rlt, error);
    sql = "SELECT * FROM RecentEmotionView";
    QList<QObject *> temps = db->executeQuery(sql, EmoticonData().metaObject(), rlt, error);
    db->close();
    auto it = temps.begin();
    while (it != temps.end())
    {
        objs.push_back(*it);
        ++it;
    }
    temps.clear();
    if (objs.size() <= lst.size())
    {
        for (int i = 0; i < objs.size(); i++)
        {
            EmoticonData *ed = qobject_cast<EmoticonData*>(objs.at(i));
            if (ed)
            {
                emit loadPath(i, *ed);
            }
            delete ed;
            ed = nullptr;
        }
    }
}

void EmotionCommon::writedb(EmoticonData ed)
{
    QMutexLocker lock(&commonmutex);
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

void EmotionCommon::mousePressEvent(QMouseEvent *e)
{
    return QWidget::mousePressEvent(e);
}

void EmotionCommon::mouseReleaseEvent(QMouseEvent *e)
{
    return QWidget::mouseReleaseEvent(e);
}

void EmotionCommon::mouseMoveEvent(QMouseEvent *e)
{
    return QWidget::mouseMoveEvent(e);
}


void EmotionCommon::enterEvent(QEvent *e)
{
    return THWidgetBase::enterEvent(e);
}

void EmotionCommon::leaveEvent(QEvent *e)
{
    QTimer::singleShot(100, this, [=] () {
        closeEmotion();
    });
    return THWidgetBase::leaveEvent(e);
}
