#include "firstrun.h"
#include "../happytitlebar.h"
#include "progressbar.h"
#include "msgbox.h"
#include <QtWidgets>

FirstRun::FirstRun(QWidget *parent) : THWidgetBase(parent)
{
    id = -1;

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(QStringLiteral("���ù���Ŀ¼"));

    setFixedSize(300, 200);
    setTitleBarWidth(300);

    QLabel *contentLbl = new QLabel(this);
    contentLbl->setWordWrap(true);
    contentLbl->setStyleSheet(QStringLiteral("font-family:΢���ź�;font:12px;color:white;"));
    contentLbl->setText(QStringLiteral("��ơ��Ҫһ������Ŀ¼�����ڴ���������ݺ���Դ�ļ�������300����ѡ��Ŀ¼����ʱ���ơ���Զ�ѡ����Ŀ¼"));

    QLabel *selectLbl = new QLabel(this);
    selectLbl->setAlignment(Qt::AlignCenter);
    selectLbl->setWordWrap(true);
    selectLbl->setFixedWidth(260);
    selectLbl->setStyleSheet(QStringLiteral("font-family:΢���ź�;font:10px;color:white;"));


    QLabel *tipsLbl = new QLabel(this);
    tipsLbl->setAlignment(Qt::AlignCenter);
    tipsLbl->setStyleSheet(QStringLiteral("font-family:΢���ź�;font:10px;color:white;"));
    tipsLbl->setFixedWidth(260);
    tipsLbl->setText(QStringLiteral("ֱ�ӵ��ȷ������ơ���Զ�ѡ����Ŀ¼"));

    ProgressBar *pbar = new ProgressBar(this);
    pbar->setTextVisible(false);
    pbar->setRange(0, 300);
    pbar->setValue(300);
    pbar->setFixedSize(260, 2);
    pbar->setStyleSheet("QProgressBar{background:transparent;border:0px solid transparent;border-radius:1px;}"
                       "QProgressBar::chunk{background-color:rgba(230,230,230,200);width: 1px;}");

    QHBoxLayout *h = new QHBoxLayout();
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);
    QPushButton *selectPb = new QPushButton(QStringLiteral("ѡ��"), this);
    selectPb->setFixedSize(60, 25);
    QPushButton *pb = new QPushButton(QStringLiteral("ȷ��"), this);
    pb->setFixedSize(60, 25);
    h->addWidget(selectPb);
    h->addWidget(pb);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(10, 40, 10, 15);
    v->setSpacing(0);

    v->addWidget(contentLbl, 0, Qt::AlignCenter);
    v->addStretch(1);
    v->addWidget(selectLbl, 0, Qt::AlignCenter);
    v->addSpacing(5);
    v->addLayout(h);
    v->addSpacing(10);
    v->addWidget(tipsLbl, 0, Qt::AlignCenter);
    v->addWidget(pbar, 0, Qt::AlignCenter);

    QFile file;
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        selectPb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        pb->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();



    connect(selectPb, &QPushButton::clicked, this, [=] () {
        QString directiry = QFileDialog::getExistingDirectory(this,
                                                              QStringLiteral("ѡ����Ŀ¼"),
                                                              "C:");
        selectLbl->setText(directiry + "/HappyWork");
    });


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=] () {
        if (pbar->value() <= 0)
        {
            timer->stop();
            pb->setEnabled(false);
            tipsLbl->setText(QStringLiteral("��ʼ׼��Ŀ¼"));
            writeWorkDirectiry(selectLbl->text().trimmed());
        }
        else
        {
            pbar->setValue(pbar->value() - 1);
        }
    });
    timer->start(1000);

    connect(pb, &QPushButton::clicked, this, [=] () {
        QString dir = selectLbl->text().trimmed();
        if (dir.isEmpty())
        {
            MsgBox *box = new MsgBox();
            box->setTitle(QStringLiteral("��ơ��ʾ"));
            box->setContent(QStringLiteral("û��ѡ����Ŀ¼��ȷ���ù�ơѡ��ô?"));
            int i = box->exec();
            if (i != 0)
            {
                return;
            }
        }
        timer->stop();
        // write ini to user/appdata/
        tipsLbl->setText(QStringLiteral("��ʼ׼��Ŀ¼"));
        writeWorkDirectiry(selectLbl->text().trimmed());
    });

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        timer->stop();
        loop.exit();
        this->close();
        this->deleteLater();
    });
}

FirstRun::~FirstRun()
{

}

int FirstRun::exec()
{
    show();
    activateWindow();
    loop.exec(QEventLoop::DialogExec);
    return id;
}

void FirstRun::writeWorkDirectiry(QString str)
{
    QString temp = str;
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(dataPath);
    dir.cdUp();
    dataPath = dir.absolutePath() + "/TogetherHappy";
    if (temp.isEmpty())
    {
        temp = dataPath + "/HappyWork";
    }
    if (temp.contains("://"))
    {
        temp = temp.replace("://", ":/");
    }
    // mk other paths
    bool b = QDir().mkpath(temp);
    b &= QDir().mkpath(temp + "/custom");
    b &= QDir().mkpath(temp + "/database");
    b &= QDir().mkpath(temp + "/face");
    b &= QDir().mkpath(temp + "/plug");
    b &= QDir().mkpath(temp + "/backup");
    b &= QDir().mkpath(temp + "/update");
    b &= QDir().mkpath(temp + "/temp");
    b &= QDir().mkpath(temp + "/log");
    if (!b)
    {
        MsgBox *box = new MsgBox();
        box->setTitle(QStringLiteral("��ơ��ʾ"));
        box->setContent(QStringLiteral("����,���ܴ�������Ŀ¼"));
        box->setButtons(QStringLiteral("�˳�"));
        int i = box->exec();
        if (i == 0)
        {
            loop.exit();
            this->close();
            this->deleteLater();
            return;
        }
    }

    QSettings set(dataPath + "/config.ini", QSettings::IniFormat);
    set.setIniCodec("UTF-8");
    set.setValue("run/work", temp);
    id = 0;
    loop.exit();
    this->close();
    this->deleteLater();
}

