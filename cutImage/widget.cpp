#include "widget.h"
#include <QApplication>
#include <QStyleFactory>
#include <QRegExp>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QValidator>
#include <QMenu>
#include <QAction>
#include <QDesktopWidget>
#include <QClipboard>
#include <QScreen>
#include <QRgb>
#include <Windows.h>

CutWidget::CutWidget(QWidget *parent)
    : QWidget(parent)
{
    createUI();
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
    QApplication::setStyle(QStyleFactory::create(QStyleFactory::keys().last()));

    QMenu *menu = new QMenu();
    QAction *topAction = new QAction(cnr("前端显示"), this);
    topAction->setCheckable(true);
    connect(topAction, SIGNAL(toggled(bool)), this, SLOT(clickIsTop(bool)));
    QAction *quit = new QAction(cnr("退出"), this);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    menu->addAction(topAction);
    menu->addAction(quit);
    mTray = new QSystemTrayIcon(this);
    mTray->setContextMenu(menu);
    mTray->setToolTip(cnr("小工具"));
    connect(mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activeTray(QSystemTrayIcon::ActivationReason)));
    QIcon ico(colorUI->getpixmap());
    mTray->setIcon(ico);
    mTray->show();

    if (RegisterHotKey((HWND)this->winId(), 0, MOD_CONTROL | MOD_ALT, 'Z') == false)
    {
//        qDebug() << "RegisterHotKey fail";
        mTray->showMessage(cnr("警告"), cnr("热键注册失败"));
    }
    qApp->installNativeEventFilter(this);
    imageW = nullptr;
    setObjectName("main widget");
    setWindowTitle(cnr("小工具"));
}

CutWidget::~CutWidget()
{

}

void CutWidget::completed()
{
    mTray->showMessage(cnr("提示"), cnr("已截图到剪贴板"));
}

void CutWidget::pointColor()
{
    if (nullptr != imageW)
    {
        imageW->close();
//        delete imageW;
        imageW = nullptr;
    }
    imageW  = new ScreenshotWidget();
    connect(imageW, SIGNAL(pointRGB(QColor)), this, SLOT(fillColor(QColor)));
    connect(imageW, SIGNAL(completed()), this, SLOT(completed()));
    if (mCheck->isChecked())
    {
        this->hide();
    }
    QImage image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
    imageW->setImage(image);
    imageW->setTray(mTray);
    imageW->setWindowState(Qt::WindowFullScreen);
    imageW->move(0, 0);
    QDesktopWidget *desk = QApplication::desktop();
    qDebug() << desk->width() << " " << desk->height();
//    imageW->resize(1920, 1080);
    imageW->show();
    imageW->activateWindow();
}

void CutWidget::copyRGB()
{
    QClipboard *borad = QApplication::clipboard();
    borad->setText(mRedit->text() + "," + mGedit->text() + "," + mBedit->text());
    mTray->showMessage(cnr("提示"), cnr("已将RGB数值复制到剪贴板"));
}

void CutWidget::clickIsShow(bool b)
{
    if (b)
    {
        mCheck->setText(cnr("隐藏"));
    }
    else
    {
        mCheck->setText(cnr("显示"));
    }
}

void CutWidget::clickIsTop(bool b)
{
    if (b)
    {
        setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    }
    else
    {
        setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
    }
    show();
    activateWindow();
}


void CutWidget::editchange(QString)
{
    int r = mRedit->text().toInt();
    int g = mGedit->text().toInt();
    int b = mBedit->text().toInt();
    colorUI->setRGB(r, g, b);
    colorUI->repaint();
    mTray->setIcon(QIcon(colorUI->getpixmap()));
}

void CutWidget::activeTray(QSystemTrayIcon::ActivationReason enums)
{
    switch (enums)
    {
//    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->show();
        this->activateWindow();
        break;
    }
}

void CutWidget::fillColor(QColor color)
{
    mRedit->setText(QString::number(color.red()));
    mGedit->setText(QString::number(color.green()));
    mBedit->setText(QString::number(color.blue()));
}

