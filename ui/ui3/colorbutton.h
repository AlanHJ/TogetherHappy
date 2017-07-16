#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QAbstractButton>

class ColorButton : public QAbstractButton
{
    Q_OBJECT
public:
    ColorButton(QWidget *parent = 0);
    ~ColorButton();
    QColor CurrentColor();
public slots:
    void setColor(QColor color);
    // QWidget interface
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

private:
    QColor color;
    int alpha;
};

#endif // COLORBUTTON_H
