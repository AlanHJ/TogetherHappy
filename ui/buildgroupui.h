#ifndef BUILDGROUPUI_H
#define BUILDGROUPUI_H

#include <QWidget>

class MemberUI;
class QLineEdit;
class BuildGroupUI : public QWidget
{
    Q_OBJECT
public:
    explicit BuildGroupUI(QWidget *parent = 0);
    ~BuildGroupUI();
    void setMembers(QStringList lst);
signals:
    void signalCreateGroup(QString title, QString uuid, QStringList members, int flag);
private slots:
    void sureClicked();
private:
    void init();

    MemberUI *mMembers;
    QLineEdit *mEd;
};

#endif // BUILDGROUPUI_H
