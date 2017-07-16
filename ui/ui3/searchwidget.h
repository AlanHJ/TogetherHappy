#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

class SearchLabel;
class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);
    ~SearchWidget();
    void clear();
signals:
    void searchTxtChanged(QString str);
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    SearchLabel *edit;
};

#endif // SEARCHWIDGET_H
