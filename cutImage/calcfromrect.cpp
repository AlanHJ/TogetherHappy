#include "calcfromrect.h"
//#include <atlbase.h>
//#include <atlwin.h>
#include <QRect>
#include <QTimer>
#include <QDebug>
#include <QThread>

//#include "../cutImage/calc.h"

class CalcFromRect::PrivData
{
public:
    PrivData()
    {
        thread = new QThread();
    }
    ~PrivData()
    {

    }

public:
    QRect rect;
    QThread *thread;
};

CalcFromRect::CalcFromRect(QObject *parent) : QObject(parent)
{
//    mData = new PrivData();
//    mData->thread->setObjectName("Calc Thread");
//    this->moveToThread(mData->thread);
//    connect(this, SIGNAL(signalCalcFromRect()), this, SLOT(slotCalcFromRect()));
//    connect(this, SIGNAL(signalSetPoint(QPoint)), this, SLOT(slotCalcRect(QPoint)));
//    connect(mData->thread, SIGNAL(finished()),
//            mData->thread, SLOT(deleteLater()));
//    connect(mData->thread, SIGNAL(finished()),
//            this, SLOT(deleteLater()));
//    mData->thread->start();
}

CalcFromRect::~CalcFromRect()
{
    if (nullptr != mData)
    {
        delete mData;
    }
}

void CalcFromRect::deleteCalc()
{
    mData->thread->quit();
}

void CalcFromRect::slotCalcFromRect()
{
//    CSCWinSpy<CSCWinFilter>::GetInstance()->SnapshotAllWinRect();
//     HWND hwnd = GetDesktopWindow();

//     EnumChildWindows(hwnd, &CalcFromRect::EnumChildProc, NULL);

    //     QTimer::singleShot(5000, this, SLOT(slotTimeOver()));
}

void CalcFromRect::slotCalcRect(QPoint /*pos*/)
{
//    CPoint pt;
//    pt.SetPoint(pos.x(), pos.y());
//    CRect rtSelect = CSCWinSpy<CSCWinFilter>::GetInstance()->GetWinRectByPoint(pt, FALSE);
//    CPoint topleft = rtSelect.TopLeft();
//    CSize size = rtSelect.Size();
//    QRect qrect(QPoint(topleft.x, topleft.y), QSize(size.cx, size.cy));
//    if (qrect.width() >= 1920 && qrect.height() >= 1080)
//    {
//        return;
//    }
//    emit signalRect(qrect);
}

//BOOL CalcFromRect::EnumChildProc(HWND chwnd, LPARAM /*lp*/)
//{
//    if (IsWindow(chwnd))
//    {
//        LPRECT rect;
//        GetWindowRect(chwnd, rect);



//        QRect rt;
//        rt.setLeft((int)rect->left);
//        rt.setRight((int)rect->right);
//        rt.setTop((int)rect->top);
//        rt.setBottom((int)rect->bottom);
//        if (rt.isValid())
//        qDebug() << "left:" << rect->left
//                 << " rigth:" << rect->right
//                 << " top:" << rect->top
//                 << "bottom: " << rect->bottom << rt;
//    }

//    return TRUE;
//}

