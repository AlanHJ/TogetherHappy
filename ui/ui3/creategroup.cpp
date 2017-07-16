#include "creategroup.h"
#include "../happytitlebar.h"
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../thwidgets/happytextedit.h"
#include "msgbox.h"
#include <ctime>
#include <QtWidgets>
#include "emotionsbox.h"

CreateGroup::CreateGroup(QStringList items, QWidget *parent)
    : THWidgetBase(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(380, 250);
    setTitleBarWidth(380);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(cn("创建群/讨论组"));
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
            this->close();
    });

    QVBoxLayout *v= new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *v1= new QVBoxLayout();
    v1->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *v2= new QVBoxLayout();
    v2->setContentsMargins(0, 0, 0, 0);

    EmojiLabel *photo = new EmojiLabel(this);
    photo->setMoiveRes(":res/ui3/default_group_create.gif");
    photo->setMovieable(true);
    photo->setFixedSize(70, 70);
    photo->setStyleSheet(cns("QLabel{border-width:1px;border-style:solid;border-radius:3px;border-color:transparent;"
                             "background:transparent;}"
                             "QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}"
                             "QLabel:pressed{border-width:1px;border-style:solid;border-radius:3px;border-color:rgba(240,240,240,50);}"));
    QLabel *lbl1 = new QLabel(cn("修改群图片(点击)"), this);
    lbl1->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QPushButton *pb1 = new QPushButton(cn("从表情库中选择"), this);
    pb1->setFixedWidth(120);
    v1->addWidget(photo, 8, Qt::AlignCenter);
    v1->addWidget(lbl1,  1, Qt::AlignCenter);
    v1->addWidget(pb1,  1, Qt::AlignCenter);

    QLineEdit *edit1 = new QLineEdit(this);
    edit1->setAlignment(Qt::AlignCenter);
    edit1->setFixedWidth(180);
    edit1->setPlaceholderText(cn("输入群/讨论组名称"));

    QLineEdit *edit2 = new QLineEdit(this);
    edit2->setText(cn("讨论组"));
    edit2->setReadOnly(true);
    edit2->setAlignment(Qt::AlignCenter);
    edit2->setFixedWidth(180);
    edit2->setPlaceholderText(cn("选择属性"));

    HappyTextEdit *edit3 = new HappyTextEdit(this);
    edit3->setPlaceholderText(cn("写点什么吧，这群是干啥的"));
    edit3->setFixedWidth(180);

    v2->addWidget(edit1, 1, Qt::AlignCenter);
    v2->addWidget(edit2, 1, Qt::AlignCenter);
    v2->addWidget(edit3, 8, Qt::AlignCenter);

    h->addLayout(v1, 2);
    h->addLayout(v2, 3);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(55, 0, 0, 10);
    QPushButton *selectPb = new QPushButton(cn("选择成员"), this);
    selectPb->setCheckable(true);
    selectPb->setFixedWidth(70);
    QPushButton *confirmPb = new QPushButton(cn("确定"), this);
    confirmPb->setFixedWidth(50);
    QPushButton *cancelPb = new QPushButton(cn("取消"), this);
    cancelPb->setFixedWidth(50);
    h1->addStretch(7);
    h1->addWidget(selectPb);
    h1->addWidget(confirmPb);
    h1->addWidget(cancelPb);
    h1->addStretch(1);

    v->addLayout(h, 9);
    v->addLayout(h1, 1);

    QHBoxLayout *h2 = new QHBoxLayout(this);
    h2->setContentsMargins(5, 35, 5, 10);
    CheckList *list = new CheckList(this);
    list->setFixedSize(120, 195);
    list->setVisible(false);
    h2->addLayout(v, 5);
    h2->addWidget(list, 1, Qt::AlignHCenter | Qt::AlignTop);
    h2->addSpacing(15);


    QFile file;
    file.setFileName(":res/css/lineedit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        edit2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pb1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        selectPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        confirmPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        cancelPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/list2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        list->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
    list->setmyuid(conf->getUuid());
    list->addCheckItems(items);

    connect(confirmPb, &QPushButton::clicked, this, [=] () {
        QString name = edit1->text();
        if (name.trimmed().isEmpty())
        {
            MsgBox::ShowMsgBox(cn("错误"),
                               cn("不能使用空的名称"),
                               cn("确定"));
            return;
        }
        QString attr = edit2->text();
        QString des = edit3->toPlainText();
        if (des.trimmed().isEmpty())
        {
            int rlt = MsgBox::ShowMsgBox(cn("提示"),
                               cn("真的不写点什么?"),
                               cn("不写了"), cn("那写点"));
            if (rlt != 0)
                return;
        }
        QString uids = list->checkedids();
        if (uids.split(";").size() <= 1)
        {
            int rlt = MsgBox::ShowMsgBox(cn("提示"),
                               cn("这个组里面只有你一个人\n"
                                  "确定创建这个寂寞的群组么?"),
                               cn("确定"), cn("去选人"));
            if (rlt == 1)
            {
                selectPb->setChecked(true);
                return;
            }

        }
        // check image
        QString newPhotoName = photo->ImagePath();
        QString path = GetWorkPath() + "/face";
        if (QDir().mkpath(path))
        {
            QFile file;
            file.setFileName(newPhotoName);
            if (file.exists())
            {
                QString temp = path + "/" + QFileInfo(newPhotoName).fileName();
                if (QFile::exists(temp))
                {
                    newPhotoName = temp;
                }
                else
                {
                    bool b = file.copy(temp);
                    if (b)
                        newPhotoName = temp;
                }
            }
            file.close();
        }

        // 创建数据
        GroupData gd;
        gd.setName(name);
        gd.setAttr(attr);
        gd.setGroupdescribe(des);
        gd.setPhoto(newPhotoName);
        gd.setGroupmember(uids);
        gd.setUid(getUuid());
        gd.setDeleteable(true);
        gd.setEditable(true);
        QDateTime dt = QDateTime::currentDateTime();
        gd.setCreationtime(dt.toString("yyyy-MM-dd hh:mm:ss"));
        gd.setUpdatetime(dt.toTime_t());
        gd.setCreator(conf->getIni("nickname"));
        gd.setPort(qrand() % 10000 + 20000);
        gd.setNetmask(createNetmask());
        emit confrimCreate(gd);
        close();
    });
    connect(cancelPb, &QPushButton::clicked, this, [=] () {
        emit bar->signalClose();
    });

    connect(selectPb, &QPushButton::toggled, this, [=] (bool b) {
        if (b)
        {
            setFixedWidth(480);
            setTitleBarWidth(480);
        }
        else
        {
            setFixedWidth(380);
            setTitleBarWidth(380);
        }
        list->setVisible(b);
    });

    connect(this, &CreateGroup::updateItems,
            list, &CheckList::addCheckItems);

    connect(pb1, &QPushButton::clicked, this, [=] () {
        EmotionsBox *box = EmotionsBox::GetInstance();
        box->show();
        box->activateWindow();
        QPoint p = QCursor::pos();
        box->move(p.x() - box->width() / 2, p.y() - 20 - box->height());
        connect(box, &EmotionsBox::select, this, [=] (QString str) {
            photo->setMoiveRes(str);
        });
        connect(box, &EmotionsBox::signalHide, this, [=] () {
            disconnect(box, &EmotionsBox::signalHide, 0, 0);
            disconnect(box, &EmotionsBox::select, 0, 0);
        });
    });
    connect(photo, &EmojiLabel::clicked,
            this, [=] () {
        QString filter = cns("图片资源(*.jpg *.gif *.png*.bmp);;全部文件(*.*)");
        QString path = QFileDialog::getOpenFileName(this, QStringLiteral("选择图片"),
                                                    ".", filter);
        if (!QFileInfo(path).isFile() && !QImageReader(path).canRead())
        {
            MsgBox::ShowMsgBox(cn("错误"),
                               cn("不是有效的文件"),
                               cn("确定"));
            return;
        }
        photo->setMoiveRes(path);
    });
}

