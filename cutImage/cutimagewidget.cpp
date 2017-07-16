#include "cutimagewidget.h"
#include "calcfromrect.h"
#include "../config/configuredata.h"
#include <QTime>
#include <QDebug>
#include <QScreen>
#include <QPainter>
#include <QClipboard>
#include <QStaticText>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QApplication>
#include <QDesktopWidget>
#include "../ui/notifyui.h"

#define ShootDebug
#define PrintPoint(p)(painter.drawText(p, QString("x:%1, y:%2").arg(p.x()).arg(p.y())))
CutImageWidget *CutImageWidget::cut = nullptr;
class CutImageWidget::PrivData
{
public:

    PrivData()
    {
        color = Qt::red;
        rectP = CutImageWidget::outside;
        lPressed = false;
        rect.setRect(0, 0, 0, 0);
        time.start();
        moveKeyCounts = -1;
//        opt = new CalcRectOpt();
//        calc = new CalcFromRect();
    }
    ~PrivData()
    {
//        if (opt != nullptr)
//        {
//            opt->deleteOpt();
//        }
//        calc->deleteCalc();
    }
    QRect centerRect(QPoint center, QSize size = QSize(8, 8))
    {
        QRect rect(QPoint(0, 0), size);
        rect.moveCenter(center);
        return rect;
    }
    QRect containsRect(CutImageWidget::RectPosition p)
    {
        QRect rt;
        if (p == CutImageWidget::hleftedge)
        {
            rt = centerRect(QPoint(rect.center().x() - rect.width() / 2, rect.center().y()),
                            QSize(8, rect.height() - 8));
        }
        else if (p == CutImageWidget::hrightedge)
        {
            rt = centerRect(QPoint(rect.center().x() + rect.width() / 2, rect.center().y()),
                            QSize(8, rect.height() - 8));
        }
        else if (p == CutImageWidget::vtopedge)
        {
            rt = centerRect(QPoint(rect.center().x(), rect.center().y() - rect.height() / 2),
                            QSize(rect.width() - 8, 8));
        }
        else if (p == CutImageWidget::vbottomedge)
        {
            rt = centerRect(QPoint(rect.center().x(), rect.center().y() + rect.height() / 2),
                            QSize(rect.width() - 8, 8));
        }
        rectP = p;
        return rt;
    }

public:
    CutImageWidget::RectPosition rectP;
    bool lPressed;
    QRect rect;
    QRect lastRect;
    QPoint pressPoint;
    QImage image;
    QTime time;
    int moveKeyCounts;

//    CalcRectOpt *opt;
//    CalcFromRect *calc;

    QPainterPath path;

    QColor color;

};

CutImageWidget::CutImageWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);
    mData = new PrivData();
    setMouseTracking(true);
//    connect(mData->calc, SIGNAL(signalRect(QRect)),
//            this, SLOT(slotCalcRect(QRect)));
//    emit mData->calc->signalCalcFromRect();
//    mData->opt->setImage(mData->image);
//    connect(mData->opt, SIGNAL(signalCalcRect(QRect)),
//            this, SLOT(slotCalcRect(QRect)));
#ifdef Calc_Debug
//    connect(mData->opt, SIGNAL(signalDebugPath(QPainterPath)),
//            this, SLOT(slotDebugPath(QPainterPath)));
#endif

}

CutImageWidget::~CutImageWidget()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

CutImageWidget *CutImageWidget::GetInstance()
{
    if (!cut)
        cut = new CutImageWidget;
    return cut;
}

void CutImageWidget::Destruct()
{
    if (cut)
        delete cut;
    cut = nullptr;
}

void CutImageWidget::init()
{
    NotifyUI::Show3sNotify(QStringLiteral("Ë«»÷½ØÍ¼£¬ESCÍË³ö½ØÍ¼"), 1500);
    mData->image = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId()).toImage();
    mData->color = ConfigureData::getInstance()->getColorIni("color1");
    mData->rectP = CutImageWidget::outside;
    mData->lPressed = false;
    mData->rect.setRect(0, 0, 0, 0);
    mData->time.restart();
    mData->moveKeyCounts = -1;
}

void CutImageWidget::slotCalcRect(QRect rect)
{
    // get calc rect
    mData->rect = rect;
//    mData->path.addRect(rect);
    repaint();
}
#ifdef Calc_Debug
void CutImageWidget::slotDebugPath(QPainterPath path)
{
    mData->path = path;
    repaint();
}
#endif
void CutImageWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mData->lPressed = true;
        QPoint p = e->pos();
        mData->pressPoint = p;
        if (!mData->rect.marginsAdded(QMargins(4, 4, 4, 4)).contains(p))
        {
            mData->rect.setRect(0, 0, 0, 0);
            mData->rect.setTopLeft(p);
        }
        mData->lastRect = mData->rect;
    }
}

