#ifndef COLORWELL_H
#define COLORWELL_H

#include "../thwidgetbase.h"

class ColorWell : public THWidgetBase
{
    Q_OBJECT
public:
    explicit ColorWell(QColor init, QWidget *parent = 0);
    ~ColorWell();

signals:
    void signalSelectColor(QColor color);
private slots:
    void DefaultColorSelect();
    void SelectColorByHsv(int h, int s, int v);
};

#endif // COLORWELL_H
