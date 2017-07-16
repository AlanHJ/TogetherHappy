#include "loademojiui.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include "moveemojicustom.h"
#include "emojiinstance.h"
#include <QtWidgets>

LoadEmojiUI::LoadEmojiUI(QWidget *parent) : QWidget(parent)
{
    init();
    setWindowFlags(Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(cns("哈啤表情导入"));
}

LoadEmojiUI::~LoadEmojiUI()
{

}

void LoadEmojiUI::clickLoad()
{
//    QString dirctory =
//            QFileDialog::getExistingDirectory(this, cns("选择表情包路径"));
//    QStringList filters;
//    filters << "*.jpg" << "*.gif" << "*.bmp"
//                 << "*.jpeg" << "*.png" << "*.pbm"
//                 << "*.pgm" << "*.ppm" << "*.xbm" << "*.xpm";
//    QDir dirs(dirctory);
//    QFileInfoList infolst = dirs.entryInfoList(filters, QDir::Files);
    QString filter = "(*.jpg);;(*.png);;(*.gif);;(*.bmp);;(*)";
    QStringList infolst = QFileDialog::getOpenFileNames(this, cns("选择表情包"), ".", filter);

    QStringList imagelist;
    auto it = infolst.begin();
    while (it != infolst.end())
    {
        QFileInfo info = *it;
        if (info.exists())
        {
            qint64 size = info.size();
            if (size > mValue)
            {
                imagelist << info.filePath();
            }
//            qDebug() << info.filePath() + " size:" + QString::number(size);
        }
        ++it;
    }



    QString path = "D:/Program Files/happy";
    QDir dir(".");
    QFileInfo info(path);
    if (!info.exists())
    {
        dir.mkpath(info.absoluteFilePath());
    }
    if (imagelist.isEmpty())
    {
//        mBar->setValue(100);
    }
    QFile file(path +  "/emoji.data");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        for (int i = 0; i < imagelist.size(); i++)
        {
            QString image = imagelist.at(i) + ";";
            file.seek(file.size());
            file.write(image.toLocal8Bit());
//            float num = i;
//            float counts = imagelist.size();
//            mBar->setValue(num / (counts - 1) * 100);
        }
    }

    file.close();
    emit MoveEmojiCustom::GetInstance()->signalStartMoves(imagelist);
}

void LoadEmojiUI::valueChanged(int value)
{
    if (value == 0)
    {
        mValue = 0;
    }
    else
    {
        mValue = 1024 * 0.1 + (value - 1) * 1024 / 10.0;
    }

    float kbs = mValue / 1024.0;
    mLabel->setText(cns("当前:%1Kb").arg(QString::number(kbs), 'f', 2));
}

void LoadEmojiUI::progressValue(int value)
{
    if (value == 101)
    {
        EmojiInstance::GetInstance()->reloadEmoji();
    }
    mBar->setValue(value);
}

void LoadEmojiUI::init()
{
    mValue = 0;

    QVBoxLayout *v = new QVBoxLayout();

    QHBoxLayout *h = new QHBoxLayout();
    QLabel *label = new QLabel(cns("请选择过滤文件的大小"), this);
    mSlider = new QSlider(Qt::Horizontal, this);
    mSlider->setMaximum(100);
    mSlider->setMinimum(0);
    mSlider->setValue(0);
    connect(mSlider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    mLabel = new QLabel(cns("当前:0.0Kb"), this);
    h->addWidget(label);
    h->addSpacing(2);
    h->addWidget(mLabel);

    v->addLayout(h);
    v->addWidget(mSlider);

    mBar = new QProgressBar(this);
    mBar->setMaximum(100);
    mBar->setMinimum(0);
    MoveEmojiCustom *movee = MoveEmojiCustom::GetInstance();
    connect(movee, SIGNAL(signalProgress(int)), this, SLOT(progressValue(int)));
    QPushButton *load = new QPushButton(cns("选择目录"), this);
    connect(load, SIGNAL(clicked()), this, SLOT(clickLoad()));
    v->addWidget(mBar);
    v->addWidget(load, 0, Qt::AlignCenter);
    setLayout(v);
    this->setStyle(QStyleFactory::create(QStyleFactory::keys().last()));
}

