#include "buildgroupui.h"
#include "memberui.h"
#include "notifyui.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QtWidgets>

BuildGroupUI::BuildGroupUI(QWidget *parent) : QWidget(parent)
{
    init();
    setWindowTitle(cns("新建群"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
}

BuildGroupUI::~BuildGroupUI()
{

}

void BuildGroupUI::setMembers(QStringList lst)
{
    mMembers->setMembers(lst);
}

void BuildGroupUI::sureClicked()
{
    QString title = mEd->text();
    if (title.length() > 50)
    {
        NotifyUI::ShowNotify(cns("名称不能过长"));
//        QMessageBox::information(this, cns("提示"), cns("昵称不能过长"), cns("吼啊"));
        return;
    }
    if (title.isEmpty() || title.isNull())
    {
        NotifyUI::ShowNotify(cns("群名称不能为空"));
//        QMessageBox::information(this, cns("提示"), cns("昵称不能为空"), cns("吼啊"));
        return;
    }
    QStringList lst = mMembers->getChoiceMembers();
    QString uuid = getUuid();
    emit signalCreateGroup(title, uuid, lst, 1);
    close();
}

void BuildGroupUI::init()
{
    // 初始化界面
    QLabel *l = new QLabel(cns("群的名字"), this);
    mEd = new QLineEdit(this);
    QFont font;
    font.setPointSize(8);
    font.setPixelSize(8);
    mEd->setFont(font);
    mEd->setPlaceholderText(cns("大家一起行走江湖需要一个名号"));
    mMembers = new MemberUI(this);
    QPushButton *p = new QPushButton(cns("没错,创建吧"), this);
    connect(p, SIGNAL(clicked()), this, SLOT(sureClicked()));

    QVBoxLayout *v = new QVBoxLayout();
    v->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(l, 1);
    h->addWidget(mEd, 9);
    v->addLayout(h);
    v->addWidget(mMembers);
    v->addWidget(p, 0, Qt::AlignCenter);
    setLayout(v);
}

