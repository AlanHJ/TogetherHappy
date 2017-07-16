#ifndef SELFTOOLBUTTON_H
#define SELFTOOLBUTTON_H

#include <QPushButton>

class SelfToolButton : public QWidget
{
    Q_OBJECT
public:
    SelfToolButton(QWidget *parent = 0);
    virtual~SelfToolButton();
    void setImagePaths(QStringList list);
    void setArrowDownImage(QStringList list);
    void setSign(int id);

public slots:
    void selectButton(int id);

signals:
    void signalClicked();
    void signalArrowClicked();

private:
    bool isEnter;
    int alpha;
    int id;
    QStringList arrowDownPaths;
    QString arrowImage;
    QStringList imagePaths;
    QString mainImage;
    QRect target1;
    QRect target2;

    // QWidget interface
protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // SELFTOOLBUTTON_H
