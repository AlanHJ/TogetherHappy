#include "chat.h"
#include "../happytitlebar.h"
#include "../../thwidgets/happytextedit.h"
#include "../../config/configuredata.h"
#include "../../util/thmethod.h"
#include "combox.h"
#include "colorbutton.h"
#include "colorwell.h"
#include "emotionsbox.h"
#include "selfbutton.h"
#include "emotioncommon.h"
#include <QtWidgets>
#include "selfmenu.h"
#include "msgbox.h"
#include "../../sql/sql.h"
#include "../notifyui.h"
#include "../../cutImage/cutimagewidget.h"

Chat::Chat(bool shake, QWidget *w)
    : THWidgetBase(w)
{
    isAlert = false;
    lastRecvTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    hidea = nullptr;
    setFixedSize(586, 510);
    setTitleBarWidth(586);
    setSizeEnable(true, QSize(586, 510));
    bar->setBarButtons(HappyTitleBar::MinMaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");   // 需重新设定
    bar->setBarContent(QStringLiteral("哈啤聊天界面"));
    connect(bar, &HappyTitleBar::signalMaximize, this, [=] () {
        if (isMaximized())
            showNormal();
        else
            showMaximized();
    });
    connect(bar, SIGNAL(signalMinimize()), this, SLOT(showMinimized()));
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        this->close();
    });

    // add widgets
    QVBoxLayout *v1 = new QVBoxLayout();
    v1->setSpacing(0);
    v1->setContentsMargins(0, 0, 0, 0);
    browser = new HappyTextEdit(this);
    browser->setReadOnly1(true);
    edit = new HappyTextEdit(this);

    centerBar = new QToolBar(this);
    centerBar->setMovable(true);
    SelfButton *emojiBtn = new SelfButton(this);
    emojiBtn->setToolTip(cn("表情"));
    emojiBtn->setFixedSize(22, 22);
    emojiBtn->setCheckable(true);
    QPushButton *fontBtn = new QPushButton(this);
    fontBtn->setToolTip(cn("字体"));
    fontBtn->setCheckable(true);
    fontBtn->setFixedSize(20, 20);

    shakeBtn = new QToolButton(this);
    shakeBtn->setToolTip(cn("抖一抖"));
    shakeBtn->setFixedSize(30, 20);
    shakeBtn->setPopupMode(QToolButton::MenuButtonPopup);
    SelfMenu *shakeMenu = new SelfMenu(shakeBtn);
    shakeReject = shakeMenu->addAction(QStringLiteral("屏蔽"));
    shakeReject->setCheckable(true);
    shakeBtn->setMenu(shakeMenu);
    shakeBtn->setIcon(QIcon(":res/ui3/shake.png"));
    shakeBtn->setVisible(shake);

    QPushButton *cutBtn = new QPushButton(this);
    cutBtn->setToolTip(cn("截图"));
    cutBtn->setFixedSize(20, 20);

    QPushButton *clearBtn = new QPushButton(this);
    clearBtn->setToolTip(cn("清理消息"));
    clearBtn->setFixedSize(20, 20);

    hideBtn = new QPushButton(this);
    hideBtn->setToolTip(cn("隐藏/显示侧边栏"));
    hideBtn->setCheckable(true);
    hideBtn->setFixedSize(20, 20);

    centerBar->addWidget(emojiBtn);
    centerBar->addWidget(fontBtn);
    centerBar->addWidget(shakeBtn);
    centerBar->addWidget(cutBtn);
    centerBar->addWidget(clearBtn);
    hidea = centerBar->addWidget(hideBtn);
    centerBar->setStyleSheet("spacing:5px;");

    fontBar = new QToolBar(this);
    fontBar->setStyleSheet("spacing:2px;");
    Combox *fontCbox = new Combox(fontBar);
    fontCbox->addItems1(QFontDatabase().families());
    fontCbox->setTextAlignment(Qt::AlignCenter);
    Combox *fontsizeCbox = new Combox(fontBar);
    QStringList sizes;
    for (int size : QFontDatabase().standardSizes())
        sizes << QString::number(size);
    fontsizeCbox->setTextAlignment(Qt::AlignCenter);
    fontsizeCbox->addItems1(sizes);
    QPushButton *boldBtn = new QPushButton(fontBar);
    boldBtn->setCheckable(true);
    boldBtn->setFixedSize(20, 18);
    QPushButton *italicBtn = new QPushButton(fontBar);
    italicBtn->setCheckable(true);
    italicBtn->setFixedSize(20, 18);
    QPushButton *underlineBtn = new QPushButton(fontBar);
    underlineBtn->setCheckable(true);
    underlineBtn->setFixedSize(20, 18);
    ColorButton *colorBtn = new ColorButton(fontBar);
    colorBtn->setFixedSize(20, 20);
    colorBtn->setColor(Qt::white);
    fontBar->addWidget(fontCbox);
    fontBar->addWidget(fontsizeCbox);
    fontBar->addWidget(boldBtn);
    fontBar->addWidget(italicBtn);
    fontBar->addWidget(underlineBtn);
    fontBar->addWidget(colorBtn);

    QToolButton *toolBtn = new QToolButton(this);
    toolBtn->setFixedSize(85, 25);
    toolBtn->setText(QStringLiteral("发送"));
    toolBtn->setPopupMode(QToolButton::MenuButtonPopup);
    SelfMenu *menu = new SelfMenu(toolBtn);
    QAction *a1 = menu->addAction(QStringLiteral("Enter"));
    a1->setCheckable(true);
    QAction *a2 = menu->addAction(QStringLiteral("Ctrl + Enter"));
    a2->setCheckable(true);
    toolBtn->setMenu(menu);
    QActionGroup *ag = new QActionGroup(this);
    ag->addAction(a1);
    ag->addAction(a2);
    a1->setChecked(true);

    msgpupw = new MsgLine(this);
    msgpupw->setVisible(false);

    QPushButton *btn = new QPushButton(QStringLiteral("关闭"), this);
    btn->setFixedSize(70, 25);
    connect(btn, &QPushButton::clicked, this, [=] () {
        this->close();
    });

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 10);
    h1->addWidget(btn, 0, Qt::AlignRight | Qt::AlignVCenter);
    h1->addWidget(toolBtn, 0, Qt::AlignVCenter);


    v1->addWidget(browser, 5);
    v1->addSpacing(1);
    v1->addWidget(msgpupw, 0);
    v1->addWidget(fontBar, 0);
    v1->addWidget(centerBar, 0);
    v1->addSpacing(1);
    v1->addWidget(edit, 2);
    v1->addSpacing(5);
    v1->addLayout(h1, 0);

    topBar = new QToolBar(this);
    topBar->setStyleSheet("spacing:5px;");
    stackBtnGroup = new QButtonGroup(this);

    stack = new QStackedWidget(this);
    stack->setMaximumWidth(180);
    stack->setMouseTracking(true);
    stack->setVisible(false);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    h2->addLayout(v1, 2);
    h2->addWidget(stack, 1);

    QVBoxLayout *v2 = new QVBoxLayout();
    v2->setSpacing(0);
    v2->setContentsMargins(10, 0, 0, 0);
    v2->addSpacerItem(new QSpacerItem(this->width(), 30));
    v2->addWidget(topBar, 0);
    v2->addSpacing(1);
    v2->addLayout(h2, 1);

    edit->setFocus();
    setLayout(v2);


    ConfigureData *conf = ConfigureData::getInstance();

    //set style
    QFile file;
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        btn->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/toolbtn.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        toolBtn->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString str = file.readAll();
        emojiBtn->setStyleSheet(str.arg("emoticon").arg("emoticon").arg("emoticon"));
        fontBtn->setStyleSheet(str.arg("font").arg("font").arg("font"));
        cutBtn->setStyleSheet(str.arg("cut").arg("cut").arg("cut"));
        clearBtn->setStyleSheet(str.arg("clear_msg").arg("clear_msg").arg("clear_msg"));
        hideBtn->setStyleSheet(str.arg("hide").arg("hide").arg("hide"));

        boldBtn->setStyleSheet(str.arg("font_bold").arg("font_bold").arg("font_bold"));
        italicBtn->setStyleSheet(str.arg("font_italic").arg("font_italic").arg("font_italic"));
        underlineBtn->setStyleSheet(str.arg("font_underline").arg("font_underline").arg("font_underline"));
    }
    file.close();

    file.setFileName(":res/css/toolbtn2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        shakeBtn->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    file.setFileName(":res/css/combox2.css");
    if  (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        fontCbox->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba).arg("30px"));
        fontsizeCbox->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba).arg("0px"));
    }
    file.close();
    // 160,240,120
    connect(edit, &HappyTextEdit::returnPress, browser, [=] () {  
        // send msg
        if (edit->toPlainText().trimmed().isEmpty())
        {
            NotifyUI::ShowNotify(cn("不能发送空的消息"));
            return;
        }
        sendMsg();
    });

    connect(browser, &HappyTextEdit::addEmotion, this, [=] (QString path, QString tab) {
        // add emotion
        QString er;
        SqlDB *db = Sql::instance()->open(er);
        if (!db)
        {
            NotifyUI::Show3sNotify(cn("失败,添加错误\n") + er);
            return;
        }
        QString customPath = GetWorkPath() + "/custom";
        if (!QDir().mkpath(customPath))
        {
            NotifyUI::Show3sNotify(cn("失败,无法创建文件夹"));
            db->close();
            return;
        }
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly))
        {
            NotifyUI::Show3sNotify(cn("失败,无法验证文件"));
            db->close();
            return;
        }
        QByteArray ba = QCryptographicHash::hash(f.readAll(), QCryptographicHash::Md5);
        QString md5 = ba.toHex();
        f.close();

        int rlt = 0;
        QString sql = "SELECT COUNT(*) FROM " + EmoticonData().objectName()+
                " WHERE name = '" + md5 + "'";
        QVariant var = db->executeQuery(sql, rlt, er);
        if (rlt != 0)
        {
            NotifyUI::Show3sNotify(cn("失败,数据库查询失败\n") + er);
            db->close();
            return;
        }
        if (var.toInt() != 0 )
        {
            NotifyUI::Show3sNotify(cn("失败,该表情已经存在"));
            db->close();
            return;
        }
        EmoticonData ed;
        QFileInfo info(path);
        QString datetimestr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        if (info.size() >= 1024 * 10)
        {
            QImage image;
            QImageReader reader(path);
            image = reader.read();
            image = image.scaled(50, 50);
            QString suffix = info.suffix().compare("gif", Qt::CaseInsensitive) == 0 ?
                        "jpg" : info.suffix();
            QString thumbPath = customPath + "/" + md5 + "_50_50." + suffix;
            bool b = image.save(thumbPath);
            if (b)
                ed.setThumb(thumbPath);
        }
        QString newPath = customPath + "/" + md5 + "." + info.suffix();
        if (!QFile::exists(newPath))
        {
            bool b = QFile::copy(path, newPath);
            if (b)
                ed.setPath(newPath);
            else
                ed.setPath(path);
        }
        else
        {
            ed.setPath(newPath);
        }
        ed.setName(md5);
        ed.setCounts(0);
        ed.setInserttime(datetimestr);
        ed.setLasttime(datetimestr);
        ed.setGroup(tab);
        ed.setDeleteable(true);
        ed.setEditable(true);
        ed.setReadable(true);
        bool b = db->insert(&ed, er);
        db->close();
        if (b)
        {
            NotifyUI::Show3sNotify(cn("添加成功"));
            QList<EmoticonData> list;
            list.append(ed);
            emit EmotionsBox::GetInstance()->addEmotion(tab, list);
        }
        else
        {
            NotifyUI::Show3sNotify(cn("添加失败\n") + er);
        }
    });

    connect(stackBtnGroup, SIGNAL(buttonToggled(int,bool)),
            this, SLOT(buttonToggled(int,bool)));
    connect(fontBtn, &QPushButton::clicked, [=] () {
        fontBar->setVisible(!fontBar->isVisible());
    });
    fontBar->setVisible(false);

    connect(fontCbox, &QComboBox::currentTextChanged, this, [=] () {
        QString family = fontCbox->currentText();
        edit->txtFamily(family);
        conf->setIni("font_name", family);
    });
    connect(fontsizeCbox, &QComboBox::currentTextChanged, this, [=] () {
        QString size = fontsizeCbox->currentText();
        edit->txtSize(size.toFloat());
        conf->setIni("font_size", size);
    });
    connect(boldBtn, &QPushButton::toggled, this, [=] (bool b) {
        edit->txtBold(b);
        conf->setIni("font_bold", b);
    });
    connect(italicBtn, &QPushButton::toggled, this, [=] (bool b) {
        edit->txtItalic(b);
        conf->setIni("font_italic", b);
    });
    connect(underlineBtn, &QPushButton::toggled, this, [=] (bool b) {
        edit->txtUnderLine(b);
        conf->setIni("font_underline", b);
    });
    connect(colorBtn, &QPushButton::clicked, this, [=] () {
        ColorWell *well = new ColorWell(colorBtn->CurrentColor());
        well->show();
        well->activateWindow();
        QPoint p = mapToGlobal(browser->rect().bottomRight());
        p = QPoint(p.x() + 10,
                   p.y() + 40);
        well->move(p.x(), p.y());
        connect(well, SIGNAL(signalSelectColor(QColor)),
                edit, SLOT(txtColor(QColor)));
        connect(well, SIGNAL(signalSelectColor(QColor)),
                colorBtn, SLOT(setColor(QColor)));
        connect(well, &ColorWell::signalSelectColor,
                colorBtn,[=] (QColor color) {
            colorBtn->setColor(color);
            conf->setColorIni("font_color", color);
        });
    });

    connect(emojiBtn, &QPushButton::clicked, this, [=] () {
        emojiBtn->setReset(false);
        EmotionsBox *box = EmotionsBox::GetInstance();
        if (emojiBtn->isChecked())
        {
            box->show();
            QPoint p =  emojiBtn->mapToGlobal(emojiBtn->rect().topLeft());
            box->move(p.x() - box->width() / 2, p.y() - box->height() - 3);
            connect(box, &EmotionsBox::select, this, [=] (QString str) {
                edit->insertHtml1(ImagePathToHtml(str));
                edit->setFocus();
            });
            connect(box, &EmotionsBox::signalHide, this, [=] () {
                emojiBtn->setChecked(false);
                disconnect(box, &EmotionsBox::signalHide, 0, 0);
                disconnect(box, &EmotionsBox::select, 0, 0);
                edit->setFocus();
            }, Qt::UniqueConnection);
        }
        else
        {
            box->hide();
        }
    });

    connect(emojiBtn, &SelfButton::signalEnter, this, [=] () {
        QTimer::singleShot(200, this, [=] () {
            QPoint pt = emojiBtn->mapFromGlobal(QCursor::pos());
            QRect rect = emojiBtn->rect();
            if (!rect.contains(pt) || qobject_cast<EmotionCommon*>(qApp->activeWindow()))
            {
                return;
            }
            if (!emojiBtn->isReset())
            {
                return;
            }
            EmotionCommon *common = new EmotionCommon();
            common->show();
            common->activateWindow();
            QPoint p =  emojiBtn->mapToGlobal(emojiBtn->pos());
            common->move(p.x() - common->width() / 2, p.y() - common->height() - 1);
            connect(emojiBtn, &SelfButton::signalLeave, common, [=] () {
                disconnect(emojiBtn, &SelfButton::signalLeave, 0, 0);
                QTimer::singleShot(50, this, [=] () {
                    common->closeEmotion();
                });
                edit->setFocus();
            });
            connect(common, &EmotionCommon::select, this, [=] (QString str) {
                edit->insertHtml1(ImagePathToHtml(str));
                edit->setFocus();
            });
        });
    });

    connect(hideBtn, &QPushButton::toggled, this, [=] (bool b) {
        if (b)
        {
            v1->setContentsMargins(0, 0, 0, 0);
        }
        else
        {
            v1->setContentsMargins(0, 0, 10, 0);
        }
        stack->setVisible(b);
        conf->setIni("chat_hide_right", b);
    });
