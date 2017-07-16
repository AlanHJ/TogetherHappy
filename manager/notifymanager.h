#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <QObject>

/**
 * @brief The NotifyManager class
 * manager all notify
 */

class NotifyUI;
class NotifyManager : public QObject
{
    Q_OBJECT
    friend class NotifyUI;
public:
    enum NotifyOrder
    {
        Queue,
        First,
    };
private:
    NotifyManager();
    virtual~NotifyManager();
    static NotifyManager *GetInstance();
    static void DelInstance();
    void showNotify(QString &txt);
    void addNotify(NotifyUI *nf, NotifyOrder order = Queue);

    QList<NotifyUI *> lst;
    static NotifyManager *manager;
    NotifyUI *ui;

private slots:
    void removeNotify();
    void nextNotify();
    void timeOver();
};

#endif // NOTIFYMANAGER_H
