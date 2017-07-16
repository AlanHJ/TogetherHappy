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
    bar->setBarContent(cn("����Ⱥ/������"));
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
    QLabel *lbl1 = new QLabel(cn("�޸�ȺͼƬ(���)"), this);
    lbl1->setStyleSheet(QStringLiteral("font-family:΢���ź�;font:12px;color:white;"));
    QPushButton *pb1 = new QPushButton(cn("�ӱ������ѡ��"), this);
    pb1->setFixedWidth(120);
    v1->addWidget(photo, 8, Qt::AlignCenter);
    v1->addWidget(lbl1,  1, Qt::AlignCenter);
    v1->addWidget(pb1,  1, Qt::AlignCenter);

    QLineEdit *edit1 = new QLineEdit(this);
    edit1->setAlignment(Qt::AlignCenter);
    edit1->setFixedWidth(180);
    edit1->setPlaceholderText(cn("����Ⱥ/����������"));

    QLineEdit *edit2 = new QLineEdit(this);
    edit2->setText(cn("������"));
    edit2->setReadOnly(true);
    edit2->setAlignment(Qt::AlignCenter);
    edit2->setFixedWidth(180);
    edit2->setPlaceholderText(cn("ѡ������"));

    HappyTextEdit *edit3 = new HappyTextEdit(this);
    edit3->setPlaceholderText(cn("д��ʲô�ɣ���Ⱥ�Ǹ�ɶ��"));
    edit3->setFixedWidth(180);

    v2->addWidget(edit1, 1, Qt::AlignCenter);
    v2->addWidget(edit2, 1, Qt::AlignCenter);
    v2->addWidget(edit3, 8, Qt::AlignCenter);

    h->addLayout(v1, 2);
    h->addLayout(v2, 3);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(55, 0, 0, 10);
    QPushButton *selectPb = new QPushButton(cn("ѡ���Ա"), this);
    selectPb->setCheckable(true);
    selectPb->setFixedWidth(70);
    QPushButton *confirmPb = new QPushButton(cn("ȷ��"), this);
    confirmPb->setFixedWidth(50);
    QPushButton *cancelPb = new QPushButton(cn("ȡ��"), this);
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
            MsgBox::ShowMsgBox(cn("����"),
                               cn("����ʹ�ÿյ�����"),
                               cn("ȷ��"));
            return;
        }
        QString attr = edit2->text();
        QString des = edit3->toPlainText();
        if (des.trimmed().isEmpty())
        {
            int rlt = MsgBox::ShowMsgBox(cn("��ʾ"),
                               cn("��Ĳ�д��ʲô?"),
                               cn("��д��"), cn("��д��"));
            if (rlt != 0)
                return;
        }
        QString uids = list->checkedids();
        if (uids.split(";").size() <= 1)
        {
            int rlt = MsgBox::ShowMsgBox(cn("��ʾ"),
                               cn("���������ֻ����һ����\n"
                                  "ȷ�����������į��Ⱥ��ô?"),
                               cn("ȷ��"), cn("ȥѡ��"));
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

        // ��������
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
        QString filter = cns("ͼƬ��Դ(*.jpg *.gif *.png*.bmp);;ȫ���ļ�(*.*)");
        QString path = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ��ͼƬ"),
                                                    ".", filter);
        if (!QFileInfo(path).isFile() && !QImageReader(path).canRead())
        {
            MsgBox::ShowMsgBox(cn("����"),
                               cn("������Ч���ļ�"),
                               cn("ȷ��"));
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
    bar->setBarContent(cn("�����Ա�б�"));
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
            this->close();
    });
    members = items;
    gdm = gd;
    QLabel *lab = new QLabel(cn("ѡ���������(δɸѡ)"));
    lab->setFixedWidth(120);
    lab->setStyleSheet(QStringLiteral("font-family:΢���ź�;font:12px;color:white;"));
    QPushButton *pb = new QPushButton(cn("ɸѡ"), this);
    pb->setFixedWidth(50);
    pb->setCheckable(true);
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->addStretch(1);
    h1->addWidget(lab, 0, Qt::AlignCenter);
    h1->addWidget(pb, 0, Qt::AlignCenter);
    h1->addStretch(1);
    CheckList *list = new CheckList(this);
    QPushButton *confrim = new QPushButton(cn("ȷ��"), this);
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
        QString str = cn("ѡ���������(%1)");
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
            str = str.arg(cn("ɸѡ"));
        }
        else
        {
            list->addCheckItems(members);
            str = str.arg(cn("δɸѡ"));
        }
        lab->setText(str);
    });
    connect(confrim, &QPushButton::clicked,
            this, [=] () {
        QString uids = list->checkedids();
        QStringList lst = uids.split(";");
        if (lst.size() <= 1)
        {
            MsgBox::ShowMsgBox(cn("��ʾ"),
                               cn("û��ѡ��"),
                               cn("ȷ��"));
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
