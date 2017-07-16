#include "optionpersonalization.h"
#include "../../util/thmethod.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../config/configuredata.h"
#include "emotionsbox.h"
#include "optionsui.h"
#include "msgbox.h"
#include <QtWidgets>

OptionPersonalization::OptionPersonalization(QWidget *parent) : OptionWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(3, 3, 3, 3);
    v->setSpacing(0);

    edit = new QLineEdit(this);
    edit->setAlignment(Qt::AlignCenter);
    edit->setFixedWidth(150);
    edit->setPlaceholderText(QStringLiteral("修改您的昵称"));

    v->addWidget(edit, 1, Qt::AlignCenter | Qt::AlignTop);

    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    photo = new EmojiLabel(this);
    photo->setMovieable(true);
    photo->setFixedSize(70, 70);
    photo->setStyleSheet(cns("QLabel{border-width:1px;border-style:solid;border-radius:3px;border-color:transparent;"
                             "background:transparent;}"
                             "QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}"
                             "QLabel:pressed{border-width:1px;border-style:solid;border-radius:3px;border-color:rgba(240,240,240,50);}"));
    image = new EmojiLabel(this);
    image->setMovieable(true);
    image->setStyleSheet(cns("QLabel{border-width:1px;border-style:solid;border-radius:3px;border-color:transparent;"
                             "background:transparent;}"
                             "QLabel:hover{border-width:1px;border-style:solid;border-radius:3px;border-color:white;}"
                             "QLabel:pressed{border-width:1px;border-style:solid;border-radius:3px;border-color:rgba(240,240,240,50);}"));
    image->setFixedSize(140, 140);
    h->addWidget(photo, 1, Qt::AlignCenter);
    h->addWidget(image, 2, Qt::AlignCenter);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->setSpacing(0);
    QLabel *lbl1 = new QLabel(QStringLiteral("修改您的头像(点击)"), this);
    lbl1->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QLabel *lbl2 = new QLabel(QStringLiteral("修改您的形象(点击)"), this);
    lbl2->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    h1->addWidget(lbl1, 1, Qt::AlignCenter);
    h1->addWidget(lbl2, 2, Qt::AlignCenter);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    h2->setSpacing(0);
    QPushButton *pb1 = new QPushButton(QStringLiteral("从表情库中选择"), this);
    pb1->setFixedWidth(120);
    QPushButton *pb2 = new QPushButton(QStringLiteral("从表情库中选择"), this);
    pb2->setFixedWidth(120);
    h2->addWidget(pb1, 1, Qt::AlignCenter);
    h2->addWidget(pb2, 2, Qt::AlignCenter);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(pb1, 1);
    group->addButton(pb2, 2);

    v->addLayout(h, 7);
    v->addLayout(h1, 1);
    v->addLayout(h2, 1);

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    photo->setMoiveRes(conf->getIni("photo"));
    image->setMoiveRes(conf->getIni("image"));
    edit->setText(conf->getIni("nickname"));

    QFile file;
    file.setFileName(":res/css/lineedit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pb1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        pb2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    // connect
    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(buttonsClicked(int)));
    connect(photo, &EmojiLabel::clicked, this, &OptionPersonalization::emojiClicked);
    connect(image, &EmojiLabel::clicked, this, &OptionPersonalization::emojiClicked);

    OptionsUI *opt = dynamic_cast<OptionsUI *>(parent);
    connect(opt, &OptionsUI::apply, this, [=] (int tab) {
        if (tab != index)
            return;
        saveModify();
    });
    connect(opt, &OptionsUI::confirm, this, [=] (int tab) {
        if (tab != index)
            return;
        saveModify();
    });
}

OptionPersonalization::~OptionPersonalization()
{

}

void OptionPersonalization::buttonsClicked(int id)
{
    QButtonGroup *group = qobject_cast<QButtonGroup *>(sender());
    if (!group)
        return;
    QPushButton *pb = dynamic_cast<QPushButton *>(group->button(id));
    if (!pb)
        return;
    EmotionsBox *box = EmotionsBox::GetInstance();
    box->show();
    box->activateWindow();
    QPoint p = QCursor::pos();
    box->move(p.x() - box->width() / 2, p.y() - 20 - box->height());
    connect(box, &EmotionsBox::select, this, [=] (QString str) {
        if (id == 1)
        {
            photo->setMoiveRes(str);
        }
        else if (id == 2)
        {
            image->setMoiveRes(str);
        }
    });
    connect(box, &EmotionsBox::signalHide, this, [=] () {
        disconnect(box, &EmotionsBox::signalHide, 0, 0);
        disconnect(box, &EmotionsBox::select, 0, 0);
    });
}

void OptionPersonalization::emojiClicked()
{
    EmojiLabel *emoji = qobject_cast<EmojiLabel *>(sender());
    if (!emoji)
        return;
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
    emoji->setMoiveRes(path);
}

void OptionPersonalization::saveModify()
{
    ConfigureData *conf = ConfigureData::getInstance();
    QString nickname = edit->text();
    if (nickname.trimmed().isEmpty())
    {
        MsgBox::ShowMsgBox(cn("错误"), cn("不能使用空的昵称"), cn("确定"));
        edit->setText(conf->getIni("nickname"));
        return;
    }
    if (nickname.trimmed().length() >= 18)
    {
        MsgBox::ShowMsgBox(cn("错误"), cn("昵称长度18限制"), cn("确定"));
        edit->setText(conf->getIni("nickname"));
        return;
    }
    // 移动所选图片到用户图片下，并重新命名
    QString newPhotoName = photo->ImagePath();
    QString newImageName = image->ImagePath();
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
        file.setFileName(newImageName);
        if (file.exists())
        {
            QString temp = path + "/" + QFileInfo(newImageName).fileName();
            if (QFile::exists(temp))
            {
                newImageName = temp;
            }
            else
            {
                bool b = file.copy(temp);
                if (b)
                    newImageName = temp;
            }
        }
        file.close();
    }
    conf->setIni("photo", newPhotoName);
    conf->setIni("image", newImageName);
    conf->setIni("nickname", edit->text());
}
