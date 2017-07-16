#include "broadcastjsondata.h"
#include "../../util/thmethod.h"
#include <QtConcurrent/QtConcurrent>
#include <QMetaProperty>
#include <QJsonDocument>

BroadcastJsonData::BroadcastJsonData(QObject *parent) : QObject(parent)
{
    version = HAPPY_VERSON;
    status = 0;
    tcpenabled = false;
    versioninfo = cn("请查看关于获取升级信息");
}

BroadcastJsonData::BroadcastJsonData(const BroadcastJsonData &data)
{
    BroadcastJsonData &d = (BroadcastJsonData&)data;
    uid = d.uid;
    gsoapip = d.gsoapip;
    version = d.version;
    nickname = d.nickname;
    status = d.status;
    feel = d.feel;
    photo = d.photo;
    image = d.image;
    tcpenabled = d.tcpenabled;
    versioninfo = d.versioninfo;
    mac = d.mac;
}

BroadcastJsonData *BroadcastJsonData::operator=(const BroadcastJsonData &data)
{
    BroadcastJsonData &d = (BroadcastJsonData&)data;
    uid = d.uid;
    gsoapip = d.gsoapip;
    version = d.version;
    nickname = d.nickname;
    status = d.status;
    feel = d.feel;
    photo = d.photo;
    image = d.image;
    tcpenabled = d.tcpenabled;
    versioninfo = d.versioninfo;
    mac = d.mac;
    return this;
}

BroadcastJsonData::~BroadcastJsonData()
{

}

BroadcastJsonData *BroadcastJsonData::copy()
{
    BroadcastJsonData *d = new BroadcastJsonData;
    d->uid = uid;
    d->gsoapip = gsoapip;
    d->version = version;
    d->nickname = nickname;
    d->status = status;
    d->feel = feel;
    d->photo = photo;
    d->image = image;
    d->tcpenabled = tcpenabled;
    d->versioninfo = versioninfo;
    d->mac = mac;
    return d;
}

void BroadcastJsonData::clone(BroadcastJsonData &data)
{
    BroadcastJsonData &d = (BroadcastJsonData&)data;
    uid = d.uid;
    gsoapip = d.gsoapip;
    version = d.version;
    nickname = d.nickname;
    status = d.status;
    feel = d.feel;
    photo = d.photo;
    image = d.image;
    tcpenabled = d.tcpenabled;
    versioninfo = d.versioninfo;
    mac = d.mac;
}

void BroadcastJsonData::parserJson(QJsonObject obj)
{
    parserJsonRun(obj);
//    QtConcurrent::run(this, &BroadcastJsonData::parserJsonRun, obj);
}

QByteArray BroadcastJsonData::toJson()
{
//    QJsonObject json;
//    const QMetaObject *meta = this->metaObject();
//    int counts = meta->propertyCount();
//    for (int i = 0; i < counts; i++)
//    {
//        QMetaProperty ppt = meta->property(i);
//        QString key(ppt.name());
//        if (key.compare("objectName", Qt::CaseInsensitive) == 0)
//            continue;
//        QVariant var = ppt.read(this);
//        if (var.canConvert(QVariant::String))
//        {
//            json.insert(key, var.toString());
//        }
//        else if (var.canConvert(QVariant::Int))
//        {
//            json.insert(key, var.toInt());
//        }
//        else if (var.canConvert(QVariant::Bool))
//        {
//            json.insert(key, var.toBool());
//        }
//    }
    QJsonDocument doc;
    doc.setObject(toObject());
    QByteArray ba = doc.toJson(QJsonDocument::Compact);
    return ba.toBase64();
}

QJsonObject BroadcastJsonData::toObject()
{
    QJsonObject json;
    const QMetaObject *meta = this->metaObject();
    int counts = meta->propertyCount();
    for (int i = 0; i < counts; i++)
    {
        QMetaProperty ppt = meta->property(i);
        QString key(ppt.name());
        if (key.compare("objectName", Qt::CaseInsensitive) == 0)
            continue;
        QVariant var = ppt.read(this);
        if (var.canConvert(QVariant::String))
        {
            json.insert(key, var.toString());
        }
        else if (var.canConvert(QVariant::Int))
        {
            json.insert(key, var.toInt());
        }
        else if (var.canConvert(QVariant::Bool))
        {
            json.insert(key, var.toBool());
        }
    }
    return json;
}
QString BroadcastJsonData::getUid() const
{
    return uid;
}

void BroadcastJsonData::setUid(const QString &value)
{
    uid = value;
}
float BroadcastJsonData::getVersion() const
{
    return version;
}

void BroadcastJsonData::setVersion(float value)
{
    version = value;
}
QString BroadcastJsonData::getNickname() const
{
    return nickname;
}

void BroadcastJsonData::setNickname(const QString &value)
{
    if (nickname == value)
    {
        return;
    }
    nickname = value;
    emit nicknameChanged();
}
int BroadcastJsonData::getStatus() const
{
    return status;
}

void BroadcastJsonData::setStatus(int value)
{
    if (status == value)
        return;
    status = value;
    emit statusChanged();
}
QString BroadcastJsonData::getFeel() const
{
    return feel;
}

void BroadcastJsonData::setFeel(const QString &value)
{
    if (feel == value)
        return;
    feel = value;
    emit feelChanged();
}
QString BroadcastJsonData::getPhoto() const
{
    return photo;
}

void BroadcastJsonData::setPhoto(const QString &value)
{
//    if (photo == value)
//        return;
    photo = value;
    emit photoChanged();
}
QString BroadcastJsonData::getImage() const
{
    return image;
}

void BroadcastJsonData::setImage(const QString &value)
{
//    if (image == value)
//        return;
    image = value;
    emit imageChanged();
}

void BroadcastJsonData::parserJsonRun(QJsonObject obj)
{
//    QMutexLocker lock(&mutex);
    const QMetaObject *meta = this->metaObject();
    auto it = obj.begin();
    while (it != obj.end())
    {
        QString key = it.key();
        QJsonValue value = it.value();
        int index = meta->indexOfProperty(key.toLocal8Bit().data());
        if (index >= 0)
        {
            QMetaProperty ppt = meta->property(index);
            ppt.write(this, value.toVariant());
        }
        else
        {
            qDebug() << QString("no such property:%1").arg( key);
        }
        ++it;
    }
    if (version > HAPPY_VERSON)
        emit versionChanged();
    emit parserComplete();
}
QString BroadcastJsonData::getMac() const
{
    return mac;
}

void BroadcastJsonData::setMac(const QString &value)
{
    mac = value;
}

bool BroadcastJsonData::getTcpenabled() const
{
    return tcpenabled;
}

void BroadcastJsonData::setTcpenabled(bool value)
{
    tcpenabled = value;
}

QString BroadcastJsonData::getVersioninfo() const
{
    return versioninfo;
}

void BroadcastJsonData::setVersioninfo(const QString &value)
{
    versioninfo = value;
}

QString BroadcastJsonData::getGsoapip() const
{
    return gsoapip;
}

void BroadcastJsonData::setGsoapip(const QString &value)
{
    gsoapip = value;
}









