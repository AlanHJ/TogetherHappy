#ifndef EMOJITABLE_H
#define EMOJITABLE_H

#include <QTableWidget>
#include "../../data/db/emoticondata.h"
static const int MaxRow = 6;
static const int DefaultRow = 1;
static const int DefaultColumn = 8;
static const QSize DefaultTableSize = QSize(52, 52);
static const QSize DefaultTableImageSize = QSize(46, 46);
class QMenu;
class EmojiLabel;
class EmojiTable : public QTableWidget
{
    Q_OBJECT
public:
    EmojiTable(QWidget *parent = 0);
    virtual~EmojiTable();
    void initEmojiTable(QSize size = DefaultTableSize);
    void insertEmoji(QList<EmoticonData> datas, bool ischeckable = false, QSize size = DefaultTableImageSize);
public slots:
    void updateEmoji(EmoticonData ed, bool ischeckable = false, QSize size = DefaultTableImageSize);
    void setCheckedAll(bool b);
signals:
    void emojiClicked(EmoticonData ed);
    void emojiToggled(EmoticonData ed, bool checked);
    void enterEmoji(QString path);
    void leaveEmoji();

    // QObject interface
public:
    virtual bool eventFilter(QObject *, QEvent *);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void leaveEvent(QEvent *e);
};

#endif // EMOJITABLE_H
