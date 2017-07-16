#include "configui.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include "mainui.h"
#include "notifyui.h"
#include <QFile>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>

ConfigUI::ConfigUI(QWidget *p) : QWidget(p)
{
    init();
    setWindowTitle(cns("����"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
}

ConfigUI::~ConfigUI()
{

}

void ConfigUI::init()
{
    QLabel *name = new QLabel(cns("�ǳ�:"), this);
    mNameEdit = new QLineEdit(this);
    QPushButton *apply = new QPushButton(cns("ȷ��"), this);
    connect(apply, SIGNAL(clicked()), this, SLOT(apply()));
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->addWidget(name, 1);
    h1->addWidget(mNameEdit, 0, Qt::AlignCenter);
    h1->addWidget(apply, 1);
//    name->setBuddy(mNameEdit);

    // �ֶ�IP����
    QLabel *ipl = new QLabel(cns("IP:"), this);
    mIpEdit = new QLineEdit(this);
    QPushButton *enable = new QPushButton(cns("����"), this);
    enable->setCheckable(true);
    enable->setChecked(false);
    connect(enable, SIGNAL(clicked()), this, SLOT(eanbleIP()));
    QHBoxLayout *hb1 = new QHBoxLayout();
    hb1->addWidget(ipl, 1);
    hb1->addWidget(mIpEdit, 0, Qt::AlignCenter);
    hb1->addWidget(enable, 1);

    QVBoxLayout *v1 = new QVBoxLayout();
    v1->addLayout(h1, 1);
//    v1->addWidget(apply, 1);
    v1->addLayout(hb1);

    // ������ɫ����
    int r = 47;
    int g = 106;
    int b = 147;
    int a = 255;
//    ConfigureData::getInstance()->getIni("r", r);
//    ConfigureData::getInstance()->getIni("g", g);
//    ConfigureData::getInstance()->getIni("b", b);
//    ConfigureData::getInstance()->getIni("a", a);

    QHBoxLayout *rhl = new QHBoxLayout();
    QLabel *rlabel = new QLabel(cns("��:"), this);
    mRslider = new QSlider(Qt::Horizontal, this);
    mRslider->setObjectName("r");
    mRslider->setMaximum(255);
    mRslider->setMinimum(0);
    mRslider->setValue(r);
    connect(mRslider, SIGNAL(valueChanged(int)), this, SLOT(RGBValue(int)));
    rhl->addWidget(rlabel);
    rhl->addWidget(mRslider);

    QHBoxLayout *ghl = new QHBoxLayout();
    QLabel *glabel = new QLabel(cns("��:"), this);
    mGslider = new QSlider(Qt::Horizontal, this);
    mGslider->setObjectName("g");
    mGslider->setMaximum(255);
    mGslider->setMinimum(0);
    mGslider->setValue(g);
    connect(mGslider, SIGNAL(valueChanged(int)), this, SLOT(RGBValue(int)));
    ghl->addWidget(glabel);
    ghl->addWidget(mGslider);

    QHBoxLayout *bhl = new QHBoxLayout();
    QLabel *blabel = new QLabel(cns("��:"), this);
    mBslider = new QSlider(Qt::Horizontal, this);
    mBslider->setObjectName("b");
    mBslider->setMaximum(255);
    mBslider->setMinimum(0);
    mBslider->setValue(b);
    connect(mBslider, SIGNAL(valueChanged(int)), this, SLOT(RGBValue(int)));
    bhl->addWidget(blabel);
    bhl->addWidget(mBslider);

    QHBoxLayout *ahl = new QHBoxLayout();
    QLabel *alabel = new QLabel(cns("��:"), this);
    mAslider = new QSlider(Qt::Horizontal, this);
    mAslider->setObjectName("a");
    mAslider->setMaximum(255);
    mAslider->setMinimum(0);
    mAslider->setValue(a);
    connect(mAslider, SIGNAL(valueChanged(int)), this, SLOT(RGBValue(int)));
    ahl->addWidget(alabel);
    ahl->addWidget(mAslider);

    v1->addLayout(rhl);
    v1->addLayout(ghl);
    v1->addLayout(bhl);
    v1->addLayout(ahl);

    QLabel *autostartl = new QLabel(cns("����������:"), this);
    QPushButton *autostartp = new QPushButton(cns("����"), this);
    connect(autostartp, SIGNAL(clicked()), this, SLOT(autostart()));
    autostartp->setCheckable(true);
    autostartp->setChecked(false);
    QHBoxLayout *hb2 = new QHBoxLayout();
    hb2->addWidget(autostartl);
    hb2->addWidget(autostartp);
    v1->addLayout(hb2);

    setLayout(v1);

//    int ipset;
    QString ip;
//    ConfigureData::getInstance()->getIni("ipset", ipset);
//    ConfigureData::getInstance()->getIni("ip", ip);
    mIpEdit->setText(ip);
//    if (ipset == 1)
//    {
//        enable->setChecked(true);
//        enable->setText(cns("ͣ��"));
//    }
//    else
//    {
//        enable->setChecked(false);
//        enable->setText(cns("����"));
//    }

//    int autoset;
////    ConfigureData::getInstance()->getIni("auto", autoset);
//    if (autoset == 1)
//    {
//        autostartp->setChecked(true);
//        autostartp->setText(cns("�ر�"));
//    }
//    else
//    {
//        autostartp->setChecked(false);
//        autostartp->setText(cns("����"));
//    }
}

void ConfigUI::apply()
{
    QString name = mNameEdit->text();
    if (name.length() > 20)
    {
        NotifyUI::ShowNotify(cns("�ǳƲ��ܹ���"));
//        QMessageBox::information(this, cns("��ʾ"), cns("�ǳƲ��ܹ���"), cns("��"));
        return;
    }
    if (name.isEmpty() || name.isNull())
    {
        NotifyUI::ShowNotify(cns("�ǳƲ���Ϊ��"));
//        QMessageBox::information(this, cns("��ʾ"), cns("�ǳƲ���Ϊ��"), cns("��"));
        return;
    }
    emit modifName(name);
    close();
}

void ConfigUI::RGBValue(int /*value*/)
{
//    qApp->setStyleSheet("");
    int r = mRslider->value();
    mRslider->setToolTip(QString::number(r));
    int g = mGslider->value();
    mGslider->setToolTip(QString::number(g));
    int b = mBslider->value();
    mBslider->setToolTip(QString::number(b));
    int a = mAslider->value();
    mAslider->setToolTip(QString::number(a));
    QString stylecss("");
    QFile file(QTextCodec::codecForLocale()->toUnicode(":res/css/CssStyle.css"));
    if(true == file.open(QFile::ReadOnly))
    {
        stylecss = QTextCodec::codecForLocale()->toUnicode(file.readAll());
    }
//    QString qss = "QWidget{background:rgba(%1,%2,%3,%4);color:rgb(0,0,0);}";
    stylecss = stylecss.arg(r).arg(g).arg(b).arg(a);
//    this->parentWidget()->setStyleSheet(stylecss);
//    qApp->setStyleSheet(stylecss);
//    ConfigureData::getInstance()->writeIni("r", r);
//    ConfigureData::getInstance()->writeIni("g", g);
//    ConfigureData::getInstance()->writeIni("b", b);
//    ConfigureData::getInstance()->writeIni("a", a);
}

void ConfigUI::eanbleIP()
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    if (nullptr == pb)
    {
        return;
    }
    if (pb->isChecked())
    {
        pb->setText(cns("ͣ��"));
        QString ip = mIpEdit->text().trimmed();
        if (ip.isEmpty())
        {
            QMessageBox::information(this, cns("��ʾ"), cns("IP��ҪΪ��"), cns("��"));
            pb->setChecked(false);
            eanbleIP();
        }
        else
        {
//            ConfigureData::getInstance()->writeIni("ipset", 1);
//            ConfigureData::getInstance()->writeIni("ip", ip);
        }
    }
    else
    {
        pb->setText(cns("����"));
//        ConfigureData::getInstance()->writeIni("ipset", 0);
    }
    QMessageBox::information(this, cns("��ʾ"), cns("���ֶ�������ơ�������Ӧ��"), cns("��"));
}

void ConfigUI::autostart()
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    if (nullptr == pb)
    {
        return;
    }
    if (pb->isChecked())
    {
//        ConfigureData::getInstance()->writeIni("auto", 1);
        pb->setText(cns("�ر�"));
    }
    else
    {
//        ConfigureData::getInstance()->writeIni("auto", 0);
        pb->setText(cns("����"));
    }
//    ConfigureData::getInstance()->setAutoStart(pb->isChecked());
}

