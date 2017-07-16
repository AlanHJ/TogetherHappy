#ifndef GROUPCHATUI_H
#define GROUPCHATUI_H

#include <QMainWindow>
#include <QTextCharFormat>

class MainUI;
class QPushButton;
class BroadcastOpt;
class EmojiTable;
class QListWidget;
class HappyWebView;
class HappyTextEdit;
class QListWidgetItem;
class GsoapCallOperation;
class QComboBox;
class QFontComboBox;
class GroupChatUI : public QWidget
{
    Q_OBJECT
public:
    GroupChatUI(MainUI *parent = 0);
    virtual~GroupChatUI();
    void setGsoapOpt(GsoapCallOperation *gsoap);
    void setTitle(QString name);
    void setLocalIp(QString ip);
    void setKey(QString key);
    void setFilter(QStringList lst);
    QString getKey();
    QStringList userList();
    bool isEmpty();
public slots:
    void ignoreClicked(bool b);
    void recvHtml(QString ip, QString html);
    void sendMsg();
    void emojiTableClicked();
    void emojiSelect(QString path, QString key);

    void showmsg(QString html, QString name, Qt::GlobalColor color = Qt::darkGreen);

    void refreshList(QStringList list);

    void inviteClicked();

    void sendInvitation(QStringList keys);

    void groupSendResult(QString key, QString error);

private slots:
    void itemDoubleClicked(QListWidgetItem * item);

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void textChange();
    void scollEnable(bool b);
signals:
    void happyDoubleClicked(QString name, QString key);
private:
    void init();

    HappyTextEdit *mEdit;
    HappyWebView *mWeb;
    QListWidget *mList;
//    EmojiTable *mEmoji;
    QPushButton *mNo;
    QPushButton *mInvite;

    MainUI *mMain;
    QStringList mUserList;
    QString mTitle;
    QString mLocalIp;
    QString mKey;

    int mTimerId;

    // Gsoap
    GsoapCallOperation *mGsoap;

    QAction *actionTextBold;
    QAction *actionTextUnderline;
    QAction *actionTextItalic;
    QAction *actionTextColor;

    QFontComboBox *comboFont;
    QComboBox *comboSize;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void timerEvent(QTimerEvent *);
    virtual void showEvent(QShowEvent *);

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
};

#endif // GROUPCHATUI_H
