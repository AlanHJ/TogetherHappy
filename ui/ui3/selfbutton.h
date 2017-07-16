#ifndef SELFBUTTON_H
#define SELFBUTTON_H

#include <QPushButton>

class SelfButton : public QPushButton
{
    Q_OBJECT
public:
    SelfButton(QWidget *w = 0);
    ~SelfButton();
    bool isReset();
    void setReset(bool b);

signals:
    void signalEnter();
    void signalLeave();

    // QWidget interface
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    bool reset;
};

#endif // SELFBUTTON_H
