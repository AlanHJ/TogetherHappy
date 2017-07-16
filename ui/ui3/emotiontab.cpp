#include "emotiontab.h"

EmotionTab::EmotionTab(QWidget *parent)
    : QTabWidget(parent)
{

}

EmotionTab::~EmotionTab()
{

}

int EmotionTab::indexOfByText(QString txt)
{
    // txt must be unique
    int rlt = -1;
    for (int i = 0; i < count(); i++)
    {
        if (tabText(i) == txt)
        {
            rlt = i;
            break;
        }
    }
    return rlt;
}

