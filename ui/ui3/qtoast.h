#ifndef QTOAST_H
#define QTOAST_H

#include <QLabel>

class QPropertyAnimation;
class QToast : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int aniHeight READ getHeight WRITE setHeight)
    Q_PROPERTY(QSize aniSize READ getSize WRITE setSize)
public:
    QToast(QWidget *parent = 0);
    ~QToast();
    static void ShowToast(QString txt);

    int getHeight();
    void setHeight(int h);
    QSize getSize();
    void setSize(QSize s);

private slots:
    void showAnimation();
    void hideAnimation();
    void stressAnimation();
    void recoverAnimation();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    QPropertyAnimation *anima1;
    QPropertyAnimation *anima2;
};

#endif // QTOAST_H
