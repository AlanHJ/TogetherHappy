#include "combox.h"
#include <QtWidgets>
#include "../../config/configuredata.h"

Combox::Combox(QWidget *parent) : QComboBox(parent)
{
    pupw = new PupupWidget();
    pupw->hide();
    connect(pupw->lw, &QListWidget::itemClicked, this, [=] () {
        setCurrentText(pupw->lw->currentItem()->text());
        pupw->hide();
        QTimer::singleShot(1, this, [=] () {
            pupw->visible = false;
        });
    });
    connect(this, SIGNAL(currentTextChanged(QString)),
            this, SLOT(slotCurrentTextChanged(QString)));
}

Combox::~Combox()
{
    if (pupw)
    {
        delete pupw;
        pupw = nullptr;
    }
}

void Combox::corrected()
{
    slotCurrentTextChanged(currentText());
}

void Combox::clear1()
{
    this->clear();
    pupw->clear();
}

void Combox::addItem1(QString txt)
{
    this->addItem(txt);
    pupw->lw->addItem(txt);
}

void Combox::addItems1(QStringList txts)
{
    this->addItems(txts);
    pupw->lw->addItems(txts);
}

void Combox::setTextAlignment(int alignment)
{
    for (int i = 0; i < pupw->lw->count(); i++)
    {
        QListWidgetItem *item = pupw->lw->item(i);
        item->setTextAlignment(alignment);
    }
}

void Combox::slotCurrentTextChanged(const QString &txt)
{
    for (int i = 0; i < pupw->lw->count(); i++)
    {
        QListWidgetItem *item = pupw->lw->item(i);
        if (item->text() == txt)
        {
            pupw->lw->setCurrentItem(item);
            break;
        }
    }
}



void Combox::showPopup()
{
    if (pupw->visible)
    {
        pupw->hide();
        QTimer::singleShot(1, this, [=] () {
            pupw->visible = false;
        });
        return;
    }
    pupw->resize(this->width(), 100);
    pupw->show();
    QTimer::singleShot(1, this, [=] () {
        pupw->visible = true;
    });
    pupw->activateWindow();
    QPoint p = mapToGlobal(this->rect().topLeft());
    pupw->move(p.x(), p.y() + this->height() + 1);
}

void Combox::hidePopup()
{
    pupw->hide();
}


PupupWidget::PupupWidget(QWidget *p)
    : QWidget(p)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);
    lw = new QListWidget(this);
    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(0, 0, 0, 0);
    v->addWidget(lw);
    visible = false;
    QFile file(":res/css/list.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString css = QTextCodec::codecForLocale()->toUnicode(file.readAll());
        lw->setStyleSheet(css);
    }
    file.close();
    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    c1 = conf->getColorIni("color1");
    c2 = conf->getColorIni("color2");
}

PupupWidget::~PupupWidget()
{

}

void PupupWidget::clear()
{
    lw->clear();
}

void PupupWidget::addItem(QString txt)
{
    QListWidgetItem *item = new QListWidgetItem(lw);
    item->setText(txt);
    item->setTextAlignment(Qt::AlignCenter);
}

void PupupWidget::addItems(QStringList txts)
{
    lw->addItems(txts);
}

bool PupupWidget::isVisible1()
{
    return visible;
}

void PupupWidget::setVisible1(bool b)
{
    visible = b;
}

void PupupWidget::hidePopup()
{
    hide();
    QTimer::singleShot(1, this, [=] () {
        setVisible1(false);
    });
}

void PupupWidget::showPopup()
{
    show();
    QTimer::singleShot(1, this, [=] () {
        setVisible1(true);
    });
}

QListWidget *PupupWidget::list()
{
    return lw;
}


void PupupWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
//    painte
    QPainterPath path;
    float h = this->height();
    float w = this->width();
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(QRect(0, 0, w, h), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // LinearGradient
    QLinearGradient liner(QPoint(0, 0), QPoint(w, h));
    // 132, 216, 209 À¶
    // 47, 107, 117 Ä¬ÈÏ
    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    c1 = conf->getColorIni("color1");
    c2 = conf->getColorIni("color2");
    liner.setColorAt(0, c1);
    liner.setColorAt(1, c2);
    painter.fillPath(path, QBrush(liner));
    painter.setPen(Qt::transparent);
    painter.drawPath(path);
    // draw some lines
    QList<QPoint> line1;
    QList<QPoint> line2;
    {
        // calc start, final point
        float k = h / w;
        int b[7];
        int x1[7], x2[7];
        b[0] = h /3;
        b[1] = h /2;
        b[2] = 2 * h / 3;
        b[3] = 4 * h / 3;
        b[4] = 3 * h / 2;
        b[5] = 5 * h / 3;
        b[6] = h;
        x1[0] = w / 6;
        x2[0] = w / 4;
        x1[1] = w / 4;
        x2[1] = 2 * w / 5;
        x1[2] = w / 4;
        x2[2] = 4 * w / 7;
        x1[3] = w / 2;
        x2[3] = 5 * w / 6;
        x1[4] = 2 * w / 3;
        x2[4] = 5 * w / 6;
        x1[5] = 3 * w / 4;
        x2[5] = 4 * w / 5;
        x1[6] = w / 4;
        x2[6] = 3 * w / 4;

        for (int i = 0; i < 7; i++)
        {
            line1 << QPoint(x1[i], - k * x1[i] + b[i]);
            line2 << QPoint(x2[i], - k * x2[i] + b[i]);
        }
    }
    for (int i = 0; i < 7; i++)
    {
        QPoint p1 = line1.at(i);
        QPoint p2 = line2.at(i);
        QLinearGradient linecolor(p1, p2);
        linecolor.setColorAt(0, QColor(220, 220, 220, 50));
        linecolor.setColorAt(0.45, QColor(220, 220, 220, 150));
        linecolor.setColorAt(0.55, QColor(220, 220, 220, 150));
        linecolor.setColorAt(1, QColor(220, 220, 220, 50));
        painter.setPen(QPen(QBrush(linecolor), 1));
        painter.drawLine(p1, p2);
    }
    return QWidget::paintEvent(e);
}


void PupupWidget::hideEvent(QHideEvent *)
{
    QTimer::singleShot(200, this, [=] () {
        visible = false;
    });
}
