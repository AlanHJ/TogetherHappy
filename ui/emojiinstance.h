#ifndef EMOJIINSTANCE_H
#define EMOJIINSTANCE_H

#include <QObject>

class EmojiInstance : public QObject
{
    Q_OBJECT
public:
    explicit EmojiInstance(QObject *parent = 0);
    ~EmojiInstance();
    static EmojiInstance *GetInstance();
    static void DesInstance();
    void showEmoji(QPoint point, QString key);
    void reloadEmoji();
    void setStyleSheet(QString css);
signals:
    void updateEmoji(QString url);
    void signalEmojiUrl(QString url, QString key);
private slots:
    void selectedEmoji(QString url);
    void enterEmoji(QString path);
    void leaveEmoji();
private:
    class PrivData;
    PrivData *mData;

};

#endif // EMOJIINSTANCE_H