CreateGroup::~CreateGroup()
{
}

QString CreateGroup::createNetmask()
{
    QStringList list;
    srand((unsigned)time(0));
    list << QString::number(239);
    list << QString::number(qrand() % 230 + 10);
    list << QString::number(qrand() % 230 + 10);
    list << QString::number(qrand() % 230 + 10);
    return list.join(".");
}



InviteWidget::InviteWidget(QStringList items,
                           QString gd, QWidget *parent)
    : THWidgetBase(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(250, 180);
    setTitleBarWidth(250);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(cn("邀请成员列表"));
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
            this->close();
    });
    members = items;
    gdm = gd;
    QLabel *lab = new QLabel(cn("选择邀请对象(未筛选)"));
    lab->setFixedWidth(120);
    lab->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QPushButton *pb = new QPushButton(cn("筛选"), this);
    pb->setFixedWidth(50);
    pb->setCheckable(true);
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->addStretch(1);
    h1->addWidget(lab, 0, Qt::AlignCenter);
    h1->addWidget(pb, 0, Qt::AlignCenter);
    h1->addStretch(1);
    CheckList *list = new CheckList(this);
    QPushButton *confrim = new QPushButton(cn("确认"), this);
    confrim->setFixedWidth(50);
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(10, 35, 10, 15);
    v->addLayout(h1, 1);
    v->addWidget(list, 5);
    v->addWidget(confrim, 1, Qt::AlignCenter);

    QFile file;
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        confrim->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/list2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        list->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
    list->setmyuid(conf->getUuid());

    connect(pb, &QPushButton::toggled,
            this, [=] (bool b) {
        QString str = cn("选择邀请对象(%1)");
        if (b)
        {
            QStringList selectList;
            QStringList items = gdm.split(";");
            for (QString str : members)
            {
                QStringList temp = str.split(":");
                if (temp.size() == 2)
                {
                    if (!items.contains(temp.last()))
                    {
                        selectList << str;
                    }
                }
            }
            list->addCheckItems(selectList);
            str = str.arg(cn("筛选"));
        }
        else
        {
            list->addCheckItems(members);
            str = str.arg(cn("未筛选"));
        }
        lab->setText(str);
    });
    connect(confrim, &QPushButton::clicked,
            this, [=] () {
        QString uids = list->checkedids();
        QStringList lst = uids.split(";");
        if (lst.size() <= 1)
        {
            MsgBox::ShowMsgBox(cn("提示"),
                               cn("没有选择"),
                               cn("确定"));
                return;
        }
        emit confrimInvite(lst);
        this->close();
    });

    list->addCheckItems(items);
}

InviteWidget::~InviteWidget()
{

}
