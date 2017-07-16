#ifndef CALCRECTOPT_H
#define CALCRECTOPT_H

#include <QObject>
#include <QRect>
#include <QPoint>
#include <QImage>
#include <QPainterPath>

#define Calc_Debug
class CalcRectOpt : public QObject
{
    Q_OBJECT
public:
    enum EdgeEnum
    {
        leftEdge,
        rightEdge,
        topEdge,
        bottomEdge,
        undefine,
    };

    CalcRectOpt(QObject *parent = 0);
    virtual~CalcRectOpt();
    void deleteOpt();
    void setImage(QImage image);
signals:
    void signalSetPoint(QPoint pos);
    void signalCalcRect(QRect rect);
#ifdef Calc_Debug
    void signalDebugPath(QPainterPath path);
#endif
private slots:
    void slotCalcRect(QPoint pos);
private:
    class PrivData;
    PrivData *mData;
};

#endif // CALCRECTOPT_H
