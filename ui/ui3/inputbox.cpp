#include "inputbox.h"
#include "../happytitlebar.h"
#include "../../config/configuredata.h"
#include <QtWidgets>

InputBox::InputBox(QWidget *parent) : THWidgetBase(parent)
{
    isOk = false;
    setFixedSize(280, 150);
    setTitleBarWidth(280);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setBarIcon(":res/happy.png");
    bar->setExtraButtonVisible(false);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(0, 30, 0, 5);

    edit = new QLineEdit(this);
    edit->setAlignment(Qt::AlignCenter);
    edit->setFixedWidth(150);
    edit->setPlaceholderText(QStringLiteral("请输入..."));

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->setSpacing(0);
    pb1 = new QPushButton(QStringLiteral("确定"), this);
    pb1->setFixedWidth(80);
    pb2 = new QPushButton(QStringLiteral("取消"), this);
    pb2->setFixedWidth(80);
    h1->addStretch(1);
    h1->addWidget(pb1, 1, Qt::AlignCenter);
    h1->addStretch(1);
    h1->addWidget(pb2, 1, Qt::AlignCenter);
    h1->addStretch(1);

    v->addSpacing(3);
    v->addWidget(edit, 1, Qt::AlignCenter/* | Qt::AlignTop*/);
    v->addLayout(h1, 1);
    v->addSpacing(5);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(pb1, 1);
    group->addButton(pb2, 2);

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));

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
    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        isOk = false;
        loop.exit();
        this->close();
        this->deleteLater();
    });

    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(buttonsClicked(int)));

    edit->setFocus();
}

InputBox::~InputBox()
{

}

void InputBox::setEchoMode(QLineEdit::EchoMode mode)
{
    edit->setEchoMode(mode);
}

void InputBox::setTitle(QString title)
{
    bar->setBarContent(title);
}

void InputBox::setPlaceHoldTxt(QString txt)
{
    edit->setPlaceholderText(txt);
}

QString InputBox::exec()
{
    show();
    activateWindow();
    loop.exec(QEventLoop::DialogExec);
    return edit->text();
}

QString InputBox::GetInput(QString title, bool *ok, QLineEdit::EchoMode mode, QString holdtxt)
{
    InputBox *box = new InputBox();
    box->setTitle(title);
    box->setEchoMode(mode);
    box->setPlaceHoldTxt(holdtxt);
    QString str = box->exec();
    *ok = box->isOk;
    return str;
}

void InputBox::buttonsClicked(int id)
{
    if (id == 2)
    {
        isOk = false;
        edit->clear();
    }
    else
    {
        isOk = true;
    }
    loop.exit();
    this->close();
    this->deleteLater();
}

