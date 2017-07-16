#include "emotionmanager.h"
#include "../happytitlebar.h"
#include "../../util/thmethod.h"
#include "../../sql/sql.h"
#include "../../thwidgets/emoji/emojitable.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../config/configuredata.h"
#include "rotatebutton.h"
#include "selfmenu.h"
#include "inputbox.h"
#include "msgbox.h"
#include "combox.h"
#include "progressui.h"
#include <QtConcurrent/QtConcurrent>
#include <QtWidgets>

EmotionManager::EmotionManager(QWidget *parent) : THWidgetBase(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
//    setWindowModality(Qt::ApplicationModal);
    setFixedSize(540, 300);
    setTitleBarWidth(540);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(QStringLiteral("哈啤表情管理"));

    list = new EmotionListWidget(this);
    stack = new EmotionStackedWidget(this);
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(5, 0, 5, 0);
    h1->addWidget(list, 1);
    h1->addWidget(stack, 4);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 5);
    QLabel *infolbl = new QLabel(this);
    infolbl->setStyleSheet(cn("QLabel{font-family:微软雅黑;font:10px;color:white;"
                             "}"));
    infolbl->setWindowTitle(QString::number(0));
    QPushButton *pb1 = new QPushButton(QStringLiteral("新建分组"), this);
    QPushButton *pb4 = new QPushButton(QStringLiteral("移动表情"), this);
    QPushButton *pb2 = new QPushButton(QStringLiteral("导入表情"), this);
    QPushButton *pb3 = new QPushButton(QStringLiteral("删除表情"), this);
    QCheckBox *box1 = new QCheckBox(QStringLiteral("全选或取消选中"), this);
    box1->setFixedSize(114, 22);
    box1->setToolTip(QStringLiteral("仅对当前分组有效"));
    RotateButton *fresh = new RotateButton(this);
    fresh->setImagePath(":res/ui3/refresh");
    fresh->setFixedSize(22, 22);
    fresh->setLoop(0);
    h2->addSpacing(10);
    h2->addWidget(pb1, 1);
    h2->addStretch(2);
    h2->addWidget(infolbl, 1);
    h2->addWidget(fresh, 1);
    h2->addWidget(box1, 1);
    h2->addWidget(pb4, 1);
    h2->addWidget(pb2, 1);
    h2->addWidget(pb3, 1);
    h2->addSpacing(10);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(5, 35, 5, 5);
    v->addLayout(h1, 9);
    v->addLayout(h2, 1);

    prew = new EmojiLabel();
    prew->setMovieable(true);
    prew->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    prew->setAttribute(Qt::WA_TranslucentBackground);
    prew->hide();

    pupw = new PupupWidget();
    pupw->hide();

    QFile file;
    file.setFileName(":res/css/list.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        list->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pb1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        pb2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        pb3->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        pb4->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/checkbox.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        box1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    ConfigureData *conf = ConfigureData::getInstance();
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        this->close();
        this->deleteLater();
    });

    connect(this, &EmotionManager::addTab, this, [=] (EmoticonGroupData egd) {
        EmotionItem *item = new EmotionItem();
        item->setEgd(egd);
        item->setText(egd.getGroup());
        item->setTextAlignment(Qt::AlignCenter);
        if (egd.getDeleteable())
        {
            list->insertItem(0, item);
        }
        else
        {
            list->insertItem(list->count(), item);
        }
        if (egd.getEditable())
        {
            pupw->addItem(egd.getGroup());
        }

        EmojiTable *table = new EmojiTable(this);
        table->setObjectName(egd.getGroup());
        table->setFixedSize(430, 230);
        table->initEmojiTable();
        stack->addTable(table);

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
        connect(table, &EmojiTable::emojiToggled, this, [=] (EmoticonData ed, bool b) {
            // add list
            if (b)
            {
                eds.insert(ed.getName(), ed);
            }
            else
            {
                if (eds.contains(ed.getName()))
                {
                    eds.remove(ed.getName());
                }
            }
            int num1 = infolbl->windowTitle().toInt();
            infolbl->setText(cn("收藏%1张/选中%2张").arg(num1).arg(eds.size()));
        });
    });

    connect(this, &EmotionManager::addEmotion, this, [=] (QString str, EmoticonData ed) {
        // add emotion
        int index = stack->indexOfByObjname(str);
        if (index < 0)
            return;
        EmojiTable *table = dynamic_cast<EmojiTable *>(stack->widget(index));
        if (!table)
            return;
        table->updateEmoji(ed, ed.getEditable(), QSize(40, 40));
        if (!ed.getDeleteable())
            return;
        int num1 = infolbl->windowTitle().toInt();
        num1++;
        infolbl->setWindowTitle(QString::number(num1));
        infolbl->setText(cn("收藏%1张/选中%2张").arg(num1).arg(eds.size()));
    });

    connect(list, &QListWidget::currentRowChanged, [=] (int row) {
        EmotionItem *item = list->eitem(row);
        if (!item)
            return;
        QString txt = item->text();
        int index = stack->indexOfByObjname(txt);
        if (index < 0)
            return;
        stack->setCurrentIndex(index);
    });

    // new group
    connect(pb1, &QPushButton::clicked, this, [=] () {
        bool ok;
        QString input = InputBox::GetInput(QStringLiteral("输入组名"), &ok);
        if (!ok)
            return;
        if (input.trimmed().isEmpty() && ok)
        {
            MsgBox::ShowMsgBox(cns("提示"), cns("组名不能为空"),
                               cns("确定"));
            return;
        }
        if (list->containByStr(input))
        {
            MsgBox::ShowMsgBox(cns("警告"), cns("该组名已经存在"),
                               cns("确定"));
            return;
        }
        QString er;
        SqlDB *db = Sql::instance()->open(er);
        if (!db)
        {
            MsgBox::ShowMsgBox(cns("发生错误"), cns("哎呀, 增加分组时出现错误\n") + er,
                               cns("确定"));
            return;
        }
        EmoticonGroupData egd;
        egd.setGroup(input);
        egd.setDeleteable(true);
        egd.setEditable(true);
        egd.setReadable(true);
        bool b = db->insert(&egd, er);
        db->close();
        if (b)
        {
            // reload
            reload();
        }
        else
        {
            MsgBox::ShowMsgBox(cns("发生错误"), cns("哎呀, 增加分组时出现错误\n") + er,
                               cns("确定"));
        }
    });

    connect(pb2, &QPushButton::clicked, this, [=] () {
        // load emotion
        // get current tab
        EmotionItem *it = list->eitem(list->currentRow());
        if (!it)
        {
            MsgBox::ShowMsgBox(cns("提示"), cns("没有获取到需要导入的分组信息"),
                                                 cns("确定"));
            return;
        }
        EmoticonGroupData egd = it->getEgd();
        if (!egd.getEditable())
        {
            MsgBox::ShowMsgBox(cns("提示"), cns("不能将表情导入到 %1 分组").arg(egd.getGroup()),
                                                 cns("确定"));
            return;
        }
        int rlt = MsgBox::ShowMsgBox(cns("提示"), cns("你确定将表情导入到 %1 分组么").arg(egd.getGroup()),
                                             cns("确定"), cns("取消"));
        if (rlt != 0)
            return;
        QString filter = cns("图片资源(*.jpg *.gif *.png*.bmp);;全部文件(*.*)");
        QStringList infolst = QFileDialog::getOpenFileNames(this, cns("选择表情"), "/home", filter);
        if (infolst.isEmpty())
            return;
        connect(showProgress(cns("正在添加表情")), &ProgressUI::execFinished, this, [=] () {
            reload();
        });
        QtConcurrent::run(this, &EmotionManager::addEmotions, infolst, egd.getGroup());
    });

    connect(pb3, &QPushButton::clicked, this, [=] () {
        // delete some emotions
        if (eds.isEmpty())
        {
            MsgBox::ShowMsgBox(cns("提示"), cns("没有选中需要删除的表情"),
                                                 cns("确定"));
            return;
        }
        int rlt = MsgBox::ShowMsgBox(cns("提示"), cns("你确定将所选的表情删除么"),
                                             cns("确定"), cns("取消"));
        if (rlt != 0)
            return;
        connect(showProgress(cns("正在删除表情")), &ProgressUI::execFinished, this, [=] () {
            reload();
        });
        QtConcurrent::run(this, &EmotionManager::deleteEmotions);
    });

    connect(pb4, &QPushButton::clicked, this, [=] () {
        // move emotion to other group
        if (pupw->isVisible1())
        {
            pupw->hidePopup();
            return;
        }
        pupw->list()->setCurrentRow(-1);
        pupw->resize(pb4->width(), 100);
        pupw->showPopup();
        QPoint p = pb4->mapToGlobal(this->rect().topLeft());
        pupw->move(p.x(), p.y() - pupw->height() - 2);
    });

    connect(box1, &QCheckBox::toggled, this, [=] (bool b) {
        stack->setCheckedAll(b);
    });

    connect(pupw->list(), &QListWidget::itemClicked, this, [=] () {
        QString txt = pupw->list()->currentItem()->text();
        pupw->hidePopup();
        if (eds.isEmpty())
        {
            MsgBox::ShowMsgBox(cns("提示"), cns("没有选中需要移动的表情"),
                                                 cns("确定"));
            return;
        }
        connect(showProgress(cns("正在移动表情")), &ProgressUI::execFinished, this, [=] () {
            reload();
        });
        QtConcurrent::run(this, &EmotionManager::moveEmotions, txt);
    });

    connect(list, &EmotionListWidget::deleteGroup, this, [=] (EmoticonGroupData egd) {
        int rlt = MsgBox::ShowMsgBox(cns("提示"), cns("你确定要删除 %1 分组么\n删除分组会同时删除该分组下的表情").arg(egd.getGroup()),
                                     cns("确定"), cns("取消"));
        if (rlt != 0)
            return;
        if (!egd.getDeleteable())
        {
            MsgBox::ShowMsgBox(cns("提示"), cns("%1 分组不能被删除").arg(egd.getGroup()),
                                                 cns("确定"));
            return;
        }
        connect(showProgress(cns("正在删除分组")), &ProgressUI::execFinished, this, [=] () {
            reload();
        });
        QtConcurrent::run(this, &EmotionManager::deleteGroup, egd.getGroup());
    });

    connect(this, &EmotionManager::execProgress, this, [=] (int p) {
        if (p >= 100)
        {
            QTimer::singleShot(10, [=] () {
                list->setCurrentRow(0);
                this->activateWindow();
            });
        }
    });

    connect(this, &EmotionManager::execError, this, [=] (QString er) {
        QTimer::singleShot(100, this, [=] () {
            MsgBox::ShowMsgBox(cns("发生错误"), er,
                               cns("确定"));
        });
    });

    connect(fresh, &RotateButton::clicked, this, [=] () {
        connect(reload(), &ProgressUI::execFinished, this, [=] () {
            emit fresh->stopAnimation();
        });
    });

    // read sql data
    QTimer::singleShot(500, this, [=] () {
        QtConcurrent::run(this, &EmotionManager::loadData);
        showProgress(QStringLiteral("正在载入表情资源，请稍后..."));
    });

    // load data

    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
}

