#ifndef EDITLABEL_H
#define EDITLABEL_H

#include <QWidget>
#include <QLineEdit>

class QLineEdit;
class QLabel;
class EditLabel : public QLineEdit
{
    Q_OBJECT
public:
    explicit EditLabel(QWidget *parent = 0);
    ~EditLabel();

private slots:
    void slotEditFinished();

private:
    QString css;
    bool isClicked;
    void init();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

class SearchLabel : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchLabel(QWidget *parent = 0);
    ~SearchLabel();
signals:
    void enterPress();
private slots:
    void slotEditFinished();
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
private:
    QString css;
    bool isClicked;
    void init();
};

#endif // EDITLABEL_H
