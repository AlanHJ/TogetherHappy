#ifndef MUILTPROGRESSBAR_H
#define MUILTPROGRESSBAR_H

#include <QProgressBar>

class MuiltProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit MuiltProgressBar(QWidget *parent = 0);
    ~MuiltProgressBar();

    void reset();
    void setInfo(const QString &value);

public slots:
    void setSecondValue(qreal value);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);

private:
    qreal percent;
    QColor color;
    QString info;
};

#endif // MUILTPROGRESSBAR_H
