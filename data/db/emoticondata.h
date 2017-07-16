#ifndef EMOTICONDATA_H
#define EMOTICONDATA_H

#include <QObject>

class EmoticonData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString path READ getPath WRITE setPath)
    Q_PROPERTY(QString thumb READ getThumb WRITE setThumb)
    Q_PROPERTY(int counts READ getCounts WRITE setCounts)
    Q_PROPERTY(QString inserttime READ getInserttime WRITE setInserttime)
    Q_PROPERTY(QString lasttime READ getLasttime WRITE setLasttime)
    Q_PROPERTY(QString tab READ getGroup WRITE setGroup)
    Q_PROPERTY(bool deleteable READ getDeleteable WRITE setDeleteable)
    Q_PROPERTY(bool editable READ getEditable WRITE setEditable)
    Q_PROPERTY(bool readable READ getReadable WRITE setReadable)
public:
    Q_INVOKABLE EmoticonData(QObject *parent = 0);
    EmoticonData(const EmoticonData &data);
    EmoticonData *operator=(const EmoticonData &data);
    ~EmoticonData();

    QString getName() const;
    void setName(const QString &value);

    QString getPath() const;
    void setPath(const QString &value);

    int getCounts() const;
    void setCounts(int value);

    QString getInserttime() const;
    void setInserttime(const QString &value);

    QString getLasttime() const;
    void setLasttime(const QString &value);

    QString getThumb() const;
    void setThumb(const QString &value);

    QString getGroup() const;
    void setGroup(const QString &value);

    bool getDeleteable() const;
    void setDeleteable(bool value);

    bool getEditable() const;
    void setEditable(bool value);

    bool getReadable() const;
    void setReadable(bool value);

private:
    QString name;
    QString path;
    QString thumb;
    int counts;
    QString inserttime;
    QString lasttime;
    QString group;
    bool deleteable;
    bool editable;
    bool readable;
};

#endif // EMOTICONDATA_H
