#ifndef FRIENDINFO_H
#define FRIENDINFO_H

#include <QWidget>

class EmojiLabel;
class QLabel;
class BroadcastJsonData;
class GsoapCallOperation;
class BroadcastReceverJson;
class FriendInfo : public QWidget
{
    Q_OBJECT
public:
    explicit FriendInfo(QWidget *parent = 0);
    ~FriendInfo();
    void setJsondata(QString uid);
    void updateMemoName();
    void setRecever(BroadcastReceverJson *recever);
signals:

private:
    EmojiLabel *photoLbl;
    QLabel *nameLbl;
    QLabel *editLbl;
    QLabel *versionLbl;
    QLabel *extraInfoLbl;
    QLabel *sayLbl;
    BroadcastJsonData *data;
    GsoapCallOperation *sender;
};

#endif // FRIENDINFO_H
