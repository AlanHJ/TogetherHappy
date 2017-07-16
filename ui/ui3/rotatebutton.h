#ifndef ROTATEBUTTON_H
#define ROTATEBUTTON_H

#include <QAbstractButton>

class RotateButton : public QAbstractButton
{
    Q_OBJECT
public:
    RotateButton(QWidget *parent = 0);
    ~RotateButton();
    void setImagePath(QString path);
    void setLoop(int times);

signals:
    void stopAnimation();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QString imagePath;
    int alpha;
    int degree;
    int loop;
};

#endif // ROTATEBUTTON_H