//    hideBtn->setChecked(true);

    connect(clearBtn, &QPushButton::clicked, this, [=] () {
        browser->clearContent();
    });

    connect(cutBtn, &QPushButton::clicked, this, [=] () {
        CutImageWidget *cut = CutImageWidget::GetInstance();
        if (cut->isVisible())
            return;
        cut->init();
        cut->show();
        cut->activateWindow();
    });

    connect(toolBtn, &QToolButton::clicked, this, [=] () {
        if (edit->toPlainText().trimmed().isEmpty())
        {
            NotifyUI::ShowNotify(cn("不能发送空的消息"));
            return;
        }
        sendMsg();
    });

    connect(ag, &QActionGroup::triggered, this, [=] () {
        QAction *a = ag->checkedAction();
        if (!a)
            return;
        QString txt = a->text();
        conf->setIni("send_key", txt);
    });

    connect(msgpupw, &MsgLine::Clicked,
            this, [=] () {
        browser->scrollMax();
    });

    connect(browser, &HappyTextEdit::scrollValueMax,
            this, [=] () {
        msgpupw->setVisible(false);
    });

    // load data
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
    fontCbox->setCurrentText(conf->getIni("font_name"));
    fontsizeCbox->setCurrentText(conf->getIni("font_size"));
    boldBtn->setChecked(conf->IsTrue("font_bold"));
    italicBtn->setChecked(conf->IsTrue("font_italic"));
    underlineBtn->setChecked(conf->IsTrue("font_underline"));
    colorBtn->setColor(conf->getColorIni("font_color"));
    hideBtn->setChecked(conf->IsTrue("chat_hide_right"));
    emit hideBtn->toggled(conf->IsTrue("chat_hide_right"));
    edit->txtColor(conf->getColorIni("font_color"));
    QString key = conf->getIni("send_key");
    a1->setChecked(true);
    if (key.compare("Ctrl + Enter", Qt::CaseInsensitive) == 0)
    {
        a2->setChecked(true);
    }
    setFixedSize(conf->getIntIni("chat_w"), conf->getIntIni("chat_h"));

    bezierEnable = true;
    if (conf->IsTrue("bezier"))
    {
        bezierTime->setFrameRange(0, this->width());
        bezierTime->start();
    }
}