void CutWidget::createUI()
{
    QRegExp regx("[0-9]+$");

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    QVBoxLayout *vbox = new QVBoxLayout();

    QHBoxLayout *h1 = new QHBoxLayout();
    QLabel *mRlabel = new QLabel(cnr("红:"), this);
    mRedit = new QLineEdit(this);
    mRlabel->setBuddy(mRedit);
    mRedit->setValidator(new QIntValidator(0, 255, this));
    mRedit->setMaxLength(3);
    h1->addWidget(mRlabel, 1);
    h1->addWidget(mRedit, 3);

    QHBoxLayout *h2 = new QHBoxLayout();
    QLabel *mGlabel = new QLabel(cnr("绿:"), this);
    mGedit = new QLineEdit(this);
    mGlabel->setBuddy(mGedit);
    mGedit->setValidator(new QIntValidator(0, 255, this));
    mGedit->setMaxLength(3);
    h2->addWidget(mGlabel, 1);
    h2->addWidget(mGedit, 3);

    QHBoxLayout *h3 = new QHBoxLayout();
    QLabel *mBlabel = new QLabel(cnr("蓝:"), this);
    mBedit = new QLineEdit(this);;
    mBlabel->setBuddy(mBedit);
    mBedit->setValidator(new QIntValidator(0, 255, this));
    mBedit->setMaxLength(3);
    h3->addWidget(mBlabel, 1);
    h3->addWidget(mBedit, 3);

    vbox->addLayout(h1);
    vbox->addLayout(h2);
    vbox->addLayout(h3);

    colorUI = new ColorWidget(this);
    hbox->addWidget(colorUI, 1);
    hbox->addLayout(vbox, 1);

    QHBoxLayout *btlyt = new QHBoxLayout();
    QPushButton *pb1 = new QPushButton(cnr("截图"), this);
    connect(pb1, SIGNAL(clicked()), this, SLOT(pointColor()));
    mCheck = new QPushButton(cnr("显示"), this);
    mCheck->setCheckable(true);
    connect(mCheck, SIGNAL(clicked(bool)), this, SLOT(clickIsShow(bool)));
    QPushButton *pb2 = new QPushButton(cnr("复制"), this);
    connect(pb2, SIGNAL(clicked()), this, SLOT(copyRGB()));
    btlyt->addStretch(1);
    btlyt->addWidget(pb1, 1);
    btlyt->addWidget(mCheck, 1);
    btlyt->addWidget(pb2, 1);
    btlyt->addStretch(1);

    mRedit->setText("0");
    mGedit->setText("0");
    mBedit->setText("0");
    connect(mRedit, SIGNAL(textChanged(QString)), this, SLOT(editchange(QString)));
    connect(mGedit, SIGNAL(textChanged(QString)), this, SLOT(editchange(QString)));
    connect(mBedit, SIGNAL(textChanged(QString)), this, SLOT(editchange(QString)));

    layout->addLayout(hbox, 3);
    layout->addLayout(btlyt, 1);
    setLayout(layout);
}

void CutWidget::closeEvent(QCloseEvent *)
{
    hide();
    mTray->showMessage(cnr("提示"), cnr("已隐藏到任务栏"));
}

void CutWidget::paintEvent(QPaintEvent *)
{

}

void CutWidget::mouseMoveEvent(QMouseEvent *)
{

}

bool CutWidget::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
    if (eventType == "windows_generic_MSG")
    {
        MSG *msg = (MSG *)message;
        if (msg->message == WM_HOTKEY && msg->wParam == 0)
        {
//            qDebug() << "hotkey";
            pointColor();
            return true;
        }
    }
    return false;
}

