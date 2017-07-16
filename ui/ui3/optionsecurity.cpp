#include "optionsecurity.h"
#include "msgbox.h"
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include <QtWidgets>

OptionSecurity::OptionSecurity(QWidget *parent) : OptionWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(3, 3, 60, 3);
    v->setSpacing(10);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->setSpacing(0);
    QLabel *lbe1 = new QLabel(QStringLiteral("原密码:"), this);
    lbe1->setFixedWidth(60);
    lbe1->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QLineEdit *edit1 = new QLineEdit(this);
    edit1->setFixedWidth(150);
    edit1->setAlignment(Qt::AlignCenter);
    edit1->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    h1->addStretch(3);
    h1->addWidget(lbe1);
    h1->addWidget(edit1);
    h1->addStretch(5);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    h2->setSpacing(0);
    QLabel *lbe2 = new QLabel(QStringLiteral("新密码:"), this);
    lbe2->setFixedWidth(60);
    lbe2->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QLineEdit *edit2 = new QLineEdit(this);
    edit2->setFixedWidth(150);
    edit2->setAlignment(Qt::AlignCenter);
    edit2->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    h2->addStretch(3);
    h2->addWidget(lbe2);
    h2->addWidget(edit2);
    h2->addStretch(5);

    QHBoxLayout *h3 = new QHBoxLayout();
    h3->setContentsMargins(0, 0, 0, 0);
    h3->setSpacing(0);
    QLabel *lbe3 = new QLabel(QStringLiteral("再次确定:"), this);
    lbe3->setFixedWidth(60);
    lbe3->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QLineEdit *edit3 = new QLineEdit(this);
    edit3->setFixedWidth(150);
    edit3->setAlignment(Qt::AlignCenter);
    edit3->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    h3->addStretch(3);
    h3->addWidget(lbe3, 1);
    h3->addWidget(edit3, 4);
    h3->addStretch(5);

    QLabel *lbe4 = new QLabel(QStringLiteral("锁定密码"), this);
    lbe4->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QPushButton *pb = new QPushButton(QStringLiteral("确定"), this);
    pb->setFixedWidth(60);

    v->addWidget(lbe4, 0, Qt::AlignCenter);
    v->addSpacing(10);
    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
    v->addWidget(pb, 0, Qt::AlignRight);
    v->addStretch(5);

    QFile file;
    file.setFileName(":res/css/lineedit.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        edit1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        edit2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        edit3->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();

    connect(pb, &QPushButton::clicked, this, [=] () {
        QString pw = ConfigureData::getInstance()->getPassword();
        if (pw != edit1->text())
        {
            MsgBox::ShowMsgBox(cns("错误"), cns("原密码错误"), cns("确定"));
            return;
        }
        QString pw1 = edit2->text();
        QString pw2 = edit3->text();
        if (pw1 != pw2)
        {
            MsgBox::ShowMsgBox(cns("错误"), cns("两次输入不一致"), cns("确定"));
            return;
        }
        ConfigureData::getInstance()->setPassword(pw1);
        MsgBox::ShowMsgBox(cns("提示"), cns("修改成功"), cns("确定"));
        edit1->clear();
        edit2->clear();
        edit3->clear();
    });
}

OptionSecurity::~OptionSecurity()
{

}

