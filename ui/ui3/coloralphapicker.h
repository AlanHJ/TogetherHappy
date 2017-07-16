#ifndef COLORALPHAPICKER_H
#define COLORALPHAPICKER_H

#include <QAbstractButton>

class ColorAlphaPicker : public QAbstractButton
{
    Q_OBJECT
public:
    explicit ColorAlphaPicker(QWidget *parent = 0);
    ~ColorAlphaPicker();
    void setRgb(int r, int g, int b, int a = 10);
    void setColor(QColor color);
    QColor currentColor();
signals:
    void newColor(QColor color);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *m);
    virtual void mouseMoveEvent(QMouseEvent *m);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void paintEvent(QPaintEvent *);

private:

    int r;
    int g;
    int b;
    int a;

    int alp2x(int a);
    int x2alp(int x);
    void setAlp(int a);
};

#endif // COLORALPHAPICKER_H
