#ifndef THWIDGETBASE_H
#define THWIDGETBASE_H

#include <QWidget>

static const QSize CIRCLE = QSize(8, 8);
class QTimeLine;
class HappyTitleBar;
class THWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit THWidgetBase(QWidget *parent = 0, bool shadow = true);
    virtual~THWidgetBase();
    void setTitleBarWidth(int width);
    void hideTitleBar();
    void setSizeEnable(bool b, QSize max = CIRCLE);
    void updateColor(QColor color1, QColor color2);
    void setBezierEnable(bool b);
    qint64 getActivateTime();
signals:

private:
    QRect centerRect(QPoint center, QSize size = CIRCLE);

protected:

    HappyTitleBar *bar;
    qint64 activeTime;

    int bezierX;
    int bezierY;
    bool bezierEnable;
    QColor bezierColor;
    QTimeLine *bezierTime;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *);

private:
    QPoint position;
    bool leftPressed;
    bool isResize;

    QColor startColor;
    QColor endColor;

    QSize minSize;

    bool showShadow;

};

#endif // THWIDGETBASE_H
