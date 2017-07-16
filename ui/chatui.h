#ifndef CHATUI_H
#define CHATUI_H

#include "thwidgetbase.h"
#include "../data/messagedata.h"
#include <QAbstractSocket>
#include <QMainWindow>
#include <QTimer>
#include <qmenu.h>
#include <QPushButton>
#include <QTextCharFormat>

class FileUI;
class EmojiTable;
class HappyWebView;
class QTextBrowser;
class HappyTextEdit;
class ClientOpt;
class QComboBox;
class QFontComboBox;
class ClientThread;
class QSystemTrayIcon;
class GsoapCallOperation;
class DialogOptionsWidget;
class ChatUI : public QMainWindow/*THWidgetBase*/
{
    Q_OBJECT
public:
    ChatUI(QWidget *parent = 0);
    virtual~ChatUI();
    void setMyKey(QString key);
    virtual void setKey(const QString &key);
    QString participantName();
    void setGsoapOpt(GsoapCallOperation *gsoap);
    virtual void setParticipantName(const QString &name);
    virtual bool isEmpty();
signals:
    void closeConnection();
    void recvMessage(const QString &key);
    void closeUI();
    void removeUI(const QString &key);
public slots:
    void sendMsgFail(QString key, QString error);
    virtual void createEmojiTable();
    virtual void emojiSelected(QString path, QString key);
    virtual void sendclicked();
    virtual void returnPressed();
    virtual void showMessage(const QString &msg, Qt::GlobalColor textcolor = Qt::black);
    virtual void showMessageHtml(const QString &html);
    virtual void showRecvMessage(const QString &html);

    virtual void getRichMessage(MessageData data);

    void sendShake();
    void ShakeOpt();
    void tranforUI();
    void shakeSwitch();
    void inserFiles();

    void askSend();

    void replaySend(QString content);

    void startRecv(QString content);

    void shakeUI();

    void setMsgColor();
    void setMsgFont();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void textChange();
protected:
//    void insertHtml(QString html);
//    void insertText(QString txt, Qt::GlobalColor textcolor = Qt::black);
//    QString toPlainText();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
protected:
    virtual void init();
    QList<QIcon> mIconLst;

    QString mName;
    QString mKey;
    QString mMyKey;

    HappyWebView *mShowMsg;
    HappyTextEdit *mEditMsg;

    EmojiTable *mEmojiTable;
    
    QPushButton *m_emojiPb;

    QPushButton *mShake;

    QPushButton *mShakeSwitch;

    int mShakeTimerId;
    int mShakeTime;

    QMenu *m_emojiMune;

    // Gsoap
    GsoapCallOperation *mGsoap;

    // 文件发送功能
    FileUI *mFileUI;

    QDockWidget *dockRight;

    QString mColor;

    QAction *actionTextBold;
    QAction *actionTextUnderline;
    QAction *actionTextItalic;
    QAction *actionTextColor;

    QFontComboBox *comboFont;
    QComboBox *comboSize;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // CHATUI_H
