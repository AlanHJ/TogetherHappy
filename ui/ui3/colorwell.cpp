#include "colorwell.h"
#include "../happytitlebar.h"
#include <QtWidgets>
#include "colorpicker.h"
#include "colorluminancepicker.h"
#include "colorbutton.h"
#include "../../util/thmethod.h"

ColorWell::ColorWell(QColor init, QWidget *parent) : THWidgetBase(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(290, 200);
    setTitleBarWidth(290);
    bar->setBarButtons(HappyTitleBar::MinButtonHint);
    bar->setBarButtons(HappyTitleBar::MaxButtonHint);
    bar->setExtraButtonVisible(false);
    bar->setBarIcon(":res/happy.png");   // 需重新设定
    bar->setBarContent(QStringLiteral("哈啤调色板"));

    QColorPicker *picker = new QColorPicker(this);
    QColorLuminancePicker *lum = new QColorLuminancePicker(this);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            ColorButton *bt = new ColorButton(this);
            connect(bt, SIGNAL(clicked()), this, SLOT(DefaultColorSelect()));
            bt->setColor(DefaultColor[i][j]);
            bt->setFixedSize(20, 20);
            grid->addWidget(bt, i, j);
        }
    }

    QVBoxLayout *v = new QVBoxLayout(this);
    v->setContentsMargins(10, 30, 5, 5);
    v->setSpacing(0);
    v->addWidget(picker);
    v->addWidget(lum);
    v->addLayout(grid);

    connect(bar, &HappyTitleBar::signalClose, this, [=] () {
        this->close();
        this->deleteLater();
    });
    connect(picker, SIGNAL(newCol(int,int,int)),
            lum, SLOT(setCol(int,int,int)));
    connect(picker, SIGNAL(newCol(int,int,int)),
            this, SLOT(SelectColorByHsv(int,int,int)));
    connect(lum, SIGNAL(newHsv(int,int,int)),
            this, SLOT(SelectColorByHsv(int,int,int)));
    picker->setColor(init);
    lum->setColor(init);
}

ColorWell::~ColorWell()
{
    //    qDebug() << "color well delete";
}

void ColorWell::DefaultColorSelect()
{
    ColorButton *bt = qobject_cast<ColorButton*>(sender());
    if (!bt)
        return;
    emit signalSelectColor(bt->CurrentColor());
}

void ColorWell::SelectColorByHsv(int h, int s, int v)
{
    QColor color;
    color.setHsv(h, s, v);
    emit signalSelectColor(color);
}

