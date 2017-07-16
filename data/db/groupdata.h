#ifndef GROUPDATA_H
#define GROUPDATA_H

#include <QObject>

class GroupData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ getUid WRITE setUid)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString photo READ getPhoto WRITE setPhoto)
    Q_PROPERTY(bool deleteable READ getDeleteable WRITE setDeleteable)
    Q_PROPERTY(bool editable READ getEditable WRITE setEditable)
    Q_PROPERTY(QString creationtime READ getCreationtime WRITE setCreationtime)
    Q_PROPERTY(int updatetime READ getUpdatetime WRITE setUpdatetime)
    Q_PROPERTY(QString creator READ getCreator WRITE setCreator)
    Q_PROPERTY(QString groupdescribe READ getGroupdescribe WRITE setGroupdescribe)
    Q_PROPERTY(QString groupmember READ getGroupmember WRITE setGroupmember)
    Q_PROPERTY(QString attr READ getAttr WRITE setAttr)
    Q_PROPERTY(QString netmask READ getNetmask WRITE setNetmask)
    Q_PROPERTY(int port READ getPort WRITE setPort)
public:
    Q_INVOKABLE GroupData(QObject *parent = 0);
    GroupData(const GroupData &data);
    GroupData *operator=(const GroupData &data);
    ~GroupData();

    QString getUid() const;
    void setUid(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    bool getDeleteable() const;
    void setDeleteable(bool value);

    bool getEditable() const;
    void setEditable(bool value);

    QString getCreationtime() const;
    void setCreationtime(const QString &value);

    int getUpdatetime() const;
    void setUpdatetime(int value);

    QString getCreator() const;
    void setCreator(const QString &value);

    QString getGroupdescribe() const;
    void setGroupdescribe(const QString &value);

    QString getGroupmember() const;
    void setGroupmember(const QString &value);

    QString getAttr() const;
    void setAttr(const QString &value);

    QString getNetmask() const;
    void setNetmask(const QString &value);

    QString getPhoto() const;
    void setPhoto(const QString &value);

    int getPort() const;
    void setPort(int value);

private:
    QString uid;
    QString name;
    QString photo;
    bool deleteable;
    bool editable;
    QString creationtime;
    int updatetime;
    QString creator;
    QString groupdescribe;
    QString groupmember;
    QString attr;
    QString netmask;
    int port;
};

#endif // GROUPDATA_H
