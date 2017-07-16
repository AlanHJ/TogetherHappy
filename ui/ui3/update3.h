#ifndef UPDATE3_H
#define UPDATE3_H

#include "../thwidgetbase.h"
#include <QEventLoop>

/**
 * @brief The Update3 class
 * update 2.xx to 3.0 UI
 */
class Update3 : public THWidgetBase
{
    Q_OBJECT
public:
    explicit Update3(QWidget *parent = 0);
    ~Update3();
    int exec();

signals:
    void updateSuccess();
    void updateProgress(int value);
    void updateFail(QString error);
    void updateInformation(QString info);

private slots:
    void startUpdate();

private:
    QEventLoop loop;
    int id;

    int timerId;
    float gradient;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);
};

#endif // UPDATE3_H
