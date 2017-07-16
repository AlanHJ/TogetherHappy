#ifndef INITUI_H
#define INITUI_H

#include <QDialog>
#include <QTimer>

class QLineEdit;
class QPushButton;
class InitUI : public QDialog
{
    Q_OBJECT
public:
    explicit InitUI(QDialog *parent = 0);
    virtual~InitUI();
private:
    void init();
signals:
    void countdownout();
private slots:
    void selectPath();
    void selectOk();
    void timeout();
private:
    QLineEdit *mPath;
    QPushButton *mOk;
    QTimer mTimer;
    int mTime;
};

#endif // INITUI_H
