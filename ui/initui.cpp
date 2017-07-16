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
    // 初始化界面
    QLabel *welcome = new QLabel(cns("欢迎\n工作目录设定"), this);

    QLabel *label = new QLabel(cns("路径"), this);
    mPath = new QLineEdit(this);
    label->setBuddy(mPath);
    QPushButton *select = new QPushButton(cns("选择"), this);
    connect(select, &QPushButton::clicked, this, &InitUI::selectPath);

    mOk = new QPushButton(cns("确定"), this);
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

    // 启动定时器
    connect(mOk, &QPushButton::clicked, &mTimer, &QTimer::stop);
    connect(this, &InitUI::countdownout, &mTimer, &QTimer::stop);
    connect(&mTimer, &QTimer::timeout, this, &InitUI::timeout);
    mTime = COUNTDOWN;
    mTimer.start(1000);
}

void InitUI::selectPath()
{
    QString directory =
            QFileDialog::getExistingDirectory(this,cns("选择目录"));
    if (!directory.isEmpty())
    {
        mPath->setText(directory);
    }
}

void InitUI::selectOk()
{
    // 创建目录
    QString path = mPath->text() + "/" + "iamhappy";
    QDir dir(".");
    QFileInfo info(path);
    if (!info.exists())
    {
        bool ret = dir.mkpath(info.absoluteFilePath());
        if (!ret)
        {
            QMessageBox::information(this, cns("哈啤"), cns("创建目录失败"),
                                     cns("吼啊"));
            return;
        }
    }
    // 创建图片暂存目录
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
        mOk->setText(cns("确定\n倒计时:") + QString::number(mTime) + cns("秒"));
    }
}