Chat::~Chat()
{
    //    browser->clearContent();
}

bool Chat::isAlertFlag()
{
    return isAlert;
}

qint64 Chat::getLastRecvTime()
{
    return lastRecvTime;
}

void Chat::setLastRecvTime(qint64 time)
{
    lastRecvTime = time;
}

void Chat::setChatTitle(QString content, QString ico)
{
    bar->setBarContent(content);
    bar->setBarIcon(ico);
    setWindowTitle(content);
    setWindowIcon(QIcon(ico));
}

void Chat::inserHtml(QString html, QString src, QColor color)
{
    if (browser->textCursor().hasSelection())
    {
        browser->moveCursor(QTextCursor::EndOfLine);
    }
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if (!browser->isMaxVertical())
    {
        if (src == cn("我") || src == cn("哈啤提示"))
        {
            browser->scrollMax();
        }
        else
        {
            msgpupw->setMsg(html, src, datetime);
            msgpupw->setVisible(true);
        }
    }
    // QColor(100, 240, 60, 200)

    QTextCharFormat fmt;
    fmt.setFontFamily(cn("微软雅黑"));
    fmt.setFontPointSize(8);
    fmt.setForeground(color);
    browser->setCurrentCharFormat(fmt);
    browser->append(datetime + " " + src);
    browser->appendHtml(html);
    browser->append("\n");
}

void Chat::resizeEvent(QResizeEvent *e)
{
    bar->setBarWidth(this->width());
    return THWidgetBase::resizeEvent(e);
}

void Chat::buttonToggled(int, bool)
{
    stack->setCurrentIndex(stackBtnGroup->checkedId());
}

void Chat::closeEvent(QCloseEvent *e)
{
    emit chatClose();
    ConfigureData *config = ConfigureData::getInstance();
    config->setIni("chat_w", this->width());
    config->setIni("chat_h", this->height());
    e->accept();
    this->deleteLater();
}


void Chat::showEvent(QShowEvent *e)
{
    isAlert = false;
    edit->setFocus();
    emit chatShow();
    activeTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    return THWidgetBase::showEvent(e);
}
