#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QProgressBar>

class QPropertyAnimation;
class ProgressBar : public QProgressBar
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getopacity WRITE setopacity)
public:
    ProgressBar(QWidget *parent = 0);
    ~ProgressBar();
    void showAnimation();
    qreal getopacity();
    void setopacity(qreal opacity);

private:
    qreal opacity_;
    QPropertyAnimation *animation;
};

#endif // PROGRESSBAR_H
