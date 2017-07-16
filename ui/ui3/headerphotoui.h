#ifndef HEADERPHOTOUI_H
#define HEADERPHOTOUI_H

#include <QWidget>

class EditLabel;
class EmojiLabel;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QMenu;
class SelfMenu;

class HeaderPhotoUI : public QWidget
{
    Q_OBJECT
public:
    explicit HeaderPhotoUI(QWidget *parent = 0);
    ~HeaderPhotoUI();
    void setPhoto(QString res);
    void updateResource();
signals:
    void selfStatusChanged(int status);
    void selfFeelChanged(QString str);
private:
    EmojiLabel *photo;
    QLabel *name;
    EditLabel *saysay;
    QLabel *level;
    QPushButton *status;

    int id;
    int statusid;
    bool tcpEnabled;
    // create
    void init();

private slots:
    void sendBroadcast();
    void sendClose();
    // QObject interface
protected:
    void timerEvent(QTimerEvent *e);
};

#endif // HEADERPHOTOUI_H
