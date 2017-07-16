#include "calendarui.h"
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include "../../calendar/lunar.h"
#include "../../config/configuredata.h"
#include "combox.h"
#include <QDateTime>

CalendarUI::CalendarUI(QWidget *parent)
    : THWidgetBase(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    hideTitleBar();

    timerId = -1;

    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setContentsMargins(0, 0, 0, 0);
    h1->setSpacing(1);
    QPushButton *left = new QPushButton(this);
    left->setFixedSize(15, 15);
    QPushButton *right = new QPushButton(this);
    right->setFixedSize(15, 15);
    yc = new Combox(this);
    yc->setEditable(false);
    mc = new Combox(this);
    mc->setEditable(false);
    dc = new Combox(this);
    dc->setEditable(false);
    h1->addWidget(left, 1);
    h1->addWidget(yc, 2);
    h1->addWidget(mc, 1);
    h1->addWidget(dc, 1);
    h1->addWidget(right, 1);

    lb1 = new QLabel(this);
    lb1->setAlignment(Qt::AlignCenter);
    lb2 = new QLabel(this);
    lb2->setAlignment(Qt::AlignCenter);
    lb3 = new QLabel(this);
    lb3->setAlignment(Qt::AlignCenter);
    lb3->setWordWrap(true);
    lb4 = new QLabel("00:00:00", this);
    lb4->setAlignment(Qt::AlignCenter);
    lb6 = new QLabel(this);
    lb6->setAlignment(Qt::AlignCenter);
    lb7 = new QLabel(this);
    lb7->setAlignment(Qt::AlignCenter);

    QHBoxLayout *h3 = new QHBoxLayout();
    h3->setContentsMargins(0, 0, 0, 0);
    h3->addWidget(lb4, 1, Qt::AlignRight);
    h3->addWidget(lb7, 1);

    QHBoxLayout *h2 = new QHBoxLayout();
    h2->setContentsMargins(0, 0, 0, 0);
    h2->setSpacing(2);
    QPushButton *ico = new QPushButton(this);
    ico->setFixedSize(20, 20);
    connect(ico, &QPushButton::clicked, this, [=] () {
        QDate date = QDate::currentDate();
        yc->setCurrentText(QString::number(date.year()));
        mc->setCurrentText(QString::number(date.month()));
        dc->setCurrentText(QString::number(date.day()));
    });
    QLabel *lb5 = new QLabel(this);
    lb5->setOpenExternalLinks(true);
    h2->addWidget(ico, 1);
    h2->addWidget(lb5, 9);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(6, 8, 6, 8);
    v->addLayout(h1, 1);
    v->addSpacing(2);
    v->addWidget(lb1, 1);
    v->addWidget(lb2, 1);
    v->addWidget(lb3, 2);
    v->addLayout(h3, 1);
    v->addWidget(lb6, 1);
    v->addSpacing(2);
    v->addLayout(h2, 1);

    setFixedSize(260, 210);

    // add style
    QFile file;
    file.setFileName(":res/css/combox.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString css = QTextCodec::codecForLocale()->toUnicode(file.readAll());
        yc->setStyleSheet(css.arg(15));
        mc->setStyleSheet(css.arg(3));
        dc->setStyleSheet(css.arg(3));
    }
    file.close();
    file.setFileName(":res/css/button.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString css = QTextCodec::codecForLocale()->toUnicode(file.readAll());
        left->setStyleSheet(css.arg("left").arg("left").arg("left"));
        right->setStyleSheet(css.arg("right").arg("right").arg("right"));
        ico->setStyleSheet(css.arg("mark").arg("mark").arg("mark"));
    }
    file.close();
//    // add data
    for (int i = 0; i < 150; i++)
    {
        yc->addItem1(QString::number(1900 + i));
    }
    yc->setTextAlignment(Qt::AlignCenter);
    for (int j = 1; j <= 12; j++)
    {
        mc->addItem1(QString::number(j));
        dc->addItem1(QString::number(j));
    }
    mc->setTextAlignment(Qt::AlignCenter);
    lb5->setText(QStringLiteral("万年历算法源自互联网 ") +
                 QStringLiteral("<a style='color: white;text-decoration:none;' href = www.cnblogs.com/qintangtao/archive/2013/03/01/2938887.html>来源</a>"));
    QString lbcss = QStringLiteral("QLabel{font-family:微软雅黑;font:%1 %2px;color:white;}");

    lb5->setStyleSheet(lbcss.arg("normal").arg(10));

    lb1->setStyleSheet(lbcss.arg("bold").arg(14));
    lb2->setStyleSheet(lbcss.arg("normal").arg(12));
    lb3->setStyleSheet(lbcss.arg("bold").arg(14));
    lb4->setStyleSheet(lbcss.arg("bold").arg(14));
    lb6->setStyleSheet(lbcss.arg("normal").arg(12));
    lb7->setStyleSheet(lbcss.arg("bold").arg(14));


    connect(yc, &QComboBox::currentTextChanged,
            this, &CalendarUI::dayChanged);
    connect(mc, &QComboBox::currentTextChanged,
            this, &CalendarUI::dayChanged);
    connect(dc, &QComboBox::currentTextChanged,
            this, [=] () {
        QDate date(yc->currentText().toInt(), mc->currentText().toInt(), dc->currentText().toInt());
        if (!date.isValid())
            return;
        QtConcurrent::run(this, &CalendarUI::calcLunar, date.year(), date.month(), date.day());
    });
    connect(left, &QPushButton::clicked, [=] () {
        QDate date(yc->currentText().toInt(), mc->currentText().toInt(), dc->currentText().toInt());
        date = date.addDays(-1);
        yc->setCurrentText(QString::number(date.year()));
        mc->setCurrentText(QString::number(date.month()));
        dc->setCurrentText(QString::number(date.day()));
    });
    connect(right, &QPushButton::clicked, [=] () {
        QDate date(yc->currentText().toInt(), mc->currentText().toInt(), dc->currentText().toInt());
        date = date.addDays(1);
        yc->setCurrentText(QString::number(date.year()));
        mc->setCurrentText(QString::number(date.month()));
        dc->setCurrentText(QString::number(date.day()));
    });
    connect(this, SIGNAL(signalUpdateLunar(QString,QString,QString,QString,QString,QString,QString,QString,QString)),
            this, SLOT(slotUpdateLunar(QString,QString,QString,QString,QString,QString,QString,QString,QString)));
    QTimer::singleShot(10, this, [=] () {
       emit ico->clicked();
       timerId = startTimer(1000);
    });
//    QTimer::singleShot(400, this, [=] () {
//        closeUI();
//    });

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    updateColor(conf->getColorIni("color1"), conf->getColorIni("color2"));
}

