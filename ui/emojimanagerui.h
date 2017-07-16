#ifndef EMOJIMANAGERUI_H
#define EMOJIMANAGERUI_H

#include <QWidget>

class QTabWidget;
class EmojiManagerUI : public QWidget
{
    Q_OBJECT
public:
    EmojiManagerUI();
    virtual~EmojiManagerUI();
signals:
    void leaveEmoji();
    void enterEmoji(QString path);
    void currentEmojiPath(QString path);
    void updateEmoji(QString path);
private slots:
    void tabClicked();
    void editClicked();
private:
    void init();

    QTabWidget *tab;
};

#endif // EMOJIMANAGERUI_H
