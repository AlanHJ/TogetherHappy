#include "fileui.h"
#include "../util/thmethod.h"
#include "../tcp/file/filesendrecvopt.h"
#include <QDebug>
#include <QtWidgets>
#include <QStatusBar>

class FileUI::PrivData
{
public:
    PrivData()
    {
        opt = new FileSendRecvOpt();
    }
    ~PrivData()
    {
        if (nullptr != opt)
        {
            delete opt;
            opt = nullptr;
        }
    }
public:
    FileSendRecvOpt *opt;
};

FileUI::FileUI(QWidget *parent)
    : QWidget(parent)
{
    mData = new PrivData();
    // 开始创建界面UI

    // 先创建接收界面
    QFrame *frame1 = new QFrame(this);
    frame1->setWindowIconText(cns("接收"));
    QVBoxLayout *v1 = new QVBoxLayout();
    QLabel *l1 = new QLabel(cns("接收总进度"), frame1);
    QLabel *l2 = new QLabel(cns("当前文件进度"), frame1);
    QProgressBar *bar1 = new QProgressBar(frame1);
    bar1->setMaximum(100);
    bar1->setMinimum(0);
    bar1->setValue(0);
    connect(this, SIGNAL(signalRecvTotalProgress(int)),
            bar1, SLOT(setValue(int)));
    QProgressBar *bar2 = new QProgressBar(frame1);
    bar2->setMaximum(100);
    bar2->setMinimum(0);
    bar2->setValue(0);
    connect(this, SIGNAL(signalRecvCurrentProgress(int)),
            bar2, SLOT(setValue(int)));
    mRecvMsg = new QLabel(cns("接收消息..."), frame1);
    QFont f;
    f.setPixelSize(10);
    mRecvMsg->setFont(f);

    QPushButton *pb1 = new QPushButton(cns("停止接收"), frame1);
    connect(pb1, SIGNAL(clicked()), this, SLOT(stopRecv()));
    QPushButton *pb4 = new QPushButton(cns("打开文件夹"), frame1);
    connect(pb4, SIGNAL(clicked()), this, SLOT(openPath()));

    mRecvList = new QListWidget(frame1);

    QHBoxLayout *rhb1 = new QHBoxLayout();
    QHBoxLayout *rhb2 = new QHBoxLayout();

    rhb1->addWidget(l1, 0, Qt::AlignCenter);
    rhb1->addWidget(bar1, 0, Qt::AlignCenter);
    rhb2->addWidget(l2, 0, Qt::AlignCenter);
    rhb2->addWidget(bar2, 0, Qt::AlignCenter);

    v1->addLayout(rhb1);
    v1->addLayout(rhb2);
    v1->addWidget(mRecvMsg, 0, Qt::AlignLeft | Qt::AlignTop);

    v1->addWidget(mRecvList);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->addWidget(pb1);
    h2->addWidget(pb4);

    v1->addLayout(h2);
    frame1->setLayout(v1);

    QStatusBar *status1 = new QStatusBar(frame1);
    mRecvLabel = new QLabel(cns("接收速度:0MB/s"), frame1);
    status1->addPermanentWidget(mRecvLabel);
    frame1->layout()->addWidget(status1);



    // 再创建发送界面
    QFrame *frame2 = new QFrame(this);
    frame2->setWindowIconText(cns("发送"));
    QLabel *l4 = new QLabel(cns("发送总进度"), frame2);
    QLabel *l5 = new QLabel(cns("当前文件进度"), frame2);
    QProgressBar *bar3 = new QProgressBar(frame2);
    bar3->setMaximum(100);
    bar3->setMinimum(0);
    bar3->setValue(0);
    connect(this, SIGNAL(signalSendTotalProgress(int)),
            bar3, SLOT(setValue(int)));
    QProgressBar *bar4 = new QProgressBar(frame2);
    bar4->setMaximum(100);
    bar4->setMinimum(0);
    bar4->setValue(0);
    connect(this, SIGNAL(signalSendCurrentProgress(int)),
            bar4, SLOT(setValue(int)));
    mSendMsg = new QLabel(cns("发送消息..."), frame2);
    mSendMsg->setFont(f);

    QPushButton *pb2 = new QPushButton(cns("开始发送"), frame2);
    connect(pb2, SIGNAL(clicked()), this, SLOT(askSend()));
    QPushButton *pb3 = new QPushButton(cns("停止发送"), frame2);
    connect(pb3, SIGNAL(clicked()), this, SLOT(stopSend()));
    mClear = new QPushButton(cns("清空列表"), frame2);
    connect(mClear, SIGNAL(clicked()), this, SLOT(clearList()));

    mList = new QListWidget(frame2);
    connect(mList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(itemDoubleClicked(QListWidgetItem*)));

    QVBoxLayout *v2 = new QVBoxLayout();

    QHBoxLayout *shb1 = new QHBoxLayout();
    QHBoxLayout *shb2 = new QHBoxLayout();

    shb1->addWidget(l4, 0, Qt::AlignCenter);
    shb1->addWidget(bar3, 0, Qt::AlignCenter);
    shb2->addWidget(l5, 0, Qt::AlignCenter);
    shb2->addWidget(bar4, 0, Qt::AlignCenter);
    v2->addLayout(shb1);
    v2->addLayout(shb2);
    v2->addWidget(mSendMsg, 0, Qt::AlignLeft | Qt::AlignTop);

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->addWidget(pb2);
    h1->addWidget(pb3);
    h1->addWidget(mClear);

    v2->addWidget(mList);
    v2->addLayout(h1);
    frame2->setLayout(v2);

    QStatusBar *status2 = new QStatusBar(frame2);
    mSendLabel = new QLabel(cns("发送速度:0MB/s"), frame2);
    status2->addPermanentWidget(mSendLabel);
    frame2->layout()->addWidget(status2);

    QVBoxLayout *v3 = new QVBoxLayout();
    v3->addWidget(frame1);
    QSpacerItem *item = new QSpacerItem(192, 10);
    v3->addSpacerItem(item);
    v3->addWidget(frame2);
    setLayout(v3);

//    resize(192, 372);

    // 连接信息
    connect(mData->opt, SIGNAL(recvInformation(QString,QString)),
            this, SLOT(recvInfo(QString,QString)));
    connect(mData->opt, SIGNAL(sendInformation(QString,QString)),
            this, SLOT(sendInfo(QString,QString)));
    connect(mData->opt, SIGNAL(recvFilesProgress(int)),
            this, SIGNAL(signalRecvTotalProgress(int)));
    connect(mData->opt, SIGNAL(recvCurrentFileProgress(int)),
            this, SIGNAL(signalRecvCurrentProgress(int)));
    connect(mData->opt, SIGNAL(sendFilesProgress(int)),
            this, SIGNAL(signalSendTotalProgress(int)));
    connect(mData->opt, SIGNAL(sendCurrentFileProgress(int)),
            this, SIGNAL(signalSendCurrentProgress(int)));
    connect(this, SIGNAL(signalStartSend()),
            this, SLOT(startSend()));

    connect(mData->opt, SIGNAL(signalRecvPreInfo(QStringList)),
            this, SLOT(insertRecvFile(QStringList)));
    connect(mData->opt, SIGNAL(signalRecvStart()),
            this, SLOT(recvstart()));
    connect(mData->opt, SIGNAL(signalRecvSuccess()),
            this, SLOT(recvsuccess()));
    connect(mData->opt, SIGNAL(signalSendStart()),
            this, SLOT(sendstart()));
    connect(mData->opt, SIGNAL(signalSendSuccess()),
            this, SLOT(sendsuccess()));

    connect(mData->opt, SIGNAL(sendFileSpeed(float)),
            this, SLOT(sendSpeed(float)));
    connect(mData->opt, SIGNAL(recvFileSpeed(float)),
            this, SLOT(recvSpeed(float)));
}

