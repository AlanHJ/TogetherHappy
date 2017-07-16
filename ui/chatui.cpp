#include "chatui.h"
#include "../broadcast/existbroadcast.h"
#include "../util/thmethod.h"
#include "../tcp/client.h"
#include "../tcp/clientthread.h"
#include "../tcp/clientopt.h"
#include "../tcp/connectionopt.h"
#include "../thwidgets/happytextedit.h"
#include "../thwidgets/happywebview.h"
#include "../thwidgets/emoji/emojitable.h"
#include "../data/messagerichtext.h"
#include "../config/configuredata.h"
#include "../gsoap/call/gsoapcalloperation.h"
#include "fileui.h"
#include "emojiinstance.h"
#include <QDateTime>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextEdit>
#include <QCloseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QTextCursor>
#include <QSystemTrayIcon>
#include <QWidgetAction>
#include <qmenu.h>
#include <QDockWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QToolBar>
#include <QFontComboBox>
#include <QComboBox>
#include <QRgb>

ChatUI::ChatUI(QWidget *parent) : QMainWindow(parent)
{
    m_emojiPb = nullptr;
    mEmojiTable = nullptr;
    
    init();
    
    setAttribute(Qt::WA_DeleteOnClose);
    
    connect(this, SIGNAL(closeUI()), this, SLOT(deleteLater()));
}

ChatUI::~ChatUI()
{

}

void ChatUI::setMyKey(QString key)
{

}

void ChatUI::setKey(const QString &key)
{
    this->mKey = key;
    mFileUI->setKey(key.split(":").first() + ":" + QString::number(HAPPY_LISTEN_PORT));
}

QString ChatUI::participantName()
{
    return mName;
}

void ChatUI::setGsoapOpt(GsoapCallOperation *gsoap)
{
    mGsoap = gsoap;
    connect(mGsoap, SIGNAL(signalChatMsgSendFail(QString,QString)),
            this, SLOT(sendMsgFail(QString,QString)));
}

void ChatUI::setParticipantName(const QString &name)
{
    mName = name;
    setWindowTitle(cns("和") + mName + cns("交谈中"));
}

bool ChatUI::isEmpty()
{
    QString content = mShowMsg->toPlainText().trimmed();
    int size = content.size();
    bool flag = true;
    if (size > 0)
    {
        flag = false;
    }
    return content.isEmpty() || flag;
}

void ChatUI::sendMsgFail(QString key, QString error)
{
    if (key != mKey)
    {
        return;
    }
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mShowMsg->insertText(datetime + "\n", Qt::red);

    mShowMsg->insertText(error, Qt::red);

    mShowMsg->insertText("\n");

    mShowMsg->showContent();
}

void ChatUI::createEmojiTable()
{
    EmojiInstance *instance = EmojiInstance::GetInstance();
    connect(instance, SIGNAL(signalEmojiUrl(QString,QString)),
            this, SLOT(emojiSelected(QString,QString)));
    instance->showEmoji(m_emojiPb->mapToGlobal(m_emojiPb->rect().topLeft()), mKey);


//    QRect rect = this->rect();

//    QDesktopWidget *desktop = QApplication::desktop();
//    int screenHeight = desktop->height();
//    if ( (screenHeight - this->mapToGlobal(rect.topRight()).y()) < myMenu->height() )
//    {
//        QPoint p = QPoint(this->mapToGlobal(rect.topRight()).x(), screenHeight - myMenu->height());
//        myMenu->move(p);

//    }
//    else
//    {
//        myMenu->move(this->mapToGlobal(rect.topRight()));
//    }
    //

//    QRect rect = m_emojiPb->rect();
//    m_emojiMune->move(m_emojiPb->mapToGlobal(rect.topLeft()));
    
//    m_emojiMune->exec();
}

void ChatUI::emojiSelected(QString path, QString key)
{
    if (key != mKey)
    {
        return;
    }
    mEditMsg->insertHtml(ImagePathToFileHtml(path));
    mEditMsg->setFocus();

    EmojiInstance *instance = EmojiInstance::GetInstance();
    disconnect(instance, SIGNAL(signalEmojiUrl(QString,QString)),
               this, SLOT(emojiSelected(QString,QString)));
}

