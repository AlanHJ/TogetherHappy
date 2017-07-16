#ifndef OPTIONSUI_H
#define OPTIONSUI_H

#include "../thwidgetbase.h"

class QStackedWidget;
class QListWidget;
class OptionsUI : public THWidgetBase
{
    Q_OBJECT
public:
    explicit OptionsUI(QWidget *parent = 0);
    ~OptionsUI();

signals:
    void confirm(int tab);
    void apply(int tab);
    void notify();
public slots:

private:
    QStackedWidget *stack;
    QListWidget *list;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *e);
};

#endif // OPTIONSUI_H
