#include "groupchatui.h"
#include "mainui.h"
#include "memberui.h"
#include "../thwidgets/happytextedit.h"
#include "../thwidgets/happywebview.h"
#include "../thwidgets/emoji/emojitable.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include "../broadcast/broadcastopt.h"
#include "../data/messagerichtext.h"
#include "../gsoap/call/gsoapcalloperation.h"
#include "emojiinstance.h"
#include <QtWidgets>

GroupChatUI::GroupChatUI(MainUI *parent) /*: QWidget(parent)*/
{
    init();
    mMain = parent;
    mTimerId = startTimer(1000 * 60 * 1);
    mGsoap = nullptr;
}

GroupChatUI::~GroupChatUI()
{
    killTimer(mTimerId);
//    if (nullptr != mEmoji)
//    {
//        mEmoji->deleteLater();
//    }
}

void GroupChatUI::setGsoapOpt(GsoapCallOperation *gsoap)
{
    mGsoap = gsoap;
    connect(mGsoap, SIGNAL(signalGroupSendResult(QString,QString)),
            this, SLOT(groupSendResult(QString,QString)));
}

void GroupChatUI::setTitle(QString name)
{
    mTitle = name;
    setWindowTitle(cns("��") + name + cns("��̸��"));
}

void GroupChatUI::setLocalIp(QString ip)
{
    mLocalIp = ip;
}

void GroupChatUI::setKey(QString key)
{
    mKey = key;
}

void GroupChatUI::setFilter(QStringList lst)
{
    mUserList = lst;
}

QString GroupChatUI::getKey()
{
    return mKey;
}

QStringList GroupChatUI::userList()
{
    return mUserList;
}

bool GroupChatUI::isEmpty()
{
    QString content = mWeb->toPlainText().trimmed();
    int size = content.size();
    bool flag = true;
    if (size > 0)
    {
        flag = false;
    }
    return content.isEmpty() || flag;
}

void GroupChatUI::ignoreClicked(bool b)
{
//    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    if (b)
    {
        mNo->setText(cns("�ر�����"));
    }
    else
    {
        mNo->setText(cns("��������"));
    }
}

void GroupChatUI::recvHtml(QString ip, QString html)
{
    // ��ȡ�û���
//    QString user = ip.split(":").first();
//    refreshList(mMain->returnUser());
//    for (QString name : mMain->returnUser())
//    {
//        if (name.contains(user))
//        {
//            user = name;
//            break;
//        }
//    }
//    Qt::GlobalColor color = Qt::darkBlue;
//    if (mLocalIp == ip)
//    {
//        color = Qt::darkGreen;
//        user = cns("��");
//    }
//    showmsg(html, user, color);
//    if (mNo->isChecked())
//    {
//        return;
//    }
//    qApp->alert(this, 0);
//    if (this->isHidden() || (!isVisible()))
//    {
//        mMain->startNotify();
//    }
}

void GroupChatUI::sendMsg()
{
    QString txt = mEdit->toPlainText().trimmed();
    if (txt.isEmpty())
    {
        mEdit->setPlaceholderText(cns("����Ϊ��"));
        return;
    }
    QString html = mEdit->toHtml();
    showmsg(html, cns("��"));
    MessageData msg;
    MessageRichText *content = new MessageRichText();
    content->setMessageHtml(html);
    msg.setDatatype(content->datatype());
    msg.setContent(content);
//    QStringList lst = mMain->returnUser();
//    refreshList(lst);
////    lst << mLocalIp;
//    if (mKey == "all")
//    {
//        emit mGsoap->signalChatGroupInerface(mMain->returnKeys(), mKey, msg);
//    }
//    else
//    {
//        emit mGsoap->signalChatGroupInerface(mUserList, mKey, msg);
//    }
//    mEdit->setPlaceholderText(cns("�س���ֱ�ӷ���"));
}

void GroupChatUI::emojiTableClicked()
{
    EmojiInstance *instance = EmojiInstance::GetInstance();
    connect(instance, SIGNAL(signalEmojiUrl(QString,QString)),
            this, SLOT(emojiSelect(QString,QString)));
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    instance->showEmoji(pb->mapToGlobal(pb->rect().topLeft()), mKey);
//    mEmoji->show();
//    mEmoji->activateWindow();
//    mEmoji->setWindowModality(Qt::WindowModal);
}