void ChatUI::sendclicked()
{
    QString txt = mEditMsg->toPlainText();
    QString html = mEditMsg->toHtml();
    if (txt.isEmpty())
    {
        mEditMsg->setPlaceholderText(cns("不能为空"));
        return;
    }
    showMessageHtml(html);

    MessageData msg;
    MessageRichText *content = new MessageRichText();
    content->setMessageHtml(html);
    msg.setDatatype(content->datatype());
    msg.setContent(content);
    emit mGsoap->signalChatOneInterface("", mKey, msg);
    mEditMsg->setPlaceholderText(cns("回车键直接发送"));
}

void ChatUI::returnPressed()
{
    sendclicked();
}

void ChatUI::init()
{
    // 创建图标
    mIconLst << QIcon(":/res/happy.png")
             << QIcon(":/res/notify.png");
    mShowMsg = new HappyWebView(this);
    mEditMsg = new HappyTextEdit(this);
    mEditMsg->setPlaceholderText(cns("回车键直接发送"));
    connect(mEditMsg, SIGNAL(returnPress()),
            this, SLOT(returnPressed()));
    QPushButton *closepb = new QPushButton(cns("关闭"), this);
    connect(closepb, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *sendpb = new QPushButton(cns("发送"), this);
    connect(sendpb, SIGNAL(clicked()), this, SLOT(sendclicked()));
    QHBoxLayout *hlayoutBottom = new QHBoxLayout();
    hlayoutBottom->setSpacing(10);
    hlayoutBottom->setContentsMargins(3,3,3,3);
    hlayoutBottom->addWidget(closepb, 0, Qt::AlignCenter);
    hlayoutBottom->addWidget(sendpb, 0, Qt::AlignCenter);
    
    //CentralWidget   ===================================================
    QVBoxLayout *vlayCentral = new QVBoxLayout();
    vlayCentral->addWidget(mShowMsg, 6);
    vlayCentral->setSpacing(0);
    vlayCentral->setContentsMargins(3,3,3,3);

    QFrame *frame = new QFrame(this);
    QToolBar *bar = new QToolBar(frame);
    actionTextBold = new QAction(cns("粗"), frame);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    bar->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(cns("斜"), frame);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    bar->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(cns("线"), frame);
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    bar->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, cns("颜色"), frame);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    bar->addAction(actionTextColor);

    comboFont = new QFontComboBox(frame);
    connect(comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    comboSize = new QComboBox(frame);
    comboSize->setObjectName("comboSize");
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
                                                                   .pointSize())));