FileUI::~FileUI()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void FileUI::setKey(QString key)
{
    mData->opt->setKey(key);
}

void FileUI::setSavePath(QString path)
{
    mData->opt->fileSavePath(path);
}

void FileUI::insertFile(QString file)
{
    mData->opt->addFileName(file);
    mList->addItem(file);
}

void FileUI::insertPath(QString path)
{
    mData->opt->addFilePath(path);
    mList->addItem(path);
}

void FileUI::stopRecv()
{
    mData->opt->stopRecv();
}

void FileUI::startSend()
{
    mList->setEnabled(false);
    mData->opt->startSend();
    emit signalSendCurrentProgress(0);
    emit signalSendTotalProgress(0);
}

void FileUI::askSend()
{
    if (mData->opt->isEmpty())
    {
        return;
    }
    emit signalAskSend();
    mSendMsg->setText(cns("等待对方确认"));
}

void FileUI::stopSend()
{
    mList->setEnabled(true);
    mData->opt->stopSend();
}

void FileUI::clearList()
{
    mList->clear();
    mData->opt->clearAllPrepareFile();
}

void FileUI::openPath()
{
    QString path = mData->opt->getSavePath();
    path.replace("/", "\\");
    QProcess::startDetached("explorer " + path);
}

void FileUI::itemDoubleClicked(QListWidgetItem *item)
{
    mData->opt->clearPrepareFileByName(item->text());
    delete item;
}

void FileUI::recvInfo(QString type, QString info)
{
    mRecvMsg->setText(type + ":" + info);
}

void FileUI::sendInfo(QString type, QString info)
{
    mSendMsg->setText(type + ":" + info);
}

void FileUI::insertRecvFile(QStringList lst)
{
    QString path = mData->opt->getSavePath();
    for (QString str : lst)
    {
        QFileInfo info(str);
        if (info.isFile())
        {
            mRecvList->addItem(path + info.fileName());
        }
        else if (info.isDir())
        {
            QDir dir(str);
            dir.cdUp();
            QString temp = str.replace(dir.absolutePath() + "/", path);
            mRecvList->addItem(temp);
        }
        else
        {
            QString temp = str.split("/").last();
            mRecvList->addItem(path + temp);
        }
    }
}

void FileUI::recvstart()
{
    mRecvList->clear();
    emit signalRecv();
}

void FileUI::recvsuccess()
{
    stopRecv();
}

void FileUI::sendstart()
{

}

void FileUI::sendsuccess()
{
    stopSend();
}

void FileUI::sendSpeed(float mbs)
{
    mSendLabel->setText(cns("发送速度") + QString::number(mbs, 'f', 3) + "MB/s");
}

void FileUI::recvSpeed(float mbs)
{
    mRecvLabel->setText(cns("接收速度") + QString::number(mbs, 'f', 3) + "MB/s");
}


void FileUI::resizeEvent(QResizeEvent *)
{
//    qDebug() << "w:" << width() << " h:" << height();
}