EmotionManager::~EmotionManager()
{
    if (prew)
    {
        delete prew;
        prew = nullptr;
    }
    if (pupw)
    {
        delete pupw;
        pupw = nullptr;
    }
    eds.clear();
}

ProgressUI *EmotionManager::reload()
{
    ProgressUI * ui = showProgress(QStringLiteral("正在重新载入资源，请稍后..."));
    list->clear();
    pupw->clear();
    stack->clearTabs();
    eds.clear();
    QtConcurrent::run(this, &EmotionManager::loadData);
    return ui;
}

void EmotionManager::loadData()
{
    thread()->msleep(100);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        emit execProgress(100);
        emit execError(cns("打开数据库失败:") + er);
        return;
    }
    int rlt = 0;
    QString sql = "SELECT * FROM emoticon_group";
    QList<QObject *> objs = db->executeQuery(sql, EmoticonGroupData().metaObject(),
                                             rlt, er);
    if (objs.isEmpty())
    {
        db->close();
        emit execProgress(100);
        emit execError(cns("没有数据"));
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
    emit execProgress(2);
    objs.clear();
//    thread()->msleep(1000);
    float fp = 96 / (float)lst.size();
    for (int i = 0; i < lst.size(); i++)
    {
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
                emit addEmotion(group, *ed);
            }
            if (obj)
                delete obj;
            obj = nullptr;
            emit execProgress(fp * counts / (float)temps.size() + fp * i + 2);
            ++it2;
            ++counts;
//            thread()->msleep(1);
        }
    }
    emit execProgress(100);
    db->close();
    ConfigureData::getInstance()->setIni("need_reload", 1);
}