//    QPushButton *fontButton = new QPushButton(cns("字体"), frame);
//    connect(fontButton, SIGNAL(clicked()), this, SLOT(setMsgFont()));
//    QPushButton *colorButton = new QPushButton(cns("颜色"), frame);
//    connect(colorButton, SIGNAL(clicked()), this, SLOT(setMsgColor()));
//    QHBoxLayout *msgRichHbox = new QHBoxLayout();
//    msgRichHbox->addWidget(fontButton, 0, Qt::AlignCenter);
//    msgRichHbox->addWidget(colorButton, 0, Qt::AlignCenter);

    m_emojiPb = new QPushButton(cns("选择表情"), frame);
    connect(m_emojiPb, SIGNAL(clicked()), this, SLOT(createEmojiTable()));
    mShake = new QPushButton(cns("抖一抖"), frame);
    connect(mShake, SIGNAL(clicked()), this, SLOT(sendShake()));
    mShakeSwitch = new QPushButton(cns("抖屏蔽"), frame);
    connect(mShakeSwitch, SIGNAL(clicked()), this, SLOT(shakeSwitch()));
    mShakeSwitch->setCheckable(true);
    mShakeSwitch->setChecked(false);

    QPushButton *transfor = new QPushButton(cns("文件传输（显示/隐藏）"), frame);
    connect(transfor, SIGNAL(clicked()), this, SLOT(tranforUI()));

    
    QHBoxLayout *toolh = new QHBoxLayout();
    toolh->setContentsMargins(0,3,0,3);
    toolh->addStretch(1);
    toolh->addWidget(m_emojiPb, 1);
    toolh->addStretch(1);
    toolh->addWidget(comboFont, 0);

    toolh->addWidget(comboSize, 0);

    toolh->addWidget(bar);
    toolh->addStretch(1);
    toolh->addWidget(mShakeSwitch, 1);
    toolh->addWidget(mShake, 1);
    toolh->addStretch(1);
    toolh->addWidget(transfor, 1);
    toolh->addStretch(1);
    frame->setLayout(toolh);
    bar->resize(30, 20);

    QFrame *centralWidget = new QFrame(this);
    centralWidget->setLayout(vlayCentral);
    this->setCentralWidget(centralWidget);
        

    //BottomDockWidget ===================================================
    QVBoxLayout *bottomDockLayout = new QVBoxLayout();
    bottomDockLayout->setSpacing(0);
    bottomDockLayout->setContentsMargins(3,0,3,0);
    bottomDockLayout->addWidget(frame);
    frame->setFixedHeight(30);
    bottomDockLayout->addWidget(mEditMsg, 3);
    bottomDockLayout->addLayout(hlayoutBottom, 0);
    
    QFrame *bottomDockWidget = new QFrame(this);
    bottomDockWidget->setLayout(bottomDockLayout);

    
    QDockWidget *dock = new QDockWidget(this);
    dock->setFeatures(QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(bottomDockWidget);
//    QWidget *barw = dock->titleBarWidget();
//    if(nullptr != barw)
//        barw->setFixedHeight(5);

    mFileUI = new FileUI(this);
    connect(mEditMsg, SIGNAL(signalGetFile(QString)),
            mFileUI, SLOT(insertFile(QString)));
    connect(mEditMsg, SIGNAL(signalGetPath(QString)),
            mFileUI, SLOT(insertPath(QString)));
    connect(mFileUI, SIGNAL(signalAskSend()),
            this, SLOT(askSend()));
    connect(mFileUI, SIGNAL(signalRecv()),
            this, SLOT(inserFiles()));
    connect(mEditMsg, SIGNAL(signalInserFile()),
            this, SLOT(inserFiles()));
    dockRight = new QDockWidget(this);
    dockRight->setMaximumWidth(192);
//    dockRight->setMaximumHeight(372);
    dockRight->setFeatures(QDockWidget::DockWidgetFloatable);
    dockRight->setAllowedAreas(Qt::RightDockWidgetArea);
    dockRight->setWidget(mFileUI);
    
    this->addDockWidget(Qt::BottomDockWidgetArea,dock);
    this->addDockWidget(Qt::RightDockWidgetArea, dockRight);
//    mShowMsg->show();
    mShowMsg->insertHtml("");

    connect(mShowMsg, SIGNAL(signalFavoriteImagePath(QString)),
            EmojiInstance::GetInstance(), SIGNAL(updateEmoji(QString)));
    
    //=================== m_emojiMune =====================================
//    m_emojiMune = new QMenu(this);
//    QWidgetAction *action = new QWidgetAction(m_emojiMune);
//    EmojiTable *table = new EmojiTable(m_emojiMune);
//    table->show();
//    connect(table, SIGNAL(currentEmojiPath(QString)),
//            this, SLOT(emojiSelected(QString)));
    
//    action->setDefaultWidget(table);
//    m_emojiMune->addAction(action);

    QString font = "LiHei Pro";
    QString fontcolor = "0, 0, 0";
    bool isbold = false;
    bool isItalic = false;
    bool isLine = false;
    int fontsize = 9;
//    ConfigureData::getInstance()->getIni("chat_font", font);
//    ConfigureData::getInstance()->getIni("chat_color", fontcolor);
//    ConfigureData::getInstance()->getIni("chat_bold", isbold);
//    ConfigureData::getInstance()->getIni("chat_ltalic", isItalic);
//    ConfigureData::getInstance()->getIni("chat_line", isLine);
//    ConfigureData::getInstance()->getIni("chat_fontsize", fontsize);

    actionTextBold->setChecked(isbold);
    actionTextItalic->setChecked(isItalic);
    actionTextUnderline->setChecked(isLine);
    comboSize->setCurrentIndex(comboSize->findText(QString::number(fontsize)));
    comboFont->setCurrentIndex(comboFont->findText(font));
    int r = 0;
    int g = 0;
    int b = 0;
    QStringList lst = fontcolor.split(",");
    if (lst.size() > 2)
    {
        r = lst.first().toInt();
        g = lst.at(1).toInt();
        b = lst.last().toInt();
    }
    QColor color(r, g, b);
    colorChanged(color);

    connect(mEditMsg, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    textFamily(font);

    int w = 645;
    int h = 620;
//    ConfigureData::getInstance()->getIni("chat_w", w);
//    ConfigureData::getInstance()->getIni("chat_h", h);
    resize(w, h);

//    comboFont->setEditable(false);
//    comboSize->setEditable(false);
    comboFont->setStyleSheet("QComboBox {color: white;}");
    comboSize->setStyleSheet("QComboBox {color: white;}");
    bar->setStyleSheet("QMenu::item {color: white;}");

    dockRight->hide();
}

void ChatUI::showMessage(const QString &msg, Qt::GlobalColor textcolor)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mShowMsg->insertText(datetime + "\n" + msg + "\n\n", textcolor);
}

