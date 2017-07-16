#include "happywebview.h"
#include "../util/thmethod.h"
#include "../ui/moveemojicustom.h"
#include "./emoji/emojilabel.h"
#include "../ui/notifyui.h"
#include <QTextEdit>
#include <QWebFrame>
#include <QTextBrowser>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QWebElement>
#include <QClipboard>
#include <QApplication>
#include <QWebHitTestResult>

class HappyWebView::PrivData
{
public:
    PrivData()
    {
        isScoll = true;
        browser.hide();
    }
    ~PrivData()
    {

    }
public:
    bool isScoll;
    QMenu   menu;
    QTextBrowser browser;
};

HappyWebView::HappyWebView(QWidget *parent)
    : QWebView(parent)
{
    mData = new PrivData();
    QAction *action= page()->action(QWebPage::CopyImageUrlToClipboard);
    QAction *add = new QAction(cns("收藏"), this);
    connect(add, SIGNAL(triggered()), action, SIGNAL(triggered()));
    connect(action, SIGNAL(triggered()), this, SLOT(addFavorites()));
    mData->menu.addAction(add);
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(htmlLoadFinished(bool)));
}

HappyWebView::~HappyWebView()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void HappyWebView::clear()
{
    setHtml("");
    mData->browser.clear();
//    startTimer(10);
}

void HappyWebView::showContent()
{
//    QTime time;
//    time.start();
    QString html = mData->browser.toHtml();
    setHtml(page()->currentFrame()->toHtml() + html);
    mData->browser.clear();
//    QFont font("LiHei Pro");
//    font.setPixelSize(9);
//    mData->browser.setFont(font);
//    startTimer(10);
//    qDebug() << "showcontent time:" << time.elapsed();
}

void HappyWebView::insertHtml(QString html)
{
       mData->browser.insertHtml(html);
//    startTimer(1);
}

void HappyWebView::insertText(QString txt, Qt::GlobalColor textcolor)
{
//    mData->browser.setFont(QFont("LiHei Pro"));
    mData->browser.setTextColor(textcolor);
    mData->browser.setFontPointSize(9);
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Normal);
    fmt.setFontUnderline(false);
    fmt.setFontItalic(false);
    fmt.setFontFamily(cns("宋体"));
    mData->browser.mergeCurrentCharFormat(fmt);
//    mData->browser.setFont(QFont("LiHei Pro"));
    mData->browser.insertPlainText(txt);
//    QString html = mData->browser.toHtml();
//    insertHtml(html);
}

void HappyWebView::scollMax()
{
    int max = page()->currentFrame()->scrollBarMaximum(Qt::Vertical);
    page()->currentFrame()->setScrollBarValue(Qt::Vertical, max);
}

QString HappyWebView::toPlainText()
{
    return page()->currentFrame()->toPlainText().trimmed();
}

void HappyWebView::setScollEnable(bool b)
{
    mData->isScoll = b;
}

void HappyWebView::addFavorites()
{
    QString url = qApp->clipboard()->text();
    url = url.remove("file:///");
    QString path = "D:/Program Files/happy/emoji.data";
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.seek(file.size());
        url += ";";
        int ret = file.write(url.toLocal8Bit());
        if (ret == url.toLocal8Bit().length())
        {
//            insertText(cns("收藏成功\n"), Qt::lightGray);
            NotifyUI::ShowNotify(cns("收藏成功"));
            QString imagepath = url.left(url.length() - 1);
            emit signalFavoriteImagePath(imagepath);
        }
    }
    file.close();
    QString imagepath = url.left(url.length() - 1);
    emit MoveEmojiCustom::GetInstance()->signalStartMove(imagepath);
}

void HappyWebView::htmlLoadFinished(bool /*ok*/)
{
    if (mData->isScoll == false)
    {
        return;
    }
//    qDebug() << "load ok";
    startTimer(1);
}

void HappyWebView::timerEvent(QTimerEvent *e)
{
    scollMax();
    killTimer(e->timerId());
}

void HappyWebView::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        // 双击单独列出图片查看
        // 获取当前双击的资源文件
        QPoint p = e->pos();
        QString url = page()->currentFrame()->hitTestContent(p).imageUrl().toString();
        if (url.isEmpty())
        {
            return;
        }
        url = url.remove("file:///");
        EmojiLabel *label = new EmojiLabel();
        label->setWindowTitle(cns("图片查看"));
        QPixmap pix(url);
//        label->setPixmap(pix);
        label->setMoiveRes(url);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
//        label->resize(pix.size());
        label->setAttribute(Qt::WA_DeleteOnClose);
        label->setWindowFlags(Qt::Tool);
        label->show();
        movecenter(label);
        label->setWindowModality(Qt::ApplicationModal);
        label->activateWindow();
    }
    QWebView::mouseDoubleClickEvent(e);
}

void HappyWebView::contextMenuEvent(QContextMenuEvent *e)
{
    QString url = page()->currentFrame()->hitTestContent(e->pos()).imageUrl().toString();
    if (url.isEmpty())
    {
        return;
    }
    mData->menu.exec(QCursor::pos());
    e->accept();
}
