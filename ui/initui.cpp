#include "initui.h"
#include "../config/configuredata.h"
#include "../util/thmethod.h"
#include <QtWidgets>
static const int COUNTDOWN = 60;
InitUI::InitUI(QDialog *parent) : QDialog(parent)
{
    init();
}

InitUI::~InitUI()
{

}

void InitUI::init()
{
    // ��ʼ������
    QLabel *welcome = new QLabel(cns("��ӭ\n����Ŀ¼�趨"), this);

    QLabel *label = new QLabel(cns("·��"), this);
    mPath = new QLineEdit(this);
    label->setBuddy(mPath);
    QPushButton *select = new QPushButton(cns("ѡ��"), this);
    connect(select, &QPushButton::clicked, this, &InitUI::selectPath);

    mOk = new QPushButton(cns("ȷ��"), this);
    connect(mOk, &QPushButton::clicked, this, &InitUI::selectOk);

    QVBoxLayout *v = new QVBoxLayout();
    QHBoxLayout *h = new QHBoxLayout();

    v->addWidget(welcome, 2);

    h->addWidget(label, 1);
    h->addWidget(mPath, 4);
    h->addWidget(select, 1);

    v->addLayout(h, 1);
    v->addWidget(mOk, 2);

    setLayout(v);

    // ������ʱ��
    connect(mOk, &QPushButton::clicked, &mTimer, &QTimer::stop);
    connect(this, &InitUI::countdownout, &mTimer, &QTimer::stop);
    connect(&mTimer, &QTimer::timeout, this, &InitUI::timeout);
    mTime = COUNTDOWN;
    mTimer.start(1000);
}

void InitUI::selectPath()
{
    QString directory =
            QFileDialog::getExistingDirectory(this,cns("ѡ��Ŀ¼"));
    if (!directory.isEmpty())
    {
        mPath->setText(directory);
    }
}

void InitUI::selectOk()
{
    // ����Ŀ¼
    QString path = mPath->text() + "/" + "iamhappy";
    QDir dir(".");
    QFileInfo info(path);
    if (!info.exists())
    {
        bool ret = dir.mkpath(info.absoluteFilePath());
        if (!ret)
        {
            QMessageBox::information(this, cns("��ơ"), cns("����Ŀ¼ʧ��"),
                                     cns("��"));
            return;
        }
    }
    // ����ͼƬ�ݴ�Ŀ¼
    dir.mkpath(path + "/" + "images/temp");
//    ConfigureData::getInstance()->writeIni("path", info.absoluteFilePath());

    accept();
}

void InitUI::timeout()
{
    if (mTime <= 0)
    {
        emit countdownout();
        selectOk();
    }
    else
    {
        mTime--;
        mOk->setText(cns("ȷ��\n����ʱ:") + QString::number(mTime) + cns("��"));
    }
}
