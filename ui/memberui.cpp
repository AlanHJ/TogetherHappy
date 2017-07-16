#include "memberui.h"
#include "../util/thmethod.h"
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

MemberUI::MemberUI(QWidget *parent) : QWidget(parent)
{
    init();
}

MemberUI::~MemberUI()
{

}

void MemberUI::setTitle(QString title)
{
    mLab->setText(title);
}

void MemberUI::setPbTitle(QString title)
{
    mPb->setText(title);
}

void MemberUI::setBtVisible(bool b)
{
    mPb->setVisible(b);
}

void MemberUI::setMembers(QStringList lst)
{
    for (QString str : lst)
    {
        QStringList templst = str.split(";");
        QCheckBox *c = new QCheckBox(templst.first().remove("\n"), this);
        c->setStyleSheet("QCheckBox {color: white;}");
        QListWidgetItem *item = new QListWidgetItem(mList);
        c->setObjectName(templst.last());
        mList->setItemWidget(item, c);
    }
}

QStringList MemberUI::getChoiceMembers()
{
    QStringList lst;
    for (int i = 0; i < mList->count(); i++)
    {
        QListWidgetItem *item = mList->item(i);
        if (nullptr != item)
        {
            QWidget *w =  mList->itemWidget(item);
            if (nullptr != w)
            {
                QCheckBox *c = qobject_cast<QCheckBox *>(w);
                if (nullptr != c)
                {
                    if (c->isChecked())
                    {
                        // 仅获取IP地址
//                        QString temp = c->text();
//                        int start = temp.indexOf("(");
//                        int len = temp.indexOf(")") - start;
//                        QString ip = temp.mid(start + 1, len - 1);
                        lst << c->objectName();
                    }
                }
            }
        }
    }
    return lst;
}

void MemberUI::okCilcked()
{
    emit choices(getChoiceMembers());
    close();
}

void MemberUI::init()
{
    mLab = new QLabel(cns("这些就是你在线的小伙伴啦"), this);
    mList = new QListWidget(this);
    mPb = new QPushButton(cns("发送邀请"), this);
    connect(mPb, SIGNAL(clicked()), this, SLOT(okCilcked()));
    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    v->addWidget(mLab, 0, Qt::AlignCenter);
    v->addWidget(mList);
    v->addWidget(mPb, 0, Qt::AlignCenter);
    mPb->setVisible(false);
    setLayout(v);
    mList->setStyleSheet("QListView {color: white;}");
}

