#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QObject>
#include "../ui/thwidgetbase.h"

class Chat;
class WidgetManager : public QObject
{
    Q_OBJECT
public:
    static WidgetManager *GetInstance();
    static void Destruct();
    void registerWidget(THWidgetBase *w);
    void unregisterWidget(THWidgetBase *w);
    void updateWidget();
    void updateWidgetColor(QColor color1, QColor color2);
    void setBezierEnable(bool b);
    template <class T>
    bool isExist(T **t)
    {
        auto it = hash.begin();
        while (it != hash.end())
        {
            *t = dynamic_cast<T*>(it.value());
            if (*t)
            {
                return true;
            }
            ++it;
        }
        return false;
    }
    template <class T>
    bool popWidget(T **t)
    {
        // find all T
        QHash<qint64, T*> ts;
        auto it = hash.begin();
        while (it != hash.end())
        {
            THWidgetBase *w = it.value();
            qint64 time = w->getActivateTime();
            T *t1 = dynamic_cast<T*>(w);
            if (t1 && w->isVisible())
            {
                ts.insert(time, t1);
            }
            ++it;
        }
        QList<qint64> list = ts.keys();
        if (list.isEmpty())
            return false;
        qSort(list.begin(), list.end());
        *t = ts.value(list.last());
        return true;
    }

    bool showReplayChat(bool alert = true);

private:
    explicit WidgetManager(QObject *parent = 0);
    ~WidgetManager();

    static WidgetManager *wm;

    QHash<THWidgetBase*, THWidgetBase*> hash;
};

#endif // WIDGETMANAGER_H
