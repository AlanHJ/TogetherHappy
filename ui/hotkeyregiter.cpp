#include "hotkeyregiter.h"
#include <QWidget>
#include <Windows.h>
#include <QApplication>
#include "notifyui.h"
#include "../config/configuredata.h"

HotkeyRegiter *HotkeyRegiter::regiter = nullptr;
HotkeyRegiter::HotkeyRegiter(QWidget *parent)
{
    p = parent;
//    RegisterHotKey((HWND)p->winId(), 0, MOD_ALT, 'Z');
    qApp->installNativeEventFilter(this);
}

HotkeyRegiter::~HotkeyRegiter()
{

}

int HotkeyRegiter::matchKey(QString key)
{
    int k = 0;
    if (key.trimmed().compare("Alt", Qt::CaseInsensitive) == 0)
    {
        k = MOD_ALT;
    }
    if (key.trimmed().compare("Ctrl", Qt::CaseInsensitive) == 0)
    {
        k = MOD_CONTROL;
    }
    if (key.trimmed().compare("Shift", Qt::CaseInsensitive) == 0)
    {
        k = MOD_SHIFT;
    }
    return k;
}

HotkeyRegiter *HotkeyRegiter::GetInstance(QWidget *parent)
{
    if (!regiter)
    {
        regiter = new HotkeyRegiter(parent);
    }
    return regiter;
}

void HotkeyRegiter::Destruct()
{
    if (regiter)
    {
        delete regiter;
        regiter = nullptr;
    }
}

bool HotkeyRegiter::unRegister(int id)
{
    bool b = (bool)UnregisterHotKey((HWND)p->winId(), id);
    return b;
}

bool HotkeyRegiter::RegisterHotkey(int id, QString str)
{
    UINT k1 = 0;
    UINT k2 = 0;
    QStringList keys = str.toUpper().split("+");
    int size = keys.size();
    if (size != 2 && size != 3)
    {
        NotifyUI::ShowNotify(QStringLiteral("无效的快捷键"));
        return false;
    }
    bool b = true;
    if (size == 2)
    {
        k1 = matchKey(keys.first());
        char c = keys.last().trimmed().at(0).toLatin1();
        b = (bool)RegisterHotKey((HWND)p->winId(), id, k1, c);
    }
    else
    {
        k1 = matchKey(keys.first());
        k2 = matchKey(keys.at(1));
        char c = keys.last().trimmed().at(0).toLatin1();
        b = (bool)RegisterHotKey((HWND)p->winId(), id, k1 | k2, c);
    }
    return b;
}

bool HotkeyRegiter::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
    if (eventType == "windows_generic_MSG")
    {
        MSG *msg = (MSG *)message;
        if (msg->message == WM_HOTKEY && msg->wParam == 0)
        {
            if (!ConfigureData::getInstance()->IsTrue("key_enable"))
                return false;
            emit hotkeyClicked();
            return true;
        }
        if (msg->message == WM_HOTKEY && msg->wParam == 1)
        {
            if (!ConfigureData::getInstance()->IsTrue("key_enable"))
                return false;
            emit screenshotClicked();
            return true;
        }
        if (msg->message == WM_HOTKEY && msg->wParam == 2)
        {
            if (!ConfigureData::getInstance()->IsTrue("key_enable"))
                return false;
            emit hotkeyLocked();
            return true;
        }
        if (msg->message == WM_HOTKEY && msg->wParam == 3)
        {
            if (!ConfigureData::getInstance()->IsTrue("key_enable"))
                return false;
            emit hotKeyClose();
            return true;
        }
        if (msg->message == WM_HOTKEY && msg->wParam == 4)
        {
            if (!ConfigureData::getInstance()->IsTrue("key_enable"))
                return false;
            emit hotKeyShow();
            return true;
        }
        if (msg->message == WM_HOTKEY && msg->wParam == 5)
        {
            if (!ConfigureData::getInstance()->IsTrue("key_enable"))
                return false;
            emit hotKeyHide();
            return true;
        }
    }
    return false;
}
