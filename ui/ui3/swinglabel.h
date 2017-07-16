#ifndef SWINGLABEL_H
#define SWINGLABEL_H

#include <QWidget>

class SwingLabel : public QWidget
{
public:
    SwingLabel(QWidget *parent = 0);
    ~SwingLabel();
    void setImagePath(QString path);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);

private:
    QString imagePath;
    int degree;
};

#endif // SWINGLABEL_H
