#ifndef OPTIONKEYS_H
#define OPTIONKEYS_H

#include "optionwidget.h"
#include <qlineedit.h>

class KeyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    KeyLineEdit(QWidget *parent = 0);
    ~KeyLineEdit();
    void setDefault(QString keys);
    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void enterEvent(QEvent *e);

private:
    QString defaultKeys;
};

class OptionKeys : public OptionWidget
{
    Q_OBJECT
public:
    explicit OptionKeys(QWidget *parent = 0);
    ~OptionKeys();
};

#endif // OPTIONKEYS_H
