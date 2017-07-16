#include "progressui.h"
#include <QtWidgets>
#include "../../util/thmethod.h"
#include "../../config/configuredata.h"

ProgressUI::ProgressUI(QWidget *parent)
    : THWidgetBase(parent)
{
    timerId = -1;
    gradient = 0;
    hideTitleBar();
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(5, 5, 5, 5);
    v->setSpacing(0);
    lbl = new QLabel(this);
    lbl->setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;font:12px;color:white;"));
    QProgressBar *pbar = new QProgressBar(this);
    pbar->setRange(0, 100);
    pbar->setValue(0);
    pbar->setFixedSize(260, 12);
    QFile file;
    file.setFileName(":res/css/progressbar.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        pbar->setStyleSheet(QTextCodec::codecForLocale()->toUnicode(ba));
    }
    file.close();
    v->addWidget(lbl, 1, Qt::AlignCenter);
    v->addWidget(pbar, 1, Qt::AlignCenter);
    setFixedSize(280, 50);

    connect(this, &ProgressUI::setValue, pbar, [=] (int value) {
        pbar->setValue(value);
        if (pbar->value() >= 100)
        {
            lbl->setText(QStringLiteral("Íê³É"));
            emit execFinished();
            QTimer::singleShot(1000, this, [=] () {
                disconnect(this, 0, 0, 0);
                this->close();
                this->deleteLater();
            });
        }
    });

    timerId = startTimer(60/*, Qt::VeryCoarseTimer*/);


}

ProgressUI::~ProgressUI()
{

}

void ProgressUI::setText(QString txt)
{
    lbl->setText(txt);
}

ProgressUI *ProgressUI::showProgerss(QString txt, QWidget *p)
{
    ProgressUI *ui = new ProgressUI();
    ui->setText(txt);
    ui->show();
    if (p)
        MovePcenter(p, ui);
    else
        movecenter(ui);
    ui->activateWindow();
    return ui;
}

void ProgressUI::paintEvent(QPaintEvent *e)
{
    THWidgetBase::paintEvent(e);
    QPainter p(this);
    QPainterPath path;
    int w = this->width();
    int h = this->height();
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
    path.addRoundedRect(QRect(5, 5, w - 10, h - 10), 3, 3);
    QColor color(Qt::transparent);
    QPoint p1(5, 5);
    QPoint p2(w - 10, h - 10);
    QLinearGradient linecolor(p1, p2);
    linecolor.setColorAt(0, color);
    linecolor.setColorAt(1, color);
    linecolor.setColorAt(gradient - 0.15 <= 0 ? 0 : gradient - 0.15, color);
    linecolor.setColorAt(gradient >= 1 ? 1 : gradient, QColor(230, 230, 230, 50));
    linecolor.setColorAt(gradient + 0.15 >= 1 ? 1 : gradient + 0.15, color);
    p.setPen(Qt::transparent);
    p.fillPath(path, QBrush(linecolor));
}

void ProgressUI::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerId)
    {
        if (gradient >= 1)
        {
            gradient = 0;
        }
        update();
        gradient = gradient + 0.05;
    }
}

