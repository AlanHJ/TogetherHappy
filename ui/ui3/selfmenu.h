#ifndef SELFMENU_H
#define SELFMENU_H

#include <QMenu>

class SelfMenu : public QMenu
{
    Q_OBJECT
public:
    explicit SelfMenu(QWidget *parent = 0);
    SelfMenu(QString title, QWidget *parent = 0);
    ~SelfMenu();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

private:
    QColor c1;
    QColor c2;

    void init();
};

#endif // SELFMENU_H
