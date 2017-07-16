#include "abouthp.h"
#include "../../thwidgets/emoji/emojilabel.h"
#include "../../config/configuredata.h"
#include "../happytitlebar.h"
#include "../../util/thmethod.h"
#include <QtWidgets>

AboutHP::AboutHP(QWidget *parent) : THWidgetBase(parent)
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
    bar->setBarContent(cn("关于哈啤"));
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
            this->close();
    });

    QHBoxLayout *h = new QHBoxLayout(this);
    h->setContentsMargins(5, 35, 5, 15);

    QVBoxLayout *v1= new QVBoxLayout();
    v1->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *v2= new QVBoxLayout();
    v2->setContentsMargins(0, 0, 0, 0);
    EmojiLabel *photo = new EmojiLabel(this);
    photo->setMoiveRes(":res/ui3/about_photo.gif");
    photo->setMovieable(true);
    photo->setFixedSize(70, 70);
    QLabel *lbl1 = new QLabel(cn("谢谢大家"), this);
    lbl1->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    v1->addWidget(photo, 8, Qt::AlignCenter);
    v1->addWidget(lbl1,  1, Qt::AlignCenter);

    QTextEdit *edit = new QTextEdit(this);
    edit->setReadOnly(true);
    edit->viewport()->setCursor(QCursor(Qt::ArrowCursor));
    edit->setFixedWidth(200);

    QPushButton *btn = new QPushButton(QStringLiteral("更早的信息"), this);
    btn->setFixedWidth(100);
    v2->addWidget(edit, 9, Qt::AlignCenter);
    v2->addWidget(btn, 1, Qt::AlignCenter);

    h->addLayout(v1, 2);
    h->addLayout(v2, 3);

    QFile file;
    file.setFileName(":/res/css/edit_read.css");
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
        btn->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    file.setFileName(":res/updatestr/update3str.htm");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit->setText(ba);
    }
    file.close();

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

    connect(btn, &QPushButton::clicked, this, [=] () {
        QFile file(":res/updatestr/str.data");
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray ba = file.readAll();
            QString str = QTextCodec::codecForLocale()->toUnicode(ba);
            QString html = edit->toHtml();
            edit->clear();
            edit->setText(str + "\n");
            edit->append(html);
        }
        file.close();
    });
}

AboutHP::~AboutHP()
{

}