ColorWidget::ColorWidget(QWidget *parent)
: QWidget(parent)
{
    mR = 0;
    mG = 0;
    mB = 0;
    setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

ColorWidget::~ColorWidget()
{
    QPainter painter(this);
}

void ColorWidget::setRGB(int r, int g, int b)
{
    mR= r;
    mG = g;
    mB = b;
}

void ColorWidget::saveImage()
{
//    this->grab().toImage().save("c:\\RGB.png");

    QPixmap::grabWindow(QApplication::desktop()->winId()).toImage().save("c:\\window.png");
}

QPixmap ColorWidget::getpixmap()
{
    return this->grab();
}

void ColorWidget::paintEvent(QPaintEvent *)
{
    // 绘图
//    qDebug() << cnr("绘图");
    QWidget *p= dynamic_cast<QWidget *>(parent());
//    qDebug() << p->width() << "" << p->height();
    QPainter painter(this);
    QColor color(mR, mG, mB);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true); //反锯齿
    painter.setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(color, Qt::SolidPattern));
    painter.drawEllipse(p->width() / 12, p->height() / 12, 60, 60);
}


ScreenshotWidget::ScreenshotWidget(QWidget *parent)
    : QWidget(parent)
{
    mFrame = nullptr;
    mTray = nullptr;
    mRect.setRect(0, 0, 0, 0);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    qApp->installEventFilter(this);
}

ScreenshotWidget::~ScreenshotWidget()
{
    closeFrame();
}

void ScreenshotWidget::setImage(QImage image)
{
    mImage = image;
    this->repaint();
}

void ScreenshotWidget::setTray(QSystemTrayIcon *tray)
{
    this->mTray = tray;
}

void ScreenshotWidget::paintEvent(QPaintEvent *e)
{
    QRectF target(e->rect());
    QRectF source(mImage.rect());
    QPainter painter(this);
    // 绘制图片
    painter.drawImage(target, mImage, source, Qt::ColorMode_Mask);
    // 绘制矩形框
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);
    painter.drawRect(mRect);
    // 绘制灰度框
    QPainterPath path;
    path.addRect(e->rect());
    path.addRect(mRect);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 60)));
}

void ScreenshotWidget::closeFrame()
{
    if (nullptr != mFrame)
    {
        mFrame->close();
        delete mFrame;
        mFrame = nullptr;
    }
}

void ScreenshotWidget::mousePressEvent(QMouseEvent *e)
{
    closeFrame();
    if (e->button() == Qt::LeftButton)
    {
        mRect.setRect(e->pos().x(), e->pos().y(), 0, 0);
    }
}

void ScreenshotWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        closeFrame();
        mFrame = new FrameWidget();
        connect(mFrame, SIGNAL(movePoint(QPoint)), this, SLOT(frameMove(QPoint)), Qt::UniqueConnection);
        connect(mFrame, SIGNAL(screenshotImage()), this, SLOT(screenshotImage()), Qt::UniqueConnection);
        mFrame->setRect(mRect);
        mFrame->show();
        mFrame->activateWindow();
        if (mRect.width() > 0)
        {
            mFrame->move(e->pos().x() - mRect.width(), e->pos().y() - mRect.height());
        }
        else
        {
            mFrame->move(e->pos().x(), e->pos().y());
        }
        mFrame->setLimit(QPoint(width(), height()));
    }
}

void ScreenshotWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {

    }
}

void ScreenshotWidget::mouseMoveEvent(QMouseEvent *e)
{

    int x = mRect.x();
    int y = mRect.y();
    int x1 = e->pos().x();
    int y1 = e->pos().y();
    int w = x1 - x;
    int h = y1 - y;
    // 限制范围
    if (w > width() - x)
    {
        w = width() - x - 1;
    } else if (w < -x)
    {
        w = -x;
    }
    if (h > height() - y)
    {
        h = height() - y - 1;
    } else if (h < -y)
    {
        h = -y;
    }
    mRect.setRect(x, y, w, h);
    repaint();
}

void ScreenshotWidget::frameMove(QPoint point)
{
    mRect.setX(point.x());
    mRect.setY(point.y());
    mRect.setWidth(mFrame->width() - 1);
    mRect.setHeight(mFrame->height() - 1);
    repaint();
}

