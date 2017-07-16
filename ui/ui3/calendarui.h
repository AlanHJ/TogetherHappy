#ifndef CALENDARUI_H
#define CALENDARUI_H

#include "../thwidgetbase.h"

class QLabel;
class QComboBox;
class Combox;
class CalendarUI : public THWidgetBase
{
    Q_OBJECT
public:
    CalendarUI(QWidget *parent = 0);
    ~CalendarUI();
    void closeUI();

private slots:
    void dayChanged(QString txt);
    void calcLunar(int y, int m, int d);
    void slotUpdateLunar(QString lunar, QString year,
                     QString week, QString day,
                     QString time, QString lunarday,
                     QString holiday1, QString holiday2, QString holiday3);

signals:
    void signalUpdateLunar(QString lunar, QString year,
                     QString week, QString day,
                     QString time, QString lunarday,
                     QString holiday1, QString holiday2, QString holiday3);

private:
    int timerId;
    QLabel *lb1;
    QLabel *lb2;
    QLabel *lb3;
    QLabel *lb4;
    QLabel *lb6;
    QLabel *lb7;

    Combox *yc;
    Combox *mc;
    Combox *dc;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);

    // QWidget interface
protected:
    void leaveEvent(QEvent *e);
};

#endif // CALENDARUI_H