void GroupChatUI::emojiSelect(QString path, QString key)
{
    if (key != mKey)
    {
        return;
    }
    mEdit->insertHtml(ImagePathToFileHtml(path));
    mEdit->setFocus();
    EmojiInstance *instance = EmojiInstance::GetInstance();
    disconnect(instance, SIGNAL(signalEmojiUrl(QString,QString)),
               this, SLOT(emojiSelect(QString,QString)));
//    mEmoji->hide();
}

void GroupChatUI::showmsg(QString html, QString name, Qt::GlobalColor color)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mWeb->insertText(datetime + " " + name + "\n", color);

    mWeb->insertHtml(html);

    mWeb->insertText("\n\n");

    mWeb->showContent();
}

void GroupChatUI::refreshList(QStringList list)
{
    mList->clear();

    QStringList templst;
    // ���˵����Ǳ�Ⱥ���ˣ���keyΪallʱ���ù���
    if (mKey.compare("all", Qt::CaseInsensitive) != 0)
    {
        for (QString str : mUserList)
        {
            if (str.isEmpty() == false)
            {
                for (QString user : list)
                {
                    if (user.contains(str.split(":").first()))
                    {
                        templst << user;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        mUserList = list;
        templst << list;
    }

    mList->addItems(templst);
}

void GroupChatUI::inviteClicked()
{
//    if (mKey == "all")
    {
        QMessageBox::information(this, cns("��ʾ"), cns("��ʱ�ر��˸����빦��"), cns("��"));
        return;
    }
    MemberUI *ui = new MemberUI(this);
    ui->setBtVisible(true);
//    ui->setMembers(mMain->returnUserKeys());
    ui->setWindowTitle(cns("ѡ����Ļ���/����"));
    ui->setAttribute(Qt::WA_DeleteOnClose);
    ui->setWindowFlags(Qt::Tool);
    ui->setWindowModality(Qt::ApplicationModal);
    connect(ui, SIGNAL(choices(QStringList)), this, SLOT(sendInvitation(QStringList)));
    ui->show();
    ui->activateWindow();
}

void GroupChatUI::sendInvitation(QStringList keys)
{
    if (nullptr == mGsoap)
    {
        return;
    }
    QString name;
//    ConfigureData::getInstance()->getIni("name", name);
    QString ip = mLocalIp;

    QString temp;
    temp.append(ip);
    for (QString user : mUserList)
    {
        if (temp.isEmpty() == false)
        {
            temp.append(";");
        }
        temp.append(user);
        showmsg(cns("��ʼ��") + user + cns("��������"), cns("��ơ��ʾ"),
                Qt::green);
    }
    emit mGsoap->signalInviteGroup(keys, name, mTitle, mKey, temp);
}

void GroupChatUI::groupSendResult(QString key, QString error)
{
    if (key != mKey)
    {
        return;
    }
    showmsg(error, cns("��ơ��ʾ"), Qt::red);
}

void GroupChatUI::itemDoubleClicked(QListWidgetItem *item)
{
    QString name = item->text();
}

void GroupChatUI::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_bold", actionTextBold->isChecked());
}

void GroupChatUI::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_line", actionTextUnderline->isChecked());
}

void GroupChatUI::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_ltalic", actionTextItalic->isChecked());
}

void GroupChatUI::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
//    ConfigureData::getInstance()->writeIni("chat_font", f);
}

void GroupChatUI::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
//        ConfigureData::getInstance()->writeIni("chat_fontsize", p.toInt());
    }
}

void GroupChatUI::textColor()
{
    QColor col = QColorDialog::getColor(mEdit->textColor(), this);
    if (!col.isValid())
        return;
    colorChanged(col);
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    int r = col.red();
    int g = col.green();
    int b = col.blue();
    QString fontcolor = QString("%1,%2,%3").arg(r).arg(g).arg(b);
//    ConfigureData::getInstance()->writeIni("chat_color", fontcolor);
}

void GroupChatUI::currentCharFormatChanged(const QTextCharFormat &/*format*/)
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
    mEdit->setTextColor(color);
//    QTextCharFormat fmt;
//    fmt.setForeground(color);
//    mergeFormatOnWordOrSelection(fmt);
}

void GroupChatUI::textChange()
{

}

void GroupChatUI::scollEnable(bool b)
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    if (b)
    {
        pb->setText(cns("���ù���"));
    }
    else
    {
        pb->setText(cns("��ֹ����"));
    }
    mWeb->setScollEnable(!b);
}