void ChatUI::showMessageHtml(const QString &html)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mShowMsg->insertText(datetime + "\n", Qt::darkGreen);

    mShowMsg->insertHtml(html);

    mShowMsg->insertText("\n\n");

    mShowMsg->showContent();
}

void ChatUI::showRecvMessage(const QString &html)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mShowMsg->insertText(datetime + " " + mName + "\n", Qt::darkBlue);
    mShowMsg->insertHtml(html);
    mShowMsg->insertText("\n\n");
    mShowMsg->showContent();
    emit recvMessage(mKey);
    qApp->alert(this, 0);
}

void ChatUI::getRichMessage(MessageData data)
{
    if (data.datatype() != RichText)
    {
        return;
    }
    MessageRichText *rich = dynamic_cast<MessageRichText *>(data.content());
    if (nullptr ==rich)
    {
        return;
    }
    showRecvMessage(rich->messageHtml());
}

void ChatUI::sendShake()
{
    // 发送抖一抖
//    emit mGsoap->signalChatShakeInterface(mKey);
    mShake->setEnabled(false);
    mShakeTime = 30;
    mShakeTimerId = startTimer(1000);
}

void ChatUI::ShakeOpt()
{
    // 抖动UI
    int x = pos().x();
    int y = pos().y();
    int algh = 5;
    QTime time;
    time.start();
    while (time.elapsed() < 1000)
    {
        move(x - algh, y);
        thread()->msleep(35);
        move(x, y - algh);
        thread()->msleep(35);
        move(x + algh, y);
        thread()->msleep(35);
        move(x, y + algh);
        thread()->msleep(35);
        showNormal();
        activateWindow();
    }
    move(x, y);
}

void ChatUI::tranforUI()
{
    if (dockRight->isHidden())
    {
        dockRight->show();
    }
    else
    {
        dockRight->hide();
    }
}

void ChatUI::shakeSwitch()
{
    if (mShakeSwitch->isChecked())
    {
        mShakeSwitch->setText(cns("抖开启"));
    }
    else
    {
        mShakeSwitch->setText(cns("抖屏蔽"));
    }
}

void ChatUI::inserFiles()
{
    dockRight->show();
}

void ChatUI::askSend()
{
    emit mGsoap->signalSendFileExec(mKey, cns("是否接收对方发来的文件？"));
}

void ChatUI::replaySend(QString content)
{
    this->show();
    this->activateWindow();
//    emit recvMessage(mKey);
//    qApp->alert(this, 0);

    int ret = QMessageBox::question(this, cns("收到文件提示"),
                          content, cns("吼啊"), cns("不吼"));
    if (ret == 0)
    {
        QString directory =
                QFileDialog::getExistingDirectory(this,cns("选择目录"), "D:/");
        QString temp = directory;
        temp = temp.right(1);
        if (temp != "/")
        {
            directory.append("/");
        }
        QFileInfo info(directory);
        QString tips;
        if (info.isDir() == false)
        {
            tips = cns("没有选择有效目录，将使用默认目录");
        }
        else
        {
            tips = cns("选择的目录为:") + directory;
            mFileUI->setSavePath(directory);
        }
        QMessageBox::about(this, cns("保存目录"), tips);
        emit mGsoap->signalSendFileAccept(mKey, "OK");
    }
    else
    {
        emit mGsoap->signalSendFileAccept(mKey, "NO");
    }
}

