#ifndef COLORLUMINANCEPICKER_H
#define COLORLUMINANCEPICKER_H

#include <QWidget>

class QColorLuminancePicker : public QWidget
{
    Q_OBJECT
public:
    QColorLuminancePicker(QWidget* parent=0);
    ~QColorLuminancePicker();

public slots:
    void setCol(int h, int s, int v);
    void setCol(int h, int s);
    void setColor(QColor color);

signals:
    void newHsv(int h, int s, int v);

protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    enum { foff = 3, coff = 4 }; //frame and contents offset
    int val;
    int hue;
    int sat;

    int x2val(int x);
    int val2x(int val);
    void setVal(int v);

    QPixmap pix;
};

#endif // COLORLUMINANCEPICKER_H