void CutImageWidget::mouseReleaseEvent(QMouseEvent *)
{
    // show more choice
    mData->lPressed = false;
    mData->rect = mData->rect.normalized();
}

void CutImageWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    shotImage();
}

void CutImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    // change cursor, change size
    QPoint p = e->pos();
//    emit mData->calc->signalSetPoint(p);
    if (p.x() <= 0)
    {
        p.setX(0);
    }
    if (p.x() >= width())
    {
        p.setX(width() - 1);
    }
    if (p.y() <= 0)
    {
        p.setY(0);
    }
    if (p.y() >= height())
    {
        p.setY(height() - 1);
    }
    if (mData->lPressed)
    {
        switch (mData->rectP)
        {
        case center:
        {
            QRect rct = mData->rect;
            rct.moveTo(mData->lastRect.topLeft() + p - mData->pressPoint);
            QPoint cp = rct.center();
            if (cp.x() <= (rct.width() / 2))
            {
                cp.setX(rct.width() / 2 - 1);
            }
            if (cp.y() <= (rct.height() / 2))
            {
                cp.setY(rct.height() / 2 - 1);
            }
            if (cp.x() >= (width() - rct.width() / 2 - 2))
            {
                cp.setX(width() - rct.width() / 2 - 2);
            }
            if (cp.y() >= (height() - rct.height() / 2 - 2))
            {
                cp.setY(height() - rct.height() / 2 - 2);
            }
            mData->rect.moveTo(mData->lastRect.topLeft() + p - mData->pressPoint);
            mData->rect.moveCenter(cp);
        }
            break;
        case bottomleft:
            mData->rect.setBottomLeft(mData->lastRect.bottomLeft() + p - mData->pressPoint);
            break;
        case bottomright:
            mData->rect.setBottomRight(mData->lastRect.bottomRight() + p - mData->pressPoint);
            break;
        case topleft:
            mData->rect.setTopLeft(mData->lastRect.topLeft() + p - mData->pressPoint);
            break;
        case topright:
            mData->rect.setTopRight(mData->lastRect.topRight() + p - mData->pressPoint);
            break;
        case hleftedge:
            mData->rect.setLeft(mData->lastRect.left() + p.x() - mData->pressPoint.x());
            break;
        case hrightedge:
            mData->rect.setRight(mData->lastRect.right() + p.x() - mData->pressPoint.x());
            break;
        case vtopedge:
            mData->rect.setTop(mData->lastRect.top() + p.y() - mData->pressPoint.y());
            break;
        case vbottomedge:
            mData->rect.setBottom(mData->lastRect.bottom() + p.y() - mData->pressPoint.y());
            break;
        default:
            mData->rect.setSize(QSize(p.x() - mData->pressPoint.x(), p.y() - mData->pressPoint.y()));
            break;
        }
        if (mData->rectP != center)
        {
            if (mData->rect.x() <= 0)
            {
                mData->rect.setX(0);
            }
            if (mData->rect.x() >= width())
            {
                mData->rect.setWidth(width() - 1);
            }
            if (mData->rect.y() <= 0)
            {
                mData->rect.setY(0);
            }
            if (mData->rect.y() >= height())
            {
                mData->rect.setY(height() - 1);
            }
            QRect rct = mData->rect.normalized();
            if (rct.width() + rct.x() >= width())
            {
                mData->rect.setWidth(width() - rct.x() - 1);
            }
            if (rct.height() + rct.y() >= height())
            {
                mData->rect.setHeight(height() - rct.y() - 1);
            }
        }
    }
    else
    {
        if (mData->centerRect(mData->rect.bottomLeft()).contains(p))
        {
            mData->rectP = bottomleft;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        else if (mData->centerRect(mData->rect.topRight()).contains(p))
        {
            mData->rectP = topright;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        else if (mData->centerRect(mData->rect.bottomRight()).contains(p))
        {
            mData->rectP = bottomright;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else if (mData->centerRect(mData->rect.topLeft()).contains(p))
        {
            mData->rectP = topleft;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else if (mData->containsRect(hleftedge).contains(p))
        {
            setCursor(QCursor(Qt::SizeHorCursor));
        }
        else if (mData->containsRect(hrightedge).contains(p))
        {
            setCursor(QCursor(Qt::SizeHorCursor));
        }
        else if (mData->containsRect(vtopedge).contains(p))
        {
            setCursor(QCursor(Qt::SizeVerCursor));
        }
        else if (mData->containsRect(vbottomedge).contains(p))
        {
            setCursor(QCursor(Qt::SizeVerCursor));
        }
        else
        {
            if (mData->rect.contains(p))
            {
                mData->rectP = center;
                setCursor(QCursor(Qt::SizeAllCursor));
            }
            else
            {
                mData->rectP = outside;
                setCursor(QCursor(Qt::ArrowCursor));
            }
        }
    }
    repaint();
}

void CutImageWidget::keyPressEvent(QKeyEvent *e)
{
    mData->lPressed = true;
    if (mData->time.elapsed() > 100)
    {
        mData->moveKeyCounts = -1;
    }
    mData->moveKeyCounts++;
    double v = 0.35 * mData->moveKeyCounts * mData->moveKeyCounts + 1;
    if (v >= 30) { v = 30; }
    mData->time.restart();
    int key = e->key();
    QPoint p = mData->rect.topLeft();
    switch (key)
    {
    case Qt::Key_Up:
        p.setY(p.y() - v);
        break;
    case Qt::Key_Down:
        p.setY(p.y() + v);
        break;
    case Qt::Key_Left:
        p.setX(p.x() - v);
        break;
    case Qt::Key_Right:
        p.setX(p.x() + v);
        break;
    case Qt::Key_Escape:
        this->hide();
        break;
    case Qt::Key_Return:
        shotImage();
        break;
    default:
        return;
    }
    QRect rct = mData->rect;
    rct.moveTo(p);
    QPoint cp = rct.center();
    if (cp.x() <= (rct.width() / 2))
    {
        cp.setX(rct.width() / 2 - 1);
    }
    if (cp.y() <= (rct.height() / 2))
    {
        cp.setY(rct.height() / 2 - 1);
    }
    if (cp.x() >= (width() - rct.width() / 2 - 2))
    {
        cp.setX(width() - rct.width() / 2 - 2);
    }
    if (cp.y() >= (height() - rct.height() / 2 - 2))
    {
        cp.setY(height() - rct.height() / 2 - 2);
    }
    mData->rect.moveTo(p);
    mData->rect.moveCenter(cp);
    repaint();
}

void CutImageWidget::paintEvent(QPaintEvent *e)
{
    QRect target = e->rect();
    QRect source = mData->image.rect();
    QPainter painter(this);
    painter.drawImage(target, mData->image, source, Qt::ColorMode_Mask);
    QPainterPath path;
    path.addRect(e->rect());
    path.addRect(mData->rect);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 60)));
    painter.setBrush(QBrush(QColor(0, 0, 0, 1)));
    painter.setPen(QPen(QBrush(mData->color), 3));
    painter.drawRect(mData->rect);
    if (!mData->lPressed)
    {
#ifdef ShootDebug
    painter.drawPath(mData->path);
#endif
        QString rgb(" r:%1, g:%2, b:%3");
        QColor color;
        QPoint p = mapFromParent(cursor().pos());
        color.setRgb(mData->image.pixel(p));
        rgb = rgb.arg(color.red()).arg(color.green()).arg(color.blue());
        painter.drawStaticText(p + QPoint(0, 20), QStaticText(rgb));
    }
    if (mData->rect.isEmpty() || !mData->rect.isValid())
    {
        return;
    }
    QString str1(" x:%1, y:%2");
    str1 = str1.arg(mData->rect.topLeft().x())
               .arg(mData->rect.topLeft().y());
    QString str2(" w:%1, h:%2");
    str2 = str2.arg(mData->rect.size().width())
               .arg(mData->rect.size().height());
    painter.drawStaticText(mData->rect.bottomRight(), QStaticText(str1));
    painter.drawStaticText(mData->rect.bottomRight() + QPoint(0, 10), QStaticText(str2));
#ifdef ShootDebug
    PrintPoint(mData->rect.topLeft());
    PrintPoint(mData->rect.topRight());
    PrintPoint(mData->rect.bottomLeft());
#endif
}


void CutImageWidget::contextMenuEvent(QContextMenuEvent *)
{
}

void CutImageWidget::shotImage()
{
    QImage image = mData->image.copy(mData->rect);
    QClipboard *board = qApp->clipboard();
    board->setImage(image);
    this->hide();
    NotifyUI::Show3sNotify(QStringLiteral("½ØÍ¼ÒÑ±£´æµ½¼ôÌù°å"), 1000);
}


void CutImageWidget::keyReleaseEvent(QKeyEvent *)
{
    mData->lPressed = false;
}