void ChatUI::startRecv(QString content)
{
    if (content.compare("OK", Qt::CaseInsensitive) == 0)
    {
        emit mFileUI->signalStartSend();
    }
    else if (content.compare("NO", Qt::CaseInsensitive) == 0)
    {
        sendMsgFail(mKey, cns("对方拒绝接收文件"));
    }
}

void ChatUI::shakeUI()
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mShowMsg->insertText(datetime + "\n", Qt::red);
    mShowMsg->insertText(cns("对方给了你一个抖一抖"), Qt::red);
    mShowMsg->insertText("\n");
    mShowMsg->showContent();

    if (mShakeSwitch->isChecked() == false)
    {
        this->activateWindow();
        this->show();
        this->showNormal();
        QTimer::singleShot(200, this, SLOT(ShakeOpt()));
    }

}

void ChatUI::setMsgColor()
{
    const QColor color = QColorDialog::getColor(mEditMsg->textColor(), this, cns("设置字体颜色"));
    if (color.isValid()) {
    }
}

void ChatUI::setMsgFont()
{
//    const QFontDialog::FontDialogOptions options = QFlag(fontDialogOptionsWidget->value());
    bool ok;
    QFont font = QFontDialog::getFont(&ok, mEditMsg->font(), this, cns("设置字体式样"));
    if (ok)
    {
        mEditMsg->setFont(font);
    }
}

void ChatUI::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_bold", actionTextBold->isChecked());
}

void ChatUI::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_line", actionTextUnderline->isChecked());
}

void ChatUI::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);

//    ConfigureData::getInstance()->writeIni("chat_ltalic", actionTextItalic->isChecked());
}

void ChatUI::textFamily(const QString &f)

{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_font", f);
}

void ChatUI::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
//        ConfigureData::getInstance()->writeIni("chat_fontsize", comboSize->currentText().toInt());
    }
}

void ChatUI::textColor()
{
    QColor col = QColorDialog::getColor(mEditMsg->textColor(), this);
    if (!col.isValid())
        return;
    colorChanged(col);
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    int r = col.red();
    int g = col.green();
    int b = col.blue();
    QString color = QString("%1,%2,%3").arg(r).arg(g).arg(b);
//    ConfigureData::getInstance()->writeIni("chat_color", color);
}

void ChatUI::currentCharFormatChanged(const QTextCharFormat &/*format*/)
{
    textBold();
    textItalic();
    textUnderline();
    textSize(comboSize->currentText());
    textFamily(comboFont->currentText());
    QPixmap pix = actionTextColor->icon().pixmap(QSize(16, 16));
    QRgb rgb = pix.toImage().pixel(8, 8);
    QColor color;
    color.setRgb(rgb);
    mEditMsg->setTextColor(color);
//    QTextCharFormat fmt;
//    fmt.setForeground(color);
//    mergeFormatOnWordOrSelection(fmt);
}

void ChatUI::textChange()
{

}

void ChatUI::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = mEditMsg->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mEditMsg->mergeCurrentCharFormat(format);
}

void ChatUI::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void ChatUI::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}


void ChatUI::resizeEvent(QResizeEvent *)
{
//    qDebug() << "w:" << width()
//             << " h:" << height();
}

void ChatUI::showEvent(QShowEvent *)
{
    mEditMsg->setFocus();
}


void ChatUI::closeEvent(QCloseEvent *e)
{
//    int w = width();
//    int h = height();
//    ConfigureData::getInstance()->writeIni("chat_w", w);
//    ConfigureData::getInstance()->writeIni("chat_h", h);



    emit closeUI();
    emit removeUI(mKey);
    hide();
    e->ignore();
}



void ChatUI::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == mShakeTimerId)
    {
        mShake->setText(cns("抖一抖:") + QString::number(mShakeTime));
        mShakeTime--;
        if (mShakeTime <= -1)
        {
            killTimer(e->timerId());
            mShake->setText(cns("抖一抖"));
            mShake->setEnabled(true);
        }
    }
}