void EmotionManager::deleteGroup(QString txt)
{
    thread()->msleep(100);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        emit execProgress(100);
        emit execError(cns("哎呀, 删除分组时出现错误\n") + er);
        return;
    }
    int rlt = 0;
    QString sql = "SELECT * FROM emoticon WHERE tab = '" + txt + "'";
    QList<QObject *> temps = db->executeQuery(sql, EmoticonData().metaObject(), rlt, er);
    emit execProgress(10);
    QStringList paths;
    for (int i = 0; i < temps.size(); i++)
    {
        QObject *obj = temps.at(i);
        EmoticonData *ed = qobject_cast<EmoticonData *>(obj);
        if (ed)
        {
            paths << ed->getPath();
            if (!ed->getThumb().isEmpty())
                paths << ed->getThumb();
        }
        if (obj)
            delete obj;
        obj = nullptr;
        emit execProgress(i / (float)temps.size() * 50 + 10);
    }
    EmoticonGroupData egd;
    egd.setGroup(txt);
    bool b = db->deldata(&egd, "tab", er);
    if (b)
    {
        // delete other
        EmoticonData ed;
        ed.setGroup(txt);
        b = db->deldata(&ed, "tab", er);
        if (!b)
        {
            emit execProgress(100);
            emit execError(cns("哎呀, 删除分组时出现错误\n") + er);
        }
        else
        {
            emit execProgress(65);
            b = ConfigureData::getInstance()->IsTrue("delete_image");
            if (b)
            {
                for (int i = 0; i < paths.size(); i++)
                {
                    QString path = paths.at(i);
                    bool b1= QFile::remove(path);
                    b &= b1;
                    if (!b1)
                        qDebug() << cns("删除表情文件时发生错误:") << path;
                    emit execProgress(i / (float)paths.size() * 30 + 65);
                }
                if (!b)
                {
                    emit execError(cns("哎呀, 删除文件并没有完全成功，详情查看日志"));
                }
            }
        }
    }
    else
    {
        emit execProgress(100);
        emit execError(cns("哎呀, 删除分组时出现错误\n") + er);
    }
    emit execProgress(100);
    db->close();
}

