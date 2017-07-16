#ifndef HAPPYITEMDELEGATE_H
#define HAPPYITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class HappyItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    HappyItemDelegate(QObject *parent = 0);
    ~HappyItemDelegate();

    // QAbstractItemDelegate interface
public:
};

#endif // HAPPYITEMDELEGATE_H
