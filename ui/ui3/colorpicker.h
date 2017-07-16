#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QFrame>

class QColorPicker : public QFrame
{
    Q_OBJECT
public:
    QColorPicker(QWidget* parent);
    ~QColorPicker();

public slots:
    void setCol(int h, int s, int v);
    void setColor(QColor color);

signals:
    void newCol(int h, int s, int v);

protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    int hue;
    int sat;
    int val;

    QPoint colPt();
    int huePt(const QPoint &pt);
    int satPt(const QPoint &pt);
    int valPt(const QPoint &pt);
    void setCol(const QPoint &pt);

    QPixmap pix;
};

#endif // COLORPICKER_H
