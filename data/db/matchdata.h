#ifndef MATCHDATA_H
#define MATCHDATA_H

#include <QObject>

class MatchData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ getUid WRITE setUid)
    Q_PROPERTY(QString ip READ getIp WRITE setIp)
    Q_PROPERTY(QString editname READ getEditname WRITE setEditname)
    Q_PROPERTY(QString nickname READ getNickname WRITE setNickname)
    Q_PROPERTY(int modifycounts READ getModifycounts WRITE setModifycounts)
    Q_PROPERTY(int edittime READ getEdittime WRITE setEdittime)
public:
    Q_INVOKABLE MatchData(QObject *parent = 0);
    MatchData(const MatchData &data);
    MatchData *operator=(const MatchData &data);
    ~MatchData();


    QString getUid() const;
    void setUid(const QString &value);

    QString getIp() const;
    void setIp(const QString &value);

    QString getEditname() const;
    void setEditname(const QString &value);

    QString getNickname() const;
    void setNickname(const QString &value);

    int getModifycounts() const;
    void setModifycounts(int value);

    int getEdittime() const;
    void setEdittime(int value);

private:
    QString uid;
    QString ip;
    QString editname;
    QString nickname;
    int modifycounts;
    int edittime;
};

#endif // MATCHDATA_H