void EmotionManager::moveEmotions(QString tab)
{
    thread()->msleep(100);
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        emit execProgress(100);
        emit execError(cns("哎呀, 移动表情时出现错误\n") + er);
        return;
    }
    emit execProgress(2);
    int counts = 0;
    bool b = true;
    auto it = eds.begin();
    while (it != eds.end())
    {
        QString k = it.key();
        EmoticonData ed = it.value();
        ed.setGroup(tab);
        bool b1 = db->update(&ed, "name", er);
        if (!b1)
            qDebug() << cns("移动表情时发生错误:") << er << " name:" << k;
        b &= b1;
        ++it;
        ++counts;
        emit execProgress(counts / (float)eds.size() * 96 + 2);
    }
    if (!b)
        emit execError(cns("哎呀, 移动表情并没有完全成功，详情查看日志"));
    emit execProgress(100);
    db->close();
}

void EmotionManager::addEmotions(QStringList paths, QString tab)
{
    // open sql
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        emit execProgress(100);
        emit execError(cns("哎呀, 添加表情时出现错误\n") + er);
        return;
    }
    // filter size
    int filterSize = ConfigureData::getInstance()->getIntIni("delete_image") * 1024;
    QHash<QString, QString> MD5s;
    for (int i = 0; i < paths.size(); i++)
    {
        QString path = paths.at(i);
        QFileInfo info(path);
        if (info.exists() && info.size() > filterSize)
        {
            // calc MD5
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
        emit execProgress(i / (float)paths.size() * 20);
    }
    // move image and scaled image
    bool flag = true;
    QString returninfo;
    QList<EmoticonData> eds;
    QString datetimestr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString customPath = GetWorkPath() + "/custom";
    if (QDir().mkpath(customPath))
    {
        int counts = 0;
        auto it = MD5s.begin();
        while (it != MD5s.end())
        {
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
                    flag &= b;
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
                data.setGroup(tab);
                data.setDeleteable(true);
                data.setEditable(true);
                data.setReadable(true);
                eds.push_back(data);
            }
            else
            {
                bool b = QFile::copy(path, newPath);
                flag &= b;
                if (b)
                {
                    // success
                    data.setPath(newPath);
                    data.setName(md5);
                    data.setCounts(0);
                    data.setInserttime(datetimestr);
                    data.setLasttime(datetimestr);
                    data.setGroup(tab);
                    data.setDeleteable(true);
                    data.setEditable(true);
                    data.setReadable(true);
                    eds.push_back(data);
                }
                else
                {
                    qDebug() << "scaled save failed! " << path;
                }
            }
            emit execProgress(counts / (float)MD5s.size() * 40 + 20);
            ++it;
            ++counts;
        }
    }
    //write db
    for (int i = 0; i < eds.size(); i++)
    {
        EmoticonData ed = eds.at(i);
        // check count
        int rlt = 0;
        QString sql = "SELECT COUNT(*) FROM " + EmoticonData().objectName()+
                " WHERE name = '" + ed.getName() + "'";
        QVariant tempvar = db->executeQuery(sql, rlt, er);
        if (rlt != 0)
        {
            qDebug() << "select resource counts error:" + er +
                         " name:" + ed.getPath();
        }
        else
        {
            int num = tempvar.toInt();
            if (num == 0)
            {
                bool b = db->insert(&ed, er);
                flag &= b;
                if (!b)
                {
                    qDebug() << QStringLiteral("插入数据失败：") << er << endl
                             << ed.getName() << " " << ed.getPath();
                }
            }
            else
            {
                returninfo = returninfo.append(ed.getPath() + cns(" 该资源已存在\n"));
            }
        }

        emit execProgress(i / (float)eds.size() * 38 + 60);
    }

    db->close();
    if (!flag || !returninfo.isEmpty())
    {
        emit execError(cns("哎呀, 移动表情并没有完全成功\n") + returninfo);
    }
    emit execProgress(100);
}

