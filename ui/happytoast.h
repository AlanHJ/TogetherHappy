#ifndef HAPPYTOAST_H
#define HAPPYTOAST_H

#include <QWidget>

class QLabel;
class HappyToast : public QWidget
{
    Q_OBJECT
public:
    explicit HappyToast(QWidget *parent = 0);
    ~HappyToast();

signals:

public slots:

private:
    void init();

    QLabel *label;
};

#endif // HAPPYTOAST_H