void ScreenshotWidget::screenshotImage()
{
    QImage image = mImage.copy(mRect);
    QClipboard *board = qApp->clipboard();
    board->setImage(image);
//    mTray->showMessage(cnr("提示"), cnr("已截图到剪贴板"));
//    emit completed();
    closeFrame();
    hide();
}

bool ScreenshotWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == this && e->type() == QEvent::MouseMove)
    {
        QColor color;
        color.setRgb(mImage.pixel(cursor().pos()));
        emit pointRGB(color);
//        qDebug() << cursor().pos().x() << " " << cursor().pos().y();
    }
    else if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_Escape)
        {
            if (nullptr != mFrame)
            {
                mFrame->hide();
            }
            this->hide();
        }
//        qDebug() << "Ate key press" << keyEvent->key();
    }
    return QWidget::eventFilter(obj, e);
}


FrameWidget::FrameWidget(QWidget *parent)
    : QWidget(parent)
{
    mPressed = false;
    mKeyPressCounts = -1;
    mTime.start();
    QDesktopWidget *desk = QApplication::desktop();
    mLimitPoint.setX(desk->width());
    mLimitPoint.setY(desk->height());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

FrameWidget::~FrameWidget()
{

}

void FrameWidget::setImage(QImage image)
{
    mImage = image;
    this->resize(mImage.size());
}

void FrameWidget::setRect(QRect rect)
{
    mRect = rect;
    this->resize(abs(mRect.width()) + 1, abs(mRect.height()) + 1);
}

void FrameWidget::setLimit(QPoint point)
{
    this->mLimitPoint = point;
}

void FrameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    mRect.setX(0);
    mRect.setY(0);
    mRect.setWidth(rect().width() - 1);
    mRect.setHeight(rect().height() - 1);
    painter.drawRect(mRect);
    painter.fillRect(mRect, QBrush(QColor(0, 0, 0, 1)));
}

void FrameWidget::mousePressEvent(QMouseEvent *e)
{
    // 按下的初始位置
    if (e->button() == Qt::LeftButton)
    {
        mPoint = e->globalPos() - this->pos();
        e->accept();
        mPressed = true;
    }
}

void FrameWidget::mouseReleaseEvent(QMouseEvent *)
{
    mPressed = false;
}

void FrameWidget::mouseMoveEvent(QMouseEvent *e)
{
    // change cursor

    // 移动的距离
    if (mPressed && (e->buttons() && Qt::LeftButton) && !isMaximized())
    {
        // 限制移动
        QPoint p = e->globalPos() - mPoint;
        if (p.x() < 0)
        {
            p.setX(0);
        } else if (p.x() > mLimitPoint.x() - width())
        {
            p.setX(mLimitPoint.x() - width());
        }
        if (p.y() < 0)
        {
            p.setY(0);
        } else if (p.y() > mLimitPoint.y() - height())
        {
            p.setY(mLimitPoint.y() - height());
        }
        emit movePoint(p);
        this->move(p);
        e->accept();
    }
}

void FrameWidget::mouseDoubleClickEvent(QMouseEvent *)
{
//    if (e->button() == Qt::LeftButton)
    {
        emit screenshotImage();
    }
}


void FrameWidget::keyPressEvent(QKeyEvent *e)
{
    if (mTime.elapsed() > 100)
    {
        mKeyPressCounts = -1;
    }
    mKeyPressCounts++;
    double v = 0.35 * mKeyPressCounts * mKeyPressCounts + 1;
    if (v >= 30)
    {
        v = 30;
    }
    mTime.restart();
    int key = e->key();
    QPoint p = pos();
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
    case Qt::Key_Enter:
        emit screenshotImage();
        return;
    }
    if (p.x() < 0)
    {
        p.setX(0);
    } else if (p.x() > mLimitPoint.x() - width())
    {
        p.setX(mLimitPoint.x() - width());
    }
    if (p.y() < 0)
    {
        p.setY(0);
    } else if (p.y() > mLimitPoint.y() - height())
    {
        p.setY(mLimitPoint.y() - height());
    }
    emit movePoint(p);
    move(p);
}
