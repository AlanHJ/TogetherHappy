#ifndef EMOJILABEL_H
#define EMOJILABEL_H

#include <QLabel>
#include "../../data/db/emoticondata.h"
class EmojiLabel : public QLabel
{
    Q_OBJECT
public:
    EmojiLabel(QWidget *parent = 0);
    EmojiLabel(QString file, QSize size = QSize(0, 0), QWidget *parent = 0);
    ~EmojiLabel();
    QString ImagePath();
    void setMoiveRes(QString path, QSize size = QSize(0, 0));
    void setData(EmoticonData data);
    EmoticonData getData();
    void setMovieable(bool b);
    void setCheckable(bool b);
    void setChecked(bool b);
    bool getChecked();
    // QWidget interface
signals:
    void enterEmoji();
    void leaveEmoji();
    void clicked(EmoticonData data);
    void toggled(EmoticonData data, bool checked);
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    EmoticonData ed;
    bool isPressed;
    bool isMovie;
    bool isChecked;
    bool isCheckable;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // EMOJILABEL_H