CalendarUI::~CalendarUI()
{

}

void CalendarUI::dayChanged(QString /*txt*/)
{
    QString str = dc->currentText();
    dc->clear1();
    QDate date(yc->currentText().toInt(), mc->currentText().toInt(), 1);
    for (int i = 1; i <= date.daysInMonth(); i++)
    {
        dc->addItem1(QString::number(i).trimmed());
    }
    dc->setTextAlignment(Qt::AlignCenter);
    dc->setCurrentText(str);
    QTimer::singleShot(10, this, [=] () {
        emit dc->currentTextChanged(str);
        dc->corrected();
    });
}

void CalendarUI::calcLunar(int y, int m, int d)
{
    if (m <=0 || d <= 0)
    {
        return;
    }
    QDateTime dt(QDate(y, m, d), QTime::currentTime());
    ChineseCalendar cc(dt);
    cc.calc();
    QString c24day = cc.getChineseTwentyFourDay();
    if (c24day.isEmpty())
    {
        c24day = cc.getChineseTwentyFourPrevDay() + "  " +
                cc.getChineseTwentyFourNextDay();
    }
    QString nlyear = cn("农历") + cc.getChineseYearString() +
            " " + cc.getAnimalString() + cn("年");
    QString nlday = cc.getChineseMonthString() + cc.getChineseDayString();
    emit signalUpdateLunar(nlyear,
                     nlday,
                     cc.getWeekDayStr(),
                     cc.getGanZhiDateString(),
                     cc.getChineseHour(),
                     c24day,
                     cc.getDateHoliday(),
                     cc.getChineseCalendarHoliday(),
                     cc.getWeekDayHoliday());
}

void CalendarUI::slotUpdateLunar(QString lunar, QString year, QString week,
                                 QString day, QString time, QString lunarday,
                                 QString holiday1, QString holiday2, QString holiday3)
{
    lb1->setText(lunar);
    lb2->setText(day);
    lb7->setText(time + cn("时"));
    lb6->setText(lunarday);
    QString str = "";
    if (!holiday1.isEmpty())
    {
        str = holiday1;
    }
    if (!holiday2.isEmpty())
    {
        str = str + " " + holiday2;
    }
    if (!holiday2.isEmpty())
    {
        str = str + " " + holiday3;
    }
    lb3->setText(" " + year + " " + week + " " + str);
}

void CalendarUI::closeUI()
{
    QPoint p1 = QCursor::pos();
    QPoint p2 = mapFromGlobal(p1);
    QRect rct = rect();
    if (rct.contains(p2))
    {
        return;
    }
    this->close();
    this->deleteLater();
}

void CalendarUI::timerEvent(QTimerEvent *e)
{
    if (e->timerId() != timerId)
    {
        return;
    }
    lb4->setText(QTime::currentTime().toString("hh:mm:ss"));
}



void CalendarUI::leaveEvent(QEvent *e)
{
    QTimer::singleShot(400, this, [=] () {
        closeUI();
    });
    return THWidgetBase::leaveEvent(e);
}
