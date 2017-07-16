#include "happyprogressui.h"
#include "../util/thmethod.h"
#include <QTimer>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QProgressBar>

HappyProgressUI::HappyProgressUI(QWidget *parent) : QWidget(parent)
{
    init();
    setWindowTitle(cns("哈啤的进度条君"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
}

HappyProgressUI::~HappyProgressUI()
{

}

void HappyProgressUI::setShowName(QString name)
{
    mLab1->setText(name);
}

void HappyProgressUI::setProgressInfo(QString info)
{
    mLab2->insertPlainText(info + "\n");
    mLab2->moveCursor(QTextCursor::End);
}

void HappyProgressUI::slotProgress(int value)
{
    if (value >= 101)
    {
        setProgressInfo(cns("完成, 3秒后自动关闭本界面"));
        QTimer::singleShot(3000, this, SLOT(closeUI()));
    }
}

void HappyProgressUI::closeUI()
{
    close();
}

void HappyProgressUI::init()
{
    QVBoxLayout *v = new QVBoxLayout();
    mLab1 = new QLabel(this);
    mLab2 = new QTextBrowser(this);
    QProgressBar *bar = new QProgressBar(this);
    v->addWidget(mLab1, 0, Qt::AlignCenter);
    v->addSpacing(2);
    v->addWidget(bar), 0, Qt::AlignCenter;
    v->addWidget(mLab2, 0, Qt::AlignCenter);
    bar->setMinimum(0);
    bar->setMaximum(100);
    bar->setValue(0);
    connect(this, SIGNAL(setProgress(int)), bar, SLOT(setValue(int)));
    connect(this, SIGNAL(setProgress(int)), this, SLOT(slotProgress(int)));
    v->setContentsMargins(0, 0, 0, 0);
    setLayout(v);
    bar->setStyleSheet("QProgressBar {border: 2px solid grey;border-radius: 5px;text-align: center;}"
                       "QProgressBar::chunk {background-color: #05B8CC;width: 20px;}");
}

