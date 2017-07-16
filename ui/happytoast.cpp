#include "happytoast.h"
#include <QLabel>
#include <QVBoxLayout>

HappyToast::HappyToast(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::Tool |
                   Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
}

HappyToast::~HappyToast()
{

}

void HappyToast::init()
{
    // 创建界面
    label = new QLabel();
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(label);
    setLayout(v);
}