void GroupChatUI::init()
{
    // ����Ⱥ����
    QVBoxLayout *vlayout = new QVBoxLayout();

    QHBoxLayout *h1 = new QHBoxLayout();

    mWeb = new HappyWebView(this);
    mList = new QListWidget(this);
    connect(mList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemDoubleClicked(QListWidgetItem*)));
    h1->addWidget(mWeb, 8);
    h1->addWidget(mList, 2);

    QToolBar *bar = new QToolBar(this);
    actionTextBold = new QAction(cns("��"), this);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    bar->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(cns("б"), this);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    bar->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(cns("��"), this);
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
    actionTextColor = new QAction(pix, cns("��ɫ"), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    bar->addAction(actionTextColor);

    comboFont = new QFontComboBox(this);
    connect(comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    comboSize = new QComboBox(this);
    comboSize->setObjectName("comboSize");
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
                                                                   .pointSize())));

    QHBoxLayout *h2 = new QHBoxLayout();
    QPushButton *emojiPb = new QPushButton(cns("ѡ�����"), this);
    connect(emojiPb, SIGNAL(clicked()), this, SLOT(emojiTableClicked()));
    mNo = new QPushButton(cns("��������"), this);
    mNo->setCheckable(true);
    mNo->setChecked(false);
    connect(mNo, SIGNAL(clicked(bool)), this, SLOT(ignoreClicked(bool)));
    QPushButton *scoll = new QPushButton(cns("��ֹ����"), this);
    scoll->setCheckable(true);
    scoll->setChecked(false);
    connect(scoll, SIGNAL(clicked(bool)), this, SLOT(scollEnable(bool)));

    mInvite = new QPushButton(cns("����"), this);
    connect(mInvite, SIGNAL(clicked()), this, SLOT(inviteClicked()));
    h2->addWidget(emojiPb, 0, Qt::AlignCenter);
    h2->addWidget(comboFont, 0, Qt::AlignCenter);
    h2->addWidget(comboSize, 0, Qt::AlignCenter);
    h2->addWidget(bar, 0, Qt::AlignCenter);
    h2->addWidget(mNo, 0, Qt::AlignCenter);
    h2->addWidget(scoll, 0, Qt::AlignCenter);
    h2->addWidget(mInvite, 0, Qt::AlignCenter);

    mEdit = new HappyTextEdit(this);
    mEdit->setPlaceholderText(cns("�س���ֱ�ӷ���"));
    connect(mEdit, SIGNAL(returnPress()), this, SLOT(sendMsg()));

    vlayout->addLayout(h1, 6);
    vlayout->addLayout(h2, 0);
    vlayout->addWidget(mEdit, 4);
    mWeb->clear();
    connect(mWeb, SIGNAL(signalFavoriteImagePath(QString)),
            EmojiInstance::GetInstance(), SIGNAL(updateEmoji(QString)));
    setLayout(vlayout);

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

    connect(mEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    textFamily(font);

    connect(mEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));

//    connect(mEdit, SIGNAL(textChanged()), this, SLOT(textChange()));


    int w = 420;
    int h = 700;
//    ConfigureData::getInstance()->getIni("chat_w", w);
//    ConfigureData::getInstance()->getIni("chat_h", h);
    resize(w, h);

    comboFont->setStyleSheet("QComboBox {color: white;}");
    comboSize->setStyleSheet("QComboBox {color: white;}");
//    bar->setStyleSheet("QToolBar {background: transparent;}");
    mList->setStyleSheet("QListView {color: white;}");

//    mEmoji = new EmojiTable();
//    mEmoji->setWindowFlags(Qt::Tool);
//    mEmoji->move(emojiPb->pos());
//    mEmoji->hide();
//    connect(mEmoji, SIGNAL(currentEmojiPath(QString)), this, SLOT(emojiSelect(QString)));
}



void GroupChatUI::closeEvent(QCloseEvent *e)
{
    mWeb->clear();
    hide();
//    ConfigureData::getInstance()->writeIni("chat_bold", actionTextBold->isChecked());
//    ConfigureData::getInstance()->writeIni("chat_line", actionTextUnderline->isChecked());
//    ConfigureData::getInstance()->writeIni("chat_ltalic", actionTextItalic->isChecked());
    e->ignore();
}


void GroupChatUI::timerEvent(QTimerEvent *)
{
//    refreshList(mMain->returnUser());
}


void GroupChatUI::showEvent(QShowEvent *)
{
//    refreshList(mMain->returnUser());
}

void GroupChatUI::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = mEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mEdit->mergeCurrentCharFormat(format);
}

void GroupChatUI::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void GroupChatUI::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}
