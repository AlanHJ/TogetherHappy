#ifndef OPTIONCOMMON_H
#define OPTIONCOMMON_H

#include "optionwidget.h"

class OptionCommon : public OptionWidget
{
    Q_OBJECT
public:
    explicit OptionCommon(QWidget *parent = 0);
    ~OptionCommon();

private:
    int calaValue(float v);
};

#endif // OPTIONCOMMON_H
