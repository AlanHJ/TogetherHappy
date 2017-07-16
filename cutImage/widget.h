#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QColor>
#include <QLabel>
#include <QLineEdit>
#include <QTextCodec>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QAbstractNativeEventFilter>

// those class useless

class ColorWidget : public QWidget
{
public:
    ColorWidget(QWidget *parent = 0);
    ~ColorWidget();
    void setRGB(int r, int g, int b);
    void saveImage();
    QPixmap getpixmap();
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    int mR;
    int mG;
    int mB;
};

class FrameWidget : public QWidget
{
    Q_OBJECT
public:
    FrameWidget(QWidget *parent = 0);
    ~FrameWidget();
    void setImage(QImage image);
    void setRect(QRect rect);
    void setLimit(QPoint point);
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
signals:
    void movePoint(QPoint point);
    void screenshotImage();
private:
    QImage mImage;
    QRect mRect;
    QPoint mPoint;
    QPoint mLimitPoint;
    bool mPressed;
    QTime mTime;
    int mKeyPressCounts;
};

class ScreenshotWidget : public QWidget
{
    Q_OBJECT
public:
    ScreenshotWidget(QWidget *parent = 0);
    ~ScreenshotWidget();
    void setImage(QImage image);
    void setTray(QSystemTrayIcon *tray);
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    void closeFrame();

    QImage mImage;
    QRect  mRect;
    FrameWidget *mFrame;
    QSystemTrayIcon *mTray;
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
private slots:
    void frameMove(QPoint point);
    void screenshotImage();
signals:
    void pointRGB(QColor color);
    void completed();
    // QObject interface
public:
    virtual bool eventFilter(QObject *obj, QEvent *e);
};

class CutWidget : public QWidget , QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    CutWidget(QWidget *parent = 0);
    ~CutWidget();
private slots:
    void completed();
    void pointColor();
    void copyRGB();
    void clickIsShow(bool b);
    void clickIsTop(bool b);
    void editchange(QString text);
    void activeTray(QSystemTrayIcon::ActivationReason enums);
    void fillColor(QColor color);
private:
    void createUI();
    ColorWidget *colorUI;
    QLineEdit *mRedit;
    QLineEdit *mGedit;
    QLineEdit *mBedit;
    QPushButton *mCheck;
    QSystemTrayIcon *mTray;
    ScreenshotWidget *imageW;
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseMoveEvent(QMouseEvent *e);

    // QAbstractNativeEventFilter interface
public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};


static QString cnr(const char *c)
{
    return QTextCodec::codecForLocale()->toUnicode(c);
}

#endif // WIDGET_H
