#ifndef NOTIFYUI_H
#define NOTIFYUI_H

#include <QLabel>
#include <QTime>
#include "thwidgetbase.h"
#include <QPropertyAnimation>
#include "../manager/notifymanager.h"

#define NotifyNoused
#define DefalutTime 2000
#define DefalutFontSize 20
class NotifyUI : public QWidget
{
    Q_OBJECT
    friend class NotifyManager;
    Q_PROPERTY(int aniHeight READ getHeight WRITE setHeight)
    Q_PROPERTY(int aniSize READ getSize WRITE setSize)
public:
    NotifyUI(QWidget *parent = nullptr);
    virtual~NotifyUI();
    NotifyUI *setNotifyText(QString &txt);
    int showTime();   // ms
    void showNotify(int ms = DefalutTime, NotifyManager::NotifyOrder order = NotifyManager::Queue);
    static void ShowNotify(QString &txt);
    static void Show3sNotify(QString txt, int time = 3000);
    int getHeight();
    void setHeight(int h);
    int getSize();
    void setSize(int s);
signals:
    void signalClose();
    void signalTimeOver();
private slots:
    void slotTimeOver();
    void hideNotify();
    void init();
    NotifyUI *copy();
    void setValid(bool b);
    bool isValid();
    void showAnimation();
    void closeAnimation();
    void hideAnimation();
    void stressAnimation();
    void recoverAnimation();

private:
    QSize showsize;
    QString text;
    int fontsize;
    int timerId;
    int msTime;
    int zoomSize;
    bool valid;
    QTime time;
    QPropertyAnimation *animation;
    QPropertyAnimation *animationSize;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *e);
};

#endif // NOTIFYUI_H
