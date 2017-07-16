#ifndef HAPPYPROGRESSUI_H
#define HAPPYPROGRESSUI_H

#include <QWidget>

class QLabel;
class QTextBrowser;
class HappyProgressUI : public QWidget
{
    Q_OBJECT
public:
    explicit HappyProgressUI(QWidget *parent = 0);
    ~HappyProgressUI();
    void setShowName(QString name);
signals:
    void setProgress(int value);
public slots:
    void setProgressInfo(QString info);
    void slotProgress(int value);
private slots:
    void closeUI();
private:
    void init();
    QLabel *mLab1;
    QTextBrowser *mLab2;
};

#endif // HAPPYPROGRESSUI_H
