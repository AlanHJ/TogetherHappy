#include "widgetmanager.h"
#include <QHash>
#include "../ui/ui3/chat.h"

WidgetManager *WidgetManager::wm = nullptr;
WidgetManager *WidgetManager::GetInstance()
{
    if (!wm)
    {
        wm = new WidgetManager();
    }
    return wm;
}

void WidgetManager::Destruct()
{
    if (wm)
        wm->deleteLater();
}

void WidgetManager::registerWidget(THWidgetBase *w)
{
    auto it = hash.find(w);
    if (it != hash.end() && it.key() == w)
    {
        return;
    }
    hash.insert(w, w);
}

void WidgetManager::unregisterWidget(THWidgetBase *w)
{
    auto it = hash.find(w);
    if (it != hash.end() && it.key() == w)
    {
        hash.erase(it);
    }
}

void WidgetManager::updateWidget()
{
    auto it = hash.begin();
    while (it != hash.end())
    {
        THWidgetBase *w = it.value();
        if (w)
            w->update();
        ++it;
    }
}

void WidgetManager::updateWidgetColor(QColor color1, QColor color2)
{
    auto it = hash.begin();
    while (it != hash.end())
    {
        THWidgetBase *w = it.value();
        if (w)
            w->updateColor(color1, color2);
        ++it;
    }
}

void WidgetManager::setBezierEnable(bool b)
{
    auto it = hash.begin();
    while (it != hash.end())
    {
        THWidgetBase *w = it.value();
        if (w)
            w->setBezierEnable(b);
        ++it;
    }
}

bool WidgetManager::showReplayChat(bool alert)
{
    QHash<qint64, Chat*> chats;
    auto it = hash.begin();
    while (it != hash.end())
    {
        Chat *chat = dynamic_cast<Chat*>(it.value());
        if (chat && chat->isVisible() && (alert ? chat->isAlertFlag() : true))
        {
            chats.insert(chat->getLastRecvTime(), chat);
        }
        ++it;
    }
    QList<qint64> list = chats.keys();
    if (list.isEmpty())
        return false;
    qSort(list.begin(), list.end());
    Chat *firstChat = chats.value(list.last());
    Chat *secondChat = chats.value(list.first());
    firstChat->setLastRecvTime(secondChat->getLastRecvTime() - 1);
    firstChat->showNormal();
    firstChat->activateWindow();
    return true;
}

WidgetManager::WidgetManager(QObject *parent) : QObject(parent)
{

}

WidgetManager::~WidgetManager()
{
    hash.clear();
}