void EmotionManager::deleteEmotions()
{
    QString er;
    SqlDB *db = Sql::instance()->open(er);
    if (!db)
    {
        emit execProgress(100);
        emit execError(cns("哎呀, 删除表情时出现错误\n") + er);
        return;
    }
    emit execProgress(2);
    int counts = 0;
    bool b = true;
    QString returnstr;
    bool deleteFlag = ConfigureData::getInstance()->IsTrue("delete_image");
    auto it = eds.begin();
    while (it != eds.end())
    {
        EmoticonData ed = it.value();
        QString path = ed.getPath();
        bool b1 = db->deldata(&ed, "name", er);
        b &= b1;
        if (!b1)
        {
            qDebug() << cns("删除表情时发生错误:") << path;
        }
        else
        {
            if (deleteFlag)
            {
                bool b2= QFile::remove(path);
                b &= b2;
                if (!b2)
                {
                    qDebug() << cns("删除表情文件时发生错误:") << path;
                    returnstr = returnstr.append(path + cns("删除该表情文件时失败\n"));
                }
            }
        }
        ++it;
        ++counts;
        emit execProgress(counts / (float)eds.size() * 96 + 2);
    }
    if (!b)
    {
        emit execError(cns("哎呀, 删除文件并没有完全成功，详情查看日志"));
    }
    emit execProgress(100);
    db->close();
}

