#ifndef THEMEPICKER_H
#define THEMEPICKER_H

#include "../thwidgetbase.h"

class QButtonGroup;
class ThemePicker : public THWidgetBase
{
    Q_OBJECT
public:
    explicit ThemePicker(QWidget *parent = 0);
    ~ThemePicker();

signals:

private slots:
    void SelectColorByHsv(int h, int s, int v);
    void PickColor();

private:
    QButtonGroup *group;
};

#endif // THEMEPICKER_H
