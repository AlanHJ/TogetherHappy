#ifndef PROGRESSUI_H
#define PROGRESSUI_H

#include "../thwidgetbase.h"

class QLabel;
class ProgressUI : public THWidgetBase
{
    Q_OBJECT
public:
    ProgressUI(QWidget *parent = 0);
    ~ProgressUI();
    void setText(QString txt);
    static ProgressUI *showProgerss(QString txt, QWidget *p = 0);
signals:
    void setValue(int value);
    void execFinished();
private:
    QLabel *lbl;

    int timerId;
    float gradient;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);
};

#endif // PROGRESSUI_H
