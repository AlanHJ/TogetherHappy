#ifndef HAPPYTITLEBAR_H
#define HAPPYTITLEBAR_H

#include <QWidget>

#define BarHeight 25
#define BarWidth 25
#define ButtonHeight 25
#define ButtonWidth 25

class QLabel;
class QPushButton;
class HappyTitleBar : public QWidget
{
    Q_OBJECT
public:
    enum BarButtonType
    {
        MinButtonHint = 1,
        MaxButtonHint = 2,
        MinMaxButtonHint = 4,
    };

    explicit HappyTitleBar(QWidget *parent);
    ~HappyTitleBar();

    // set bar color
    void setBarColor(int r = 255, int g = 255, int b = 255);
    void setBarColor(QColor color);
    // set bar icon
    void setBarIcon(QString path, QSize size = QSize(22, 22));
    void setBarIcon(QPixmap pix);
    // set bar title
    void setBarContent(QString content);
    // set bar title scroll, true force scroll, false force stop scroll
    void setBarContentScroll(bool b);
    // set bar button hint
    void setBarButtons(BarButtonType type);
    void setBarButtons(int type);
    // set bar width
    void setBarWidth(int width);
    // set extra button stylesheet
    void setExtraButtonStyleSheet(QString css);
    // set extra button visible
    void setExtraButtonVisible(bool visible);

    void setCloseVisible(bool b);

    void setMaxIcon();

signals:
    void signalClose();
    void signalMinimize();
    void signalMaximize();
    void signalExtra();

private slots:
    // MinButton clicked
    void slotMinButtonClicked();
    // MaxButton clicked
    void slotMaxButtonClicked();
    // CloseButton clicked
    void slotCloseButtonClicked();
    // extraButton clicked
    void slotExtraButtonClicked();

private:
    void init();
    void loadStyleCss(QString css);

    QLabel *icon;
    QLabel *content;

    QPushButton *minButton;
    QPushButton *maxButton;
    QPushButton *closeButton;

    QPushButton *extraButton;

    int timerId;
    QColor color;
    bool isScroll;
    QPoint position;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // HAPPYTITLEBAR_H
