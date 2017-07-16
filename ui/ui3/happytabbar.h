#ifndef HAPPYTABBAR_H
#define HAPPYTABBAR_H

#include <QTabBar>

class HappyTabBar : public QTabBar
{
public:
    HappyTabBar(QWidget *parent = 0);
    ~HappyTabBar();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // HAPPYTABBAR_H
