#ifndef CARDUI_H
#define CARDUI_H

#include "../thwidgetbase.h"
class QPropertyAnimation;
class EmojiLabel;
class QLabel;
class CardUI : public THWidgetBase
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getopacity WRITE setopacity)
public:
    CardUI(QWidget *parent = 0);
    virtual~CardUI();
    qreal getopacity();
    void setopacity(qreal opacity);
public slots:
    void setPicture(QString path);
    void setFirstTxt(QString str);
    void setSecondTxt(QString str);
    void setThirdTxt(QString str);
    void setIntroduceTxt(QString str);
    void setVersion(float v);
    void setGsoapGet();
    void showAnimation();
    void hideAnimation();
private slots:
    void linkUrl(QString url);
private:
    QPropertyAnimation *animation;
    EmojiLabel *emoji;
    QLabel *name1;
    QLabel *name2;
    QLabel *ip;
    QLabel *say;
    QLabel *version;

    // QWidget interface
protected:
    void leaveEvent(QEvent *);
};

#endif // CARDUI_H
