#ifndef CALCFROMRECT_H
#define CALCFROMRECT_H

#include <QObject>
#include <QRect>

struct CalcRect
{
    int left;
    int right;
    int top;
    int bottom;
};

class CalcFromRect : public QObject
{
    Q_OBJECT
public:
    explicit CalcFromRect(QObject *parent = 0);
    ~CalcFromRect();
    void deleteCalc();
signals:
    void signalSetPoint(QPoint pos);
    void signalRect(QRect rect);
    void signalCalcFromRect();
private slots:
    void slotCalcFromRect();
    void slotCalcRect(QPoint pos);
private:
    class PrivData;
    PrivData *mData;
//static BOOL CALLBACK EnumChildProc(HWND chwnd, LPARAM lp);
};

#endif // CALCFROMRECT_H
