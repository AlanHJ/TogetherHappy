#ifndef OPTIONPERSONALIZATION_H
#define OPTIONPERSONALIZATION_H

#include "optionwidget.h"

class QLineEdit;
class EmojiLabel;
class OptionPersonalization : public OptionWidget
{
    Q_OBJECT
public:
    explicit OptionPersonalization(QWidget *parent = 0);
    ~OptionPersonalization();

signals:

private slots:
    void buttonsClicked(int id);
    void emojiClicked();

    void saveModify();
private:
    EmojiLabel *photo;
    EmojiLabel *image;
    QLineEdit *edit;

};

#endif // OPTIONPERSONALIZATION_H
