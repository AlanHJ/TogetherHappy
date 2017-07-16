#ifndef CONFIGUREDATA_H
#define CONFIGUREDATA_H

#include <QtCore>
#include <QColor>
static QString DefaultItem = "RUN";
class ConfigureData
{
public:
    static ConfigureData *getInstance();
    static void disInstance();
    void initConfig();

    QString getUuid()
    {
        QString id  = getIni("uid");
        return id;
    }

    QString getPassword()
    {
        QString str = getIni("pw");
        return QString::fromLocal8Bit(QByteArray::fromBase64(str.toLocal8Bit()));
    }
    QString getIni(QString key)
    {
        QString temp;
        getIni(key, temp, QStringLiteral(""));
        return temp;
    }
    int getIntIni(QString key)
    {
        int i1 = 0;
        int i2 = 0;
        getIni(key, i1, i2);
        return i1;
    }
    bool IsTrue(QString key)
    {
        if (getIntIni(key) != 0)
        {
            return true;
        }
        else if (getIni(key) == "true")
        {
            return true;
        }
        else if (getIni(key) == "false")
        {
            return false;
        }
        else
        {
            return false;
        }
    }

    QColor getColorIni(QString key)
    {
        QColor color;
        if (key == "color1")
        {
            color = QColor(255,118,118,230);
        }
        else
        {
            color = QColor(33,150,243,230);
        }
        QStringList lst = getIni(key).split(",");
        if (lst.size() == 4)
        {
            color = QColor(lst.at(0).toInt(),
                           lst.at(1).toInt(),
                           lst.at(2).toInt(),
                           lst.at(3).toInt());
        }
        else if (lst.size() == 3)
        {
            color = QColor(lst.at(0).toInt(),
                           lst.at(1).toInt(),
                           lst.at(2).toInt(),
                           255);
        }
        return color;
    }

    void setPassword(QString pw)
    {
        setIni("pw", QString::fromLocal8Bit(pw.toLocal8Bit().toBase64()));
    }
    void setColorIni(QString key, QColor color)
    {
        QStringList lst;
        lst << QString::number(color.red())
            << QString::number(color.green())
            << QString::number(color.blue())
            << QString::number(color.alpha());
        setIni(key, lst.join(","));
    }
    template <typename T>
    void setIni(QString key, T t)
    {
        QVariant val(t);
        writeIni(key, val);
    }
    template <typename T>
    void getIni(QString key, T &t, T defaultv)
    {
        QVariant val(t);
        QVariant deval(defaultv);
        readIni(key, val, deval);
        t = val.value<T>();
    }
private:
    ConfigureData();
    virtual~ConfigureData();
    bool readIni(QString key, QVariant &value, QVariant deval);
    bool writeIni(QString key, QVariant val);
    void writedb(QString key, QVariant var);
    void writeDbAll();
private:
    class PrivData;
    PrivData *mData;
};
#endif // CONFIGUREDATA_H
