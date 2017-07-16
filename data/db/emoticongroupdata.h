#ifndef EMOTICONGROUPDATA_H
#define EMOTICONGROUPDATA_H

#include <QObject>

class EmoticonGroupData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tab READ getGroup WRITE setGroup)
    Q_PROPERTY(bool deleteable READ getDeleteable WRITE setDeleteable)
    Q_PROPERTY(bool editable READ getEditable WRITE setEditable)
    Q_PROPERTY(bool readable READ getReadable WRITE setReadable)
public:
    Q_INVOKABLE EmoticonGroupData(QObject *parent = 0);
    EmoticonGroupData(const EmoticonGroupData &data);
    EmoticonGroupData *operator=(const EmoticonGroupData &data);
    ~EmoticonGroupData();

    QString getGroup() const;
    void setGroup(const QString &value);

    bool getDeleteable() const;
    void setDeleteable(bool value);

    bool getEditable() const;
    void setEditable(bool value);

    bool getReadable() const;
    void setReadable(bool value);

private:
    QString group;
    bool deleteable;
    bool editable;
    bool readable;
};

#endif // EMOTICONGROUPDATA_H
