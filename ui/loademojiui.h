#ifndef LOADEMOJIUI_H
#define LOADEMOJIUI_H

#include <QWidget>

class QLabel;
class QSlider;
class QProgressBar;
class LoadEmojiUI : public QWidget
{
    Q_OBJECT
public:
    explicit LoadEmojiUI(QWidget *parent = 0);
    ~LoadEmojiUI();

signals:

private slots:
    void clickLoad();
    void valueChanged(int value);
    void progressValue(int value);
private:
    void init();

    float   mValue;
    QLabel *mLabel;
    QSlider *mSlider;
    QProgressBar *mBar;
};

#endif // LOADEMOJIUI_H
