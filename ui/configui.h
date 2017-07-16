#ifndef CONFIGUI_H
#define CONFIGUI_H

#include "thwidgetbase.h"
class QLabel;
class QSlider;
class QLineEdit;
class QPushButton;
class ConfigUI : public QWidget
{
    Q_OBJECT
public:
    ConfigUI(QWidget *p = 0);
    virtual~ConfigUI();
private:
    void init();

    QLineEdit *mNameEdit;
    QLineEdit *mIpEdit;
    QSlider *mRslider;
    QSlider *mGslider;
    QSlider *mBslider;
    QSlider *mAslider;
private slots:
    void apply();
    void RGBValue(int value);
    void eanbleIP();
    void autostart();
signals:
    void modifName(const QString &name);
};

#endif // CONFIGUI_H
