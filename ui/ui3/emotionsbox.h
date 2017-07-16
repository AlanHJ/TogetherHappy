#ifndef EMOTIONSBOX_H
#define EMOTIONSBOX_H

#include "../thwidgetbase.h"
#include "../../data/db/emoticondata.h"
#include "../../data/db/emoticongroupdata.h"

class EmojiTable;
class EmojiLabel;
class EmotionsBox : public THWidgetBase
{
    Q_OBJECT
private:
    explicit EmotionsBox(QWidget *parent = 0);
    ~EmotionsBox();

    static EmotionsBox *box;

public:
    static EmotionsBox* GetInstance();
    static void Destruct();

signals:
    void initProgress(int value);
    void select(QString str);
    void signalHide();
    void addTab(EmoticonGroupData egd);
    void addEmotion(QString tab, QList<EmoticonData> eds);
    void reload();
private slots:
    void loadData();
    void writedb(EmoticonData ed);
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    // QWidget interface
protected:
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);

    // QObject interface
public:
    bool event(QEvent *event);

private:
    EmojiLabel *prew;
    QList<EmojiTable *> tables;
    QMutex mutex;

};

#endif // EMOTIONSBOX_H
