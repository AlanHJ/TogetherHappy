#include "msgbox.h"
#include "../happytitlebar.h"
#include <QtWidgets>
#include "../../config/configuredata.h"

MsgBox::MsgBox(QWidget *parent) : THWidgetBase(parent)
{
    id = -1;
    setFixedSize(280, 150);
    setTitleBarWidth(280);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setBarIcon(":res/happy.png");
    bar->setExtraButtonVisible(false);
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        buttonsClicked(1);
    });

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(0, 30, 0, 0);
    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(10, 0, 10, 20);

    contentLbl = new QLabel(this);
    contentLbl->setWordWrap(true);
    contentLbl->setStyleSheet(QStringLiteral("QLabel{font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;}"));
    QButtonGroup *group = new QButtonGroup(this);
    btn1 = new QPushButton(QStringLiteral("È·¶¨"), this);
    btn1->setShortcut(Qt::Key_Return);
    btn1->setFixedSize(60, 25);
    btn2 = new QPushButton(QStringLiteral("È¡Ïû"), this);
    btn2->setShortcut(Qt::Key_Escape);
    btn2->setFixedSize(60, 25);
    btn3 = new QPushButton(this);
    btn3->setFixedSize(60, 25);
    group->addButton(btn1, 0);
    group->addButton(btn2, 1);
    group->addButton(btn3, 2);
    connect(group, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonsClicked(int)));

    edit = new QTextEdit(this);
    edit->setReadOnly(true);
    edit->setVisible(false);
    edit->viewport()->setCursor(QCursor(Qt::ArrowCursor));
    edit->setFixedSize(240, 60);

    h->addWidget(btn1);
    h->addWidget(btn2);
    h->addWidget(btn3);

    v->addWidget(contentLbl, 0, Qt::AlignCenter);
    v->addWidget(edit, 0, Qt::AlignCenter);
    v->addLayout(h);
    btn3->setVisible(false);

    QFile file;
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        btn1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        btn2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        btn3->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":/res/css/edit_read.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
}

MsgBox::~MsgBox()
{
    qDebug() << "Destruct MsgBox";
}

void MsgBox::setCloseButtonVisible(bool b)
{
    bar->setCloseVisible(b);
}

void MsgBox::setTitle(QString title)
{
    bar->setBarContent(title);
}

void MsgBox::setContent(QString content)
{
    if (content.size() >= 50)
    {
//        QTextCharFormat fmt;
//        fmt.setFontFamily(QStringLiteral("Î¢ÈíÑÅºÚ"));
//        fmt.setFontPointSize(8);
//        fmt.setForeground(Qt::white);
//        edit->mergeCurrentCharFormat(fmt);
        contentLbl->setVisible(false);
        edit->setVisible(true);
        edit->setText(content);
    }
    else
    {
        if (content.size() <= 15)
        {
            contentLbl->setStyleSheet(QStringLiteral("QLabel{font-family:Î¢ÈíÑÅºÚ;font:14px;color:white;}"));
        }
        else
        {
            contentLbl->setStyleSheet(QStringLiteral("QLabel{font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;}"));
        }
        contentLbl->setVisible(true);
        edit->setVisible(false);
        contentLbl->setText(content);

    }
}

void MsgBox::setButtons(QString bt1, QString bt2, QString bt3)
{
    btn1->setVisible(!bt1.isEmpty());
    btn1->setText(bt1);

    btn2->setVisible(!bt2.isEmpty());
    btn2->setText(bt2);

    btn3->setVisible(!bt3.isEmpty());
    btn3->setText(bt3);
}

int MsgBox::exec()
{
    setWindowModality(Qt::ApplicationModal);
    show();
    activateWindow();
    loop.exec(QEventLoop::DialogExec);
    return id;
}

int MsgBox::ShowMsgBox(QString t, QString c, QString bt1, QString bt2, QString bt3)
{
    MsgBox *box = new MsgBox();
    box->setTitle(t);
    box->setContent(c);
    box->setButtons(bt1, bt2, bt3);
    return box->exec();
}

void MsgBox::ShowBRbox(QString t, QString c)
{
    MsgBox *box = new MsgBox();
    box->setTitle(t);
    box->setContent(c);
    box->setButtons();
    box->show();
    box->activateWindow();
    int w = qApp->desktop()->width();
    int h = qApp->desktop()->height();
    box->move(w - 300, h - 200);
}

void MsgBox::buttonsClicked(int id)
{
    this->id = id;
    loop.exit();
    this->close();
    this->deleteLater();
}



