#include "notifymanager.h"
#include "../ui/notifyui.h"
#include <QTimer>
#include <QDebug>

NotifyManager *NotifyManager::manager  = nullptr;
NotifyManager::NotifyManager()
{
    ui = new NotifyUI();
    connect(ui, SIGNAL(signalTimeOver()), this, SLOT(timeOver()));
}

NotifyManager::~NotifyManager()
{
    auto it = lst.begin();
    while (it != lst.end())
    {
        NotifyUI *ui = *it;
        delete ui;
        ui = nullptr;
        ++it;
    }
    delete ui;
}

NotifyManager *NotifyManager::GetInstance()
{
    if (nullptr == manager)
    {
        manager = new NotifyManager();
    }
    return manager;
}

void NotifyManager::DelInstance()
{
    if (nullptr != manager)
    {
        delete manager;
        manager = nullptr;
    }
}

void NotifyManager::showNotify(QString &txt)
{
//    float ms = ui->time.elapsed();
//    qDebug() << -log(ms / 1000.0) + 10;
    ui->time.restart();
    ui->setNotifyText(txt);
    if (!lst.isEmpty())
    {
        return;
    }
    if (!ui->isValid())
    {
        ui->setValid(true);
        ui->showAnimation();
    }
    else
    {
        if (ui->animationSize->state() == QAbstractAnimation::Stopped)
        {
            ui->stressAnimation();
        }
    }
}

void NotifyManager::addNotify(NotifyUI *nf, NotifyOrder order)
{
    if (lst.contains(nf))
    {
        return;
    }
    if (order == First)
    {
        lst.push_front(nf);
    }
    else
    {
        lst.push_back(nf);
    }
    ui->hide();
    nextNotify();
}

void NotifyManager::nextNotify()
{
    if (lst.isEmpty())
    {
        return;
    }

    NotifyUI *showUI = lst.first();
    if (showUI->valid)
    {
        return;
    }
    showUI->valid = true;
    showUI->showAnimation();
    connect(showUI, SIGNAL(signalClose()), this, SLOT(nextNotify()));
    QTimer::singleShot(showUI->showTime(), this, SLOT(removeNotify()));
}

void NotifyManager::removeNotify()
{
    if (!lst.isEmpty())
    {
        NotifyUI *nui = lst.first();
        nui->closeAnimation();
        lst.pop_front();
    }
}

void NotifyManager::timeOver()
{
    ui->setValid(false);
    ui->hideAnimation();
    nextNotify();
}

