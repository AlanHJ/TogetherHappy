#ifndef LOCKWIDGET_H
#define LOCKWIDGET_H

#include <QWidget>

class QLabel;
class LockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LockWidget(QWidget *parent = 0);
    ~LockWidget();

private:
    QLabel *datetimeLbl;
    int id;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);
};

#endif // LOCKWIDGET_H
