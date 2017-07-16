#include "calendarlabel.h"
#include <QTimer>
#include <QDateTime>
#include <QTimerEvent>
#include <QFontMetrics>
#include <QDebug>
#include "calendarui.h"

CalendarLabel::CalendarLabel(QWidget *parent)
    : QLabel(parent)
{
    timerId = -1;
    QTimer::singleShot(200, this, [=] () {
        timerId = startTimer(1000);
    });
    setStyleSheet(QStringLiteral("font-family:Î¢ÈíÑÅºÚ;color:white;font:12px;background:transparent;"));
    adjustSize();
    setWordWrap(true);
    QFont font(QStringLiteral("Î¢ÈíÑÅºÚ"));
    font.setPointSize(10);
    QFontMetrics metrics(font);
    QSize size = metrics.size(Qt::TextExpandTabs , "hh:mm:ss");
    setFixedWidth(size.width());
}

CalendarLabel::~CalendarLabel()
{

}

void CalendarLabel::timerEvent(QTimerEvent *e)
{
    if (e->timerId() != timerId)
        return;
    setText(QTime::currentTime().toString("hh:mm:ss"));
}
#include <QApplication>
void CalendarLabel::enterEvent(QEvent *)
{
    QTimer::singleShot(500, this, [=] () {
        QPoint p = mapFromGlobal(QCursor::pos());
        if (!rect().contains(p) || qobject_cast<CalendarUI*>(qApp->activeWindow()))
        {
            return;
        }
        CalendarUI *ui = new CalendarUI();
        ui->show();
        ui->activateWindow();
        ui->move(this->parentWidget()->pos().x() - ui->width(),
                 QCursor::pos().y() - 100);
        connect(this, &CalendarLabel::signalLeave, ui, [=] () {
            disconnect(this, &CalendarLabel::signalLeave, 0, 0);
            QTimer::singleShot(400, this, [=] () {
                ui->closeUI();
            });
        });
    });
}

void CalendarLabel::leaveEvent(QEvent *)
{
    // Òþ²ØÅ©Àú½çÃæ
    emit signalLeave();
}

