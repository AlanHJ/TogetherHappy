#ifndef EMOTIONCOMMON_H
#define EMOTIONCOMMON_H

#include "../thwidgetbase.h"
#include "../../data/db/emoticondata.h"

static QMutex commonmutex;
class EmojiLabel;
class EmotionCommon : public THWidgetBase
{
    Q_OBJECT
public:
    explicit EmotionCommon(QWidget *parent = 0);
    virtual~EmotionCommon();
    void closeEmotion();
signals:
    void select(QString str);
    void loadPath(int i, EmoticonData data);
private slots:
    void loadData();
    void writedb(EmoticonData ed);
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);

private:
    QList<EmojiLabel*> lst;
    EmojiLabel *prew;
};

#endif // EMOTIONCOMMON_H
