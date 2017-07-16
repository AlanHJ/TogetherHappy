#ifndef HOTKEYREGITER_H
#define HOTKEYREGITER_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class HotkeyRegiter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
private:
    HotkeyRegiter(QWidget *parent = 0);
    virtual~HotkeyRegiter();

    static HotkeyRegiter *regiter;
    int matchKey(QString key);
    QWidget *p;
signals:
    void hotKeyHide();
    void hotKeyShow();
    void hotKeyClose();
    void hotkeyLocked();
    void hotkeyClicked();
    void screenshotClicked();
    // QAbstractNativeEventFilter interface
public:
    static HotkeyRegiter *GetInstance(QWidget *parent = 0);
    static void Destruct();

    bool unRegister(int id);
    bool RegisterHotkey(int id, QString str);

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};

#endif // HOTKEYREGITER_H
