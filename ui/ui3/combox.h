#ifndef COMBOX_H
#define COMBOX_H

#include <QWidget>
#include <QComboBox>

class QListWidget;
class PupupWidget : public QWidget
{
    Q_OBJECT
friend class Combox;
public:
    PupupWidget(QWidget *p = 0);
    ~PupupWidget();
    void clear();
    void addItem(QString txt);
    void addItems(QStringList txts);
    bool isVisible1();
    void setVisible1(bool b);
    void hidePopup();
    void showPopup();
    QListWidget *list();
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);

private:
    QListWidget *lw;
    bool visible;
    QColor c1;
    QColor c2;

    // QWidget interface
protected:
    void hideEvent(QHideEvent *);
};

// 自定义的combox
class Combox : public QComboBox
{
    Q_OBJECT
public:
    explicit Combox(QWidget *parent = 0);
    ~Combox();
    void corrected();
    void clear1();
    void addItem1(QString txt);
    void addItems1(QStringList txts);
    void setTextAlignment(int alignment);
private slots:
    void slotCurrentTextChanged(const QString &txt);

private:
    PupupWidget *pupw;

    // QComboBox interface
public:
    void showPopup();
    void hidePopup();
};

#endif // COMBOX_H
