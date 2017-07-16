#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>

class QLabel;
class QTreeWidgetItem;
class HappyTreeWidget;
class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QTreeWidgetItem *item, HappyTreeWidget *tree);
    ~TopWidget();

public slots:
    void setText(QString str);
    void slotExpanded(bool b);

private:
    QLabel *arrow;
    QLabel *txt;
};

#endif // TOPWIDGET_H
