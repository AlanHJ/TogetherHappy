#include "optionwidget.h"

OptionWidget::OptionWidget(QWidget *parent) : QWidget(parent)
{

}

OptionWidget::~OptionWidget()
{

}

void OptionWidget::setIndex(int id)
{
    this->index = id;
}

