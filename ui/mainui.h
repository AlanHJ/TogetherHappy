#ifndef MAINUI_H
#define MAINUI_H

#include "thwidgetbase.h"
#include "../tcp/client.h"
#include <QSystemTrayIcon>
#include <QTimer>
#include <QAbstractNativeEventFilter>

class QPushButton;
class QTreeWidgetItem;
class QSystemTrayIcon;
class ScreenshotWidget;
class QTreeWidget;
class QVBoxLayout;
class QHBoxLayout;
class GroupChatUI;
class ClientOpt;
class QLabel;
class ChatUI;
class HeaderPhotoUI;
class GsoapThread;
class GsoapCallOperation;
class MainUI : public THWidgetBase
{
    Q_OBJECT

public:
    MainUI(THWidgetBase *parent = 0);
    virtual~MainUI();
public slots:
    // 关于
    void abouthappy();

private slots:
    void hotkeyclicked();

    void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);

    // 截图
    void screenshot();

    // 更新
    void updateHappy();
    void updateInfo(QString info);

    void updateUpdate();

    // 清理缓存文件
    void clearTemp();
private:
    void init();
    void dafaultname();
    void autoColor();
private:

    // GSOAP
    GsoapThread *mGsoap;
    GsoapCallOperation *gsoapOpt;

    QSystemTrayIcon *mTray;

    // new ui
    HeaderPhotoUI *photo;

    QPushButton *about;

signals:
    void updateUpdateInfo(QString er);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void hideEvent(QHideEvent *);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINUI_H
