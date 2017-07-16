#include "themepicker.h"
#include "colorbutton.h"
#include "colorpicker.h"
#include "coloralphapicker.h"
#include "../happytitlebar.h"
#include "../../util/thmethod.h"
#include "../../manager/widgetmanager.h"
#include "../../config/configuredata.h"
#include <QtWidgets>

ThemePicker::ThemePicker(QWidget *parent) : THWidgetBase(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(290, 230);
    setTitleBarWidth(290);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");
    bar->setBarContent(QStringLiteral("¹þÆ¡ÑÕÉ«Ñ¡Ôñ"));

    QColorPicker *picker = new QColorPicker(this);
    ColorAlphaPicker *alp1 = new ColorAlphaPicker(this);
    alp1->setRgb(47, 107, 117, 240);
    ColorAlphaPicker *alp2 = new ColorAlphaPicker(this);
    alp2->setRgb(47, 107, 147, 250);

    group = new QButtonGroup(this);
    group->addButton(alp1, 0);
    group->addButton(alp2, 1);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 10);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            ColorButton *bt = new ColorButton(this);
            connect(bt, &ColorButton::clicked, this, [=] () {
                ColorButton *bt = qobject_cast<ColorButton*>(sender());
                if (!bt)
                    return;
                QColor c = bt->CurrentColor();
                ColorAlphaPicker *picker = qobject_cast<ColorAlphaPicker *>(group->checkedButton());
                if (!picker)
                    return;
                picker->setRgb(c.red(), c.green(), c.blue(), 230);
            });
            QString str = MDColor[i][j];
            bt->setColor(QColor(str));
            bt->setFixedSize(22, 22);
            grid->addWidget(bt, i, j);
        }
    }

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setSpacing(0);
    v->setContentsMargins(0, 35, 0, 0);
    v->addWidget(picker, 0, Qt::AlignCenter);
    v->setSpacing(1);
    v->addWidget(alp1, 0, Qt::AlignCenter);
    v->addWidget(alp2, 0, Qt::AlignCenter);
    v->setSpacing(1);
    v->addLayout(grid);

    alp1->setChecked(true);

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        this->close();
        this->deleteLater();
    });
    connect(picker, SIGNAL(newCol(int,int,int)),
            this, SLOT(SelectColorByHsv(int,int,int)));

    connect(alp1, &ColorAlphaPicker::newColor, this, [=] () {
        PickColor();
    });
    connect(alp2, &ColorAlphaPicker::newColor, this, [=] () {
        PickColor();
    });

    // load data
    ConfigureData *conf = ConfigureData::getInstance();
    QColor c1 = conf->getColorIni("color1");
    QColor c2 = conf->getColorIni("color2");
    picker->setColor(c1);
    alp1->setColor(c1);
    alp2->setColor(c2);
    updateColor(c1, c2);
}

ThemePicker::~ThemePicker()
{

}

void ThemePicker::SelectColorByHsv(int h, int s, int v)
{
    QColor color;
    color.setHsv(h, s, v);

    ColorAlphaPicker *picker = qobject_cast<ColorAlphaPicker *>(group->checkedButton());
    if (!picker)
        return;
    picker->setRgb(color.red(), color.green(), color.blue(), 230);
}

void ThemePicker::PickColor()
{
    ColorAlphaPicker *picker0 = qobject_cast<ColorAlphaPicker *>(group->button(0));
    ColorAlphaPicker *picker1 = qobject_cast<ColorAlphaPicker *>(group->button(1));
    if (!picker0 || !picker1)
        return;
    QColor c1 = picker0->currentColor();
    QColor c2 = picker1->currentColor();
    WidgetManager::GetInstance()->updateWidgetColor(c1, c2);
    ConfigureData::getInstance()->setColorIni("color1", c1);
    ConfigureData::getInstance()->setColorIni("color2", c2);
}

