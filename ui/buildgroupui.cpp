#include "buildgroupui.h"
#include "memberui.h"
#include "notifyui.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QtWidgets>

BuildGroupUI::BuildGroupUI(QWidget *parent) : QWidget(parent)
{
    init();
    setWindowTitle(cns("�½�Ⱥ"));
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
        NotifyUI::ShowNotify(cns("���Ʋ��ܹ���"));
//        QMessageBox::information(this, cns("��ʾ"), cns("�ǳƲ��ܹ���"), cns("��"));
        return;
    }
    if (title.isEmpty() || title.isNull())
    {
        NotifyUI::ShowNotify(cns("Ⱥ���Ʋ���Ϊ��"));
//        QMessageBox::information(this, cns("��ʾ"), cns("�ǳƲ���Ϊ��"), cns("��"));
        return;
    }
    QStringList lst = mMembers->getChoiceMembers();
    QString uuid = getUuid();
    emit signalCreateGroup(title, uuid, lst, 1);
    close();
}

void BuildGroupUI::init()
{
    // ��ʼ������
    QLabel *l = new QLabel(cns("Ⱥ������"), this);
    mEd = new QLineEdit(this);
    QFont font;
    font.setPointSize(8);
    font.setPixelSize(8);
    mEd->setFont(font);
    mEd->setPlaceholderText(cns("���һ�����߽�����Ҫһ������"));
    mMembers = new MemberUI(this);
    QPushButton *p = new QPushButton(cns("û��,������"), this);
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

