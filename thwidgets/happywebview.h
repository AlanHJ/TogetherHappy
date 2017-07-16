#ifndef HAPPYWEBVIEW_H
#define HAPPYWEBVIEW_H

#include <QWebView>

class HappyWebView : public QWebView
{
    Q_OBJECT
public:
    HappyWebView(QWidget *parent = 0);
    virtual~HappyWebView();
    void clear();
    void showContent();
    void insertHtml(QString html);
    void insertText(QString txt, Qt::GlobalColor textcolor = Qt::black);
    void scollMax();
    QString toPlainText();
    void setScollEnable(bool b);
signals:
    void signalFavoriteImagePath(QString url);
private:
    class PrivData;
    PrivData *mData;

private slots:
    void addFavorites();
    void htmlLoadFinished(bool ok);
    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *);
};

#endif // HAPPYWEBVIEW_H