ProgressUI *EmotionManager::showProgress(QString txt)
{
    ProgressUI *ui = ProgressUI::showProgerss(txt, this);
    connect(this, &EmotionManager::execProgress,
            ui, &ProgressUI::setValue);
    return ui;
}

void EmotionManager::paintEvent(QPaintEvent *e)
{
    THWidgetBase::paintEvent(e);
    int w = list->width();
    int h = this->height();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::white);
    QPoint p1(w + 10, 35);
    QPoint p2(w + 10, h - 10);
    QLinearGradient linecolor(p1, p2);
    linecolor.setColorAt(0, QColor(255, 255, 255, 80));
    linecolor.setColorAt(0.25, QColor(255, 255, 255, 255));
    linecolor.setColorAt(0.75, QColor(255, 255, 255, 255));
    linecolor.setColorAt(1, QColor(255, 255, 255, 80));
    p.setPen(QPen(QBrush(linecolor), 1));
    p.drawLine(p1, p2);
}


EmotionListWidget::EmotionListWidget(QWidget *parent)
    : QListWidget(parent)
{
    menu = new SelfMenu(this);
    connect(menu->addAction(cns("删除分组")), &QAction::triggered, this, [=] () {
        EmotionItem *it = dynamic_cast<EmotionItem *>(currentItem());
        if (it)
            emit deleteGroup(it->getEgd());
        else
            MsgBox::ShowMsgBox(cns("错误"), cns("未能找到分组"),
                               cns("确定"));
    });
}

int EmotionListWidget::indexOfByStr(QString str)
{
    int index = -1;
    for (int i = 0; i < count(); i++)
    {
        EmotionItem *it = eitem(i);
        if (it)
        {
            if (it->text() == str)
            {
                index = i;
                break;
            }
        }
    }
    return index;
}

bool EmotionListWidget::containByStr(QString str)
{
    return indexOfByStr(str) == -1 ? false : true;
}

EmotionItem *EmotionListWidget::eitem(int index)
{
    return dynamic_cast<EmotionItem *>(item(index));
}

EmotionStackedWidget::EmotionStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{

}

int EmotionStackedWidget::indexOfByObjname(QString txt)
{
    int index = -1;
//    for (int i = 0; i < count(); i++)
//    {
//        QWidget *w = widget(i);
//        if (w)
//        {
//            if (w->objectName() == txt)
//            {
//                index = i;
//                break;
//            }
//        }
//    }
    auto it = tabs.begin();
    while (it != tabs.end())
    {
        int key = it.key();
        EmojiTable *val = it.value();
        if (val && val->objectName() == txt)
        {
            index = key;
            break;
        }
        ++it;
    }
    return index;
}

void EmotionStackedWidget::addTable(EmojiTable *tab)
{
    int key = addWidget(tab);
    if (tabs.contains(key))
    {
        EmojiTable *t = tabs.value(key);
        if (t)
        {
            t->clear();
            delete t;
            t = nullptr;
        }
    }
    tabs.insert(key, tab);
//    qDebug() << "emotion add tab:" << this->count();
}

void EmotionStackedWidget::clearTabs()
{
//    int i = 0;
    auto it = tabs.begin();
    while (it != tabs.end())
    {
        EmojiTable *val = it.value();
        if (val)
        {
            this->removeWidget(val);
            val->clear();
//            i++;
//            qDebug() << "delete emotion tab:" << i;
        }
        ++it;
    }
}

void EmotionStackedWidget::setCheckedAll(bool b)
{
    EmojiTable *table = dynamic_cast<EmojiTable *>(currentWidget());
    if (!table)
        return;
    table->setCheckedAll(b);
}


void EmotionListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QPoint p = e->pos();
    if (!itemAt(p))
        return;
    menu->exec(QCursor::pos());
    e->accept();
}


EmotionItem::EmotionItem(QListWidget *view, int type)
    : QListWidgetItem(view, type)
{

}

EmoticonGroupData EmotionItem::getEgd()
{
    return egd;
}

void EmotionItem::setEgd(EmoticonGroupData egd)
{
    this->egd = egd;
}
