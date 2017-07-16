#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include <QWidget>
#include <QJsonObject>

#define ItemMinHeight 25
#define ItemMaxHeight 60
class Chat;
class EmojiLabel;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class PersonChat;
class BroadcastJsonData;
class QTreeWidgetItem;
class BroadcastRecever;
class BroadcastReceverJson;
class ItemEditor : public QWidget
{
    Q_OBJECT
public:
    ItemEditor(QWidget *parent = 0);
    explicit ItemEditor(BroadcastRecever *recever,
                        QTreeWidgetItem *it, QWidget *parent = 0);
    ~ItemEditor();
    virtual Chat *createChat() = 0;
    virtual bool isChatVisible() = 0;
    virtual ItemEditor *clone() = 0;
    virtual void showChat() = 0;
    virtual bool setChat(Chat *chat) = 0;
    virtual void addTimeSign();
    virtual void setUid(QString id);
    virtual void setName(QString name);
    QString getName();
    QString getUid();
    enum ItemType{selftBody, realityBody, cloneBody};
    void setItemTyle(ItemType type);
signals:
    void sendNotify(QString photo);

public slots:
    void starNotify();
    void stopNotify();
    void showMaxMin(bool min = false);

protected:
    EmojiLabel *emoji;
    QLabel *name;
    QLabel *introduce;
    QLabel *other;

    QHBoxLayout *h1;
    QHBoxLayout *h2;
    QVBoxLayout *v1;

    QString uid;
    QString myuid;
    QTreeWidgetItem *item;

    int timerid;

    ItemType itemType;

    int emojiSize;

private:
    void init();

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *e);
};

#endif // ITEMEDITOR_H
