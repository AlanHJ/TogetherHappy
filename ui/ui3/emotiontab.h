#ifndef EMOTIONTAB_H
#define EMOTIONTAB_H

#include <QTabWidget>

class EmotionTab : public QTabWidget
{
    Q_OBJECT
public:
    EmotionTab(QWidget *parent = 0);
    ~EmotionTab();
    int indexOfByText(QString txt);
};

#endif // EMOTIONTAB_H
