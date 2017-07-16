#ifndef CALENDARLABEL_H
#define CALENDARLABEL_H

#include <QLabel>

class CalendarLabel : public QLabel
{
    Q_OBJECT
public:
    CalendarLabel(QWidget *parent = 0);
    ~CalendarLabel();

signals:
    void signalLeave();

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);

private:
    int timerId;

    // QWidget interface
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // CALENDARLABEL_H
