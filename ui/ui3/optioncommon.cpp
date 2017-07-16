#include "optioncommon.h"
#include <QtWidgets>
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"
#include "../notifyui.h"
#include "../../manager/widgetmanager.h"

OptionCommon::OptionCommon(QWidget *parent)
    : OptionWidget(parent)
{
    QGridLayout *grid = new QGridLayout(this);
    grid->setContentsMargins(3, 0, 8, 0);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    grid->setMargin(0);

    QCheckBox *box1 = new QCheckBox(QStringLiteral("开机自启动"), this);
    box1->setFixedSize(CalcStrByFont(box1->text()).width(), 30);
    QCheckBox *box2 = new QCheckBox(QStringLiteral("启用上线通知"), this);
    box2->setFixedSize(CalcStrByFont(box2->text()).width(), 30);
    QCheckBox *box3 = new QCheckBox(QStringLiteral("在好友列表中显示自己"), this);
    box3->setFixedSize(CalcStrByFont(box3->text()).width(), 30);
    QCheckBox *box4 = new QCheckBox(QStringLiteral("启用表情预览"), this);
    box4->setFixedSize(CalcStrByFont(box4->text()).width(), 30);
    QCheckBox *box5 = new QCheckBox(QStringLiteral("保存在线状态"), this);
    box5->setFixedSize(CalcStrByFont(box5->text()).width(), 30);
    QCheckBox *box6 = new QCheckBox(QStringLiteral("保存哈啤日志"), this);
    box6->setFixedSize(CalcStrByFont(box6->text()).width(), 30);
    QCheckBox *box7 = new QCheckBox(QStringLiteral("删除表情时同时删除表情资源文件"), this);
    box7->setFixedSize(CalcStrByFont(box7->text()).width(), 30);
    QCheckBox *box8 = new QCheckBox(QStringLiteral("静默更新"), this);
    box8->setFixedSize(CalcStrByFont(box8->text()).width(), 30);
    QCheckBox *box9 = new QCheckBox(QStringLiteral("启用水波纹"), this);
    box9->setFixedSize(CalcStrByFont(box9->text()).width(), 30);

    QWidget *w = new QWidget(this);
    QHBoxLayout *h1 = new QHBoxLayout(w);
    h1->setContentsMargins(3, 0, 0, 0);
    h1->setSpacing(0);
    QLabel *lab1 = new QLabel(QStringLiteral("导入表情时过滤大小:0.0Kb"));
    lab1->setFixedWidth(150);
    lab1->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:12px;color:white;"));
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setFixedWidth(200);
    h1->addWidget(lab1);
    h1->addWidget(slider);

    QHBoxLayout *h2 = new QHBoxLayout;
    h2->setContentsMargins(3, 0, 0, 0);
    h2->setSpacing(0);
    QPushButton *save = new QPushButton(cn("文件接收默认保存"), this);
    save->setFixedWidth(120);
    QLabel *savelal = new QLabel(cn(""), this);
    savelal->setStyleSheet(QStringLiteral("font-family:微软雅黑;font:10px;color:white;"));
    savelal->setFixedWidth(260);
    h2->addWidget(save);
    h2->addWidget(savelal);

    grid->addWidget(box1, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box2, 0, 1, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box3, 1, 0, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box4, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box5, 2, 0, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box6, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box8, 3, 0, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box9, 3, 1, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(box7, 4, 0, 1, 2, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(w, 5, 0, 1, 2, Qt::AlignLeft | Qt::AlignTop);
    grid->addLayout(h2, 6, 0, 2, 0, Qt::AlignLeft | Qt::AlignTop);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(1, 1);
    grid->setRowStretch(2, 1);
    grid->setRowStretch(3, 1);
    grid->setRowStretch(4, 1);
    grid->setRowStretch(5, 1);
    grid->setRowStretch(6, 1);
    grid->setRowStretch(7, 1);


    QFile file;
    file.setFileName(":res/css/checkbox.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        box1->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box2->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box3->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box4->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box5->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box6->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box7->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box8->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
        box9->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/slider.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        slider->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/button2.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        save->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    file.setFileName(":res/css/label.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        savelal->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba).arg(10));
    }
    file.close();

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    box1->setChecked(conf->IsTrue("auto_start"));
    box2->setChecked(conf->IsTrue("notify_online"));
    box3->setChecked(conf->IsTrue("show_self"));
    box4->setChecked(conf->IsTrue("open_preview"));
    box5->setChecked(conf->IsTrue("load_status"));
    box6->setChecked(conf->IsTrue("log"));
    box7->setChecked(conf->IsTrue("delete_image"));
    box8->setChecked(conf->IsTrue("update_silence"));
    box9->setChecked(conf->IsTrue("bezier"));
    float v = conf->getIni("filter_image").toFloat();
    slider->setValue(calaValue(v));
    lab1->setText(cns("导入表情时过滤大小:%1Kb").arg(QString::number(v, 'f', 1)));
    QString path = conf->getIni("download");
    if (path.isEmpty())
    {
        path = GetWorkPath() + "/dowmload";
    }
    QFontMetrics fm(cn("微软雅黑"));
    savelal->setText(fm.elidedText(path, Qt::ElideMiddle, 400));
    savelal->setToolTip(path);

    connect(save, &QPushButton::clicked, this, [=] () {
        QString path = QFileDialog::getExistingDirectory(this);
        if (!path.isEmpty())
        {
            QFontMetrics fm(cn("微软雅黑"));
            savelal->setText(fm.elidedText(path, Qt::ElideMiddle, 400));
            savelal->setToolTip(path);
            conf->setIni("download", path);
        }
    });

    connect(slider, &QSlider::valueChanged, this, [=] (int value) {
        int v = 0;
        if (value != 0)
            v = 1024 * 0.1 + (value - 1) * 1024 / 10.0;
        float kbs = v / 1024.0;
        lab1->setText(cns("导入表情时过滤大小:%1Kb").arg(QString::number(kbs, 'f', 1)));
        conf->setIni("filter_image", kbs);
    });

    connect(box1, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("auto_start", b);
        SetAutoStart(b);
    });
    connect(box2, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("notify_online", b);
        NotifyUI::ShowNotify(cn("暂未启用, 目前是不会提示"));
    });
    connect(box3, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("show_self", b);
        NotifyUI::ShowNotify(cn("重启后生效"));
    });
    connect(box4, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("open_preview", b);
    });
    connect(box5, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("load_status", b);
    });
    connect(box6, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("log", b);
        NotifyUI::ShowNotify(cn("重启后生效"));
    });
    connect(box7, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("delete_image", b);
    });
    connect(box8, &QCheckBox::toggled, this, [=] (bool b) {
        conf->setIni("update_silence", b);
    });
    connect(box9, &QCheckBox::toggled, this, [=] (bool b) {
        NotifyUI::ShowNotify(cn("实验性功能"));
        conf->setIni("bezier", b);
        WidgetManager::GetInstance()->setBezierEnable(b);
    });
}

OptionCommon::~OptionCommon()
{

}

int OptionCommon::calaValue(float v)
{
    return 10.0 * (v * 1024.0 - 1024 * 0.1) / 1024 + 1;
}

