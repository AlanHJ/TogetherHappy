#ifndef CHAT_H
#define CHAT_H

#include <QLabel>
#include <QMouseEvent>
#include <QTextDocument>
#include "../thwidgetbase.h"

class MsgLine : public QLabel
{
    Q_OBJECT
public:
    MsgLine(QWidget *parent = 0)
        :   QLabel(parent) {
        isPress = false;
        setMinimumWidth(396);
        setStyleSheet(QStringLiteral("font-family:微软雅黑;font:10px;color:white;"));
    }
    ~MsgLine() {}
    void setMsg(QString html, QString src, QString dt) {
        QTextDocument doc;
        doc.setHtml(html);
        QString plaintext = doc.toPlainText();
        if (html.contains("<img src"))
            plaintext = QStringLiteral("图片信息");
        QString str = QStringLiteral(" 你有新消息 ") +
                dt + " " + src + ": " + plaintext;
        QFontMetrics fm = this->fontMetrics();
        QString elideStr =
                fm.elidedText(str, Qt::ElideRight,
                               this->width());
        setText(elideStr);
    }

signals:
    void Clicked();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *e) {
        if (e->button() != Qt::LeftButton)
            return QLabel::mousePressEvent(e);
        isPress = true;
    }

    void mouseReleaseEvent(QMouseEvent *e) {
        if (e->button() != Qt::LeftButton)
            return QLabel::mouseReleaseEvent(e);
        isPress = false;
        emit Clicked();
    }

private:
    bool isPress;
};

//#include <QHBoxLayout>
//class MsgPupw : public THWidgetBase
//{
//    Q_OBJECT
//public:
//    MsgPupw(QWidget *w = 0)
//        : THWidgetBase(w, false) {
//        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
//        line = new MsgLine(w);
//        QHBoxLayout *h = new QHBoxLayout(this);
//        h->setContentsMargins(0, 0, 0, 0);
//        h->addWidget(line);
//        setFixedHeight(20);
//        connect(line, &MsgLine::Clicked,
//                this, &MsgPupw::MsgClicked);
//    }
//    virtual~MsgPupw(){}
//    void setWidth(int w) {
//        setFixedWidth(w);
//        setTitleBarWidth(w);
//    }

//    void setMsg(QString &html, QString src) {
//        QTextDocument doc;
//        doc.setHtml(html);
//        line->setMsg(src + ":" + doc.toPlainText());
//    }

//signals:
//    void MsgClicked();
//private:
//    MsgLine *line;
//};

class QToolBar;
class QPushButton;
class QToolButton;
class QButtonGroup;
class HappyTextEdit;
class QStackedWidget;
class Chat : public THWidgetBase
{
    Q_OBJECT
public:
    Chat(bool shake = 1, QWidget *w = 0);
    virtual~Chat();
    bool isAlertFlag();
    qint64 getLastRecvTime();
    void setLastRecvTime(qint64 time);
    virtual void init() = 0;
    virtual void setChatTitle(QString content, QString ico);
    enum RecvCmd{shake = 0, fileprev, other};
public slots:
    virtual void sendMsg() = 0;
    virtual void recvMsg(QString html, QString src) = 0;
    virtual void execRecvCmd(RecvCmd cmd, QVariant var = QVariant()) = 0;
signals:
    void chatClose();
    void chatShow();

protected:
    void inserHtml(QString html, QString src, QColor color = Qt::darkGreen);

protected:
    HappyTextEdit *edit;
    HappyTextEdit *browser;
    QStackedWidget *stack;

    QToolButton *shakeBtn;
    QAction *shakeReject;

    QToolBar *topBar;
    QToolBar *fontBar;
    QButtonGroup *stackBtnGroup;

    QToolBar *centerBar;
    QAction *hidea;
    QPushButton *hideBtn;

    MsgLine *msgpupw;


    bool isAlert;
    qint64 lastRecvTime;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);
    virtual void showEvent(QShowEvent *);

private slots:
    void buttonToggled(int, bool);

};

#endif // CHAT_H
