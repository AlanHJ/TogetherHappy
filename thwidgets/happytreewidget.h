#ifndef HAPPYTREEWIDGET_H
#define HAPPYTREEWIDGET_H

#include <QTreeWidget>

class QMenu;
class QAction;
class ItemEditor;
class QPropertyAnimation;
class HappyTreeWidget : public QTreeWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getopacity WRITE setopacity NOTIFY opacityChanged)
public:
    HappyTreeWidget(QWidget *w = 0);
    ~HappyTreeWidget();

    qreal getopacity();
    void setopacity(qreal opacity);

signals:
    void opacityChanged(qreal opacity);

    void memberDoubleClicked(ItemEditor *ie);
    void topmemberDoubleClicked(ItemEditor *ie);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *e);

private slots:
    void actionRename();
    void showAnimation();
    void hiedAnimation();

private:
    qreal opac;
    bool isHide;

    QPropertyAnimation *animation;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void wheelEvent(QWheelEvent *);
};

#endif // HAPPYTREEWIDGET_H
