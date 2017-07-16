#ifndef FILEUI_H
#define FILEUI_H

#include <QWidget>

class QLabel;
class QPushButton;
class QListWidget;
class QListWidgetItem;
class FileUI : public QWidget
{
    Q_OBJECT
public:
    FileUI(QWidget *parent = 0);
    virtual~FileUI();
    void setKey(QString key);
    void setSavePath(QString path);
signals:
    void signalRecv();
    void signalAskSend();
    void signalStartSend();
    void signalRecvTotalProgress(int value);
    void signalRecvCurrentProgress(int value);
    void signalSendTotalProgress(int value);
    void signalSendCurrentProgress(int value);
public slots:
    void insertFile(QString file);
    void insertPath(QString path);
private slots:
    void stopRecv();
    void startSend();
    void askSend();
    void stopSend();
    void clearList();
    void openPath();
    void itemDoubleClicked(QListWidgetItem *);
    void recvInfo(QString type, QString info);
    void sendInfo(QString type, QString info);

    void insertRecvFile(QStringList lst);
    void recvstart();
    void recvsuccess();
    void sendstart();
    void sendsuccess();

    void sendSpeed(float mbs);
    void recvSpeed(float mbs);
private:
    QLabel *mRecvMsg;
    QLabel *mSendMsg;
    QPushButton *mClear;
    QListWidget *mList;
    QListWidget *mRecvList;

    QLabel *mRecvLabel;
    QLabel *mSendLabel;

    class PrivData;
    PrivData *mData;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // FILEUI_H
