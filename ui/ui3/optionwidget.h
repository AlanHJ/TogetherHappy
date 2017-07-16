#ifndef OPTIONWIDGET_H
#define OPTIONWIDGET_H

#include <QWidget>

class OptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OptionWidget(QWidget *parent = 0);
    ~OptionWidget();
    void setIndex(int id);
protected:
    int index;
};

#endif // OPTIONWIDGET_H
