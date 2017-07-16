#ifndef MEMBERUI_H
#define MEMBERUI_H

#include <QWidget>

class QLabel;
class QListWidget;
class QPushButton;
class MemberUI : public QWidget
{
    Q_OBJECT
public:
    explicit MemberUI(QWidget *parent = 0);
    ~MemberUI();
    void setTitle(QString title);
    void setPbTitle(QString title);
    void setBtVisible(bool b);
signals:
    void choices(QStringList lst);
public slots:
    void setMembers(QStringList lst);
    QStringList getChoiceMembers();
private slots:
    void okCilcked();
private:
    void init();
    QLabel *mLab;
    QListWidget *mList;
    QPushButton *mPb;
};

#endif // MEMBERUI_H
