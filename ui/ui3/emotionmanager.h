#ifndef EMOTIONMANAGER_H
#define EMOTIONMANAGER_H

#include <QObject>
#include <QListWidget>
#include <QStackedWidget>
#include "../thwidgetbase.h"
#include "../../data/db/emoticondata.h"
#include "../../data/db/emoticongroupdata.h"

class SelfMenu;
class ProgressUI;
class PupupWidget;
class EmojiTable;
class EmotionItem : public QListWidgetItem
{
public:
    EmotionItem(QListWidget *view = 0, int type = Type);
    EmoticonGroupData getEgd();
    void setEgd(EmoticonGroupData egd);
private:
    EmoticonGroupData egd;
};

class EmotionListWidget : public QListWidget
{
    Q_OBJECT
public:
    EmotionListWidget(QWidget *parent = 0);
    int indexOfByStr(QString str);
    bool containByStr(QString str);
    EmotionItem *eitem(int index);
signals:
    void deleteGroup(EmoticonGroupData egd);
    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *e);

private:
    SelfMenu *menu;
};

class EmotionStackedWidget : public QStackedWidget
{
public:
    EmotionStackedWidget(QWidget *parent = 0);
    int indexOfByObjname(QString txt);
    void addTable(EmojiTable *tab);
    void clearTabs();
    void setCheckedAll(bool b);
private:
    QHash<int, EmojiTable*> tabs;
};

class EmojiLabel;
class EmotionManager : public THWidgetBase
{
    Q_OBJECT
public:
    explicit EmotionManager(QWidget *parent = 0);
    ~EmotionManager();

signals:
    void execError(QString er);
    void execProgress(int value);
    void addTab(EmoticonGroupData egd);
    void addEmotion(QString tab, EmoticonData ed);
private slots:
    ProgressUI *reload();
    void loadData();
    void deleteGroup(QString txt);
    void moveEmotions(QString tab);
    void addEmotions(QStringList paths, QString tab);
    void deleteEmotions();
    ProgressUI *showProgress(QString txt);
private:
    EmotionStackedWidget *stack;
    EmotionListWidget *list;
    EmojiLabel *prew;
    PupupWidget *pupw;
    QHash<QString, EmoticonData> eds;
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *e);
};

#endif // EMOTIONMANAGER_H
