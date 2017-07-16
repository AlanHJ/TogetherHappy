#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QObject>

class ConfigData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key READ getKey WRITE setKey)
    Q_PROPERTY(QString value READ getValue WRITE setValue)
public:
    Q_INVOKABLE ConfigData(QObject *parent = 0);
    ConfigData(const ConfigData &data);
    ConfigData *operator=(const ConfigData &data);
    ~ConfigData();

    QString getKey() const;
    void setKey(const QString &value);

    QString getValue() const;
    void setValue(const QString &value);

private:
    QString k;
    QString val;
};

#endif // CONFIGDATA_H
