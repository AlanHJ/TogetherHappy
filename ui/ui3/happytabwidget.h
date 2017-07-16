#ifndef HAPPYTABWIDGET_H
#define HAPPYTABWIDGET_H

#include <QTabWidget>

class HappyTabBar;
class HappyTabWidget : public QTabWidget
{
public:
    HappyTabWidget(QWidget *parent = 0);
    ~HappyTabWidget();

private:
    HappyTabBar *bar;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // HAPPYTABWIDGET_H
