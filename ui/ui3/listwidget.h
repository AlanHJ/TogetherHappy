#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
public:
    ListWidget(QWidget *w = 0);
    ~ListWidget();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    QColor c1;
    QColor c2;
};

#endif // LISTWIDGET_H
