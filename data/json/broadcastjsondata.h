#ifndef BROADCASTJSONDATA_H
#define BROADCASTJSONDATA_H

#include <QObject>
#include <QJsonObject>

class BroadcastJsonData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ getUid WRITE setUid)
    Q_PROPERTY(QString gsoapip READ getGsoapip WRITE setGsoapip)
    Q_PROPERTY(bool tcpenabled READ getTcpenabled WRITE setTcpenabled)
    Q_PROPERTY(float version READ getVersion WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString versioninfo READ getVersioninfo WRITE setVersioninfo)
    Q_PROPERTY(QString nickname READ getNickname WRITE setNickname NOTIFY nicknameChanged)
    Q_PROPERTY(int status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString feel READ getFeel WRITE setFeel NOTIFY feelChanged)
    Q_PROPERTY(QString photo READ getPhoto WRITE setPhoto NOTIFY photoChanged)
    Q_PROPERTY(QString image READ getImage WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString mac READ getMac WRITE setMac)
public:
    BroadcastJsonData(QObject *parent = 0);
    BroadcastJsonData(const BroadcastJsonData &data);
    BroadcastJsonData *operator=(const BroadcastJsonData &data);
    virtual~BroadcastJsonData();
    BroadcastJsonData *copy();
    void clone(BroadcastJsonData &data);

    void parserJson(QJsonObject obj);
    QByteArray toJson();
    QJsonObject toObject();

    QString getUid() const;
    void setUid(const QString &value);

    float getVersion() const;
    void setVersion(float value);

    QString getNickname() const;
    void setNickname(const QString &value);

    int getStatus() const;
    void setStatus(int value);

    QString getFeel() const;
    void setFeel(const QString &value);

    QString getPhoto() const;
    void setPhoto(const QString &value);

    QString getImage() const;
    void setImage(const QString &value);

    QString getGsoapip() const;
    void setGsoapip(const QString &value);

    QString getVersioninfo() const;
    void setVersioninfo(const QString &value);

    bool getTcpenabled() const;
    void setTcpenabled(bool value);

    QString getMac() const;
    void setMac(const QString &value);

private slots:
    void parserJsonRun(QJsonObject obj);

signals:
    void versionChanged();
    void nicknameChanged();
    void statusChanged();
    void feelChanged();
    void photoChanged();
    void imageChanged();
    void parserComplete();

private:
    QString uid;
    QString gsoapip;
    float version;
    QString nickname;
    int status;
    QString feel;
    QString photo;
    QString image;
    QString versioninfo;
    bool tcpenabled;
    QString mac;

    QMutex mutex;
};

#endif // BROADCASTJSONDATA_H
