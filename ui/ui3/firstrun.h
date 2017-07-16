#ifndef FIRSTRUN_H
#define FIRSTRUN_H

#include "../thwidgetbase.h"
#include <QEventLoop>

class FirstRun : public THWidgetBase
{
    Q_OBJECT
public:
    explicit FirstRun(QWidget *parent = 0);
    ~FirstRun();
    int exec();

private:
    int id;
    QEventLoop loop;

    void writeWorkDirectiry(QString str = QString());
};

#endif // FIRSTRUN_H
