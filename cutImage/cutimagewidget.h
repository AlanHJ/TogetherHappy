#ifndef CUTIMAGEWIDGET_H
#define CUTIMAGEWIDGET_H

#include <QWidget>

// new cut image style
// full function

class CutImageWidget : public QWidget
{
    Q_OBJECT
protected:
    enum RectPosition
    {
        outside,
        center,
        topleft,
        topright,
        bottomleft,
        bottomright,
        hleftedge,
        hrightedge,
        vtopedge,
        vbottomedge,
    };

    explicit CutImageWidget(QWidget *parent = 0);
    ~CutImageWidget();

public slots:
    static CutImageWidget *GetInstance();
    static void Destruct();
    void init();

private slots:
    void slotCalcRect(QRect rect);
#ifdef Calc_Debug
    void slotDebugPath(QPainterPath path);
#endif
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void paintEvent(QPaintEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *);

private:
    class PrivData;
    PrivData *mData;

    void shotImage();

    static CutImageWidget *cut;
};

#endif // CUTIMAGEWIDGET_H
