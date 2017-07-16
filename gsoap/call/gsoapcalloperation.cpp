#include <QThread>
#include "../../config/configuredata.h"
#include "../soapProxy.h"
#include "../happy.nsmap"
#include "gsoapcalloperation.h"
#include <QFile>
#include <QDir>
#include <QFileinfo>

GsoapCallOperation* GsoapCallOperation::soapsender = nullptr;
class GsoapCallOperation::PrivData
{
public:
    PrivData()
    {
        myUid = ConfigureData::getInstance()->getUuid();
        gsoap = new Proxy();
        soap_set_mode(gsoap, SOAP_C_MBSTRING);
        gsoap->recv_timeout = 7;
        gsoap->send_timeout = 7;
        gsoap->accept_timeout = 5;
        gsoap->connect_timeout = 5;
        thread = new QThread();
        thread->setObjectName("Gsoap interface thread");
    }
    ~PrivData()
    {
        errorIps.clear();
    }
public:
    Proxy *gsoap;
    QString myUid;
    QThread *thread;
    QMutex mutex;
    QHash<QString, uint> errorIps;
};

GsoapCallOperation::GsoapCallOperation()
{
    mData = new PrivData();
    this->moveToThread(mData->thread);
    connect(mData->thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(mData->thread, SIGNAL(finished()), mData->thread, SLOT(deleteLater()));

    connect(this, &GsoapCallOperation::signalChatOneInterface,
            this, &GsoapCallOperation::slotChatOneInterface);

    connect(this, SIGNAL(signalChatGroupInerface(QStringList,QString,MessageData)),
            this, SLOT(slotChatGroupInerface(QStringList,QString,MessageData)));

    connect(this, &GsoapCallOperation::signalChatShakeInterface,
            this, &GsoapCallOperation::slotChatShakeInterface);

    connect(this, SIGNAL(signalSendFileExec(QString,QString)),
            this, SLOT(slotSendFileExec(QString,QString)));

    connect(this, SIGNAL(signalSendFileAccept(QString,QString)),
            this, SLOT(slotSendFileAccept(QString,QString)));

    connect(this, SIGNAL(signalUpdateHappy()),
            this, SLOT(slotUpdateHappy()));

    connect(this, SIGNAL(signalInviteGroup(QStringList,QString,QString,QString,QString)),
            this, SLOT(slotInviteGroup(QStringList,QString,QString,QString,QString)));
    connect(this, SIGNAL(signalGroupRespond(QStringList,QString,QString,QString)),
            this, SLOT(slotGroupRespond(QStringList,QString,QString,QString)));

    connect(this, &GsoapCallOperation::signalSetResource,
            this, &GsoapCallOperation::slotSetResource);

    connect(this, &GsoapCallOperation::signalGetResource,
            this, &GsoapCallOperation::slotGetResource);
}

GsoapCallOperation::~GsoapCallOperation()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

GsoapCallOperation *GsoapCallOperation::Sender()
{
    if (!soapsender)
    {
        soapsender = new GsoapCallOperation;
    }
    return soapsender;
}

void GsoapCallOperation::Destruct()
{
    if (soapsender)
    {
        soapsender->stopCall();
    }
}

void GsoapCallOperation::setMyUid(QString id)
{
    mData->myUid = id;
}


void GsoapCallOperation::startCall()
{
    mData->thread->start();
}


void GsoapCallOperation::stopCall()
{
    mData->thread->quit();
    mData->thread->wait();
}


void GsoapCallOperation::slotChatOneInterface(QString desuid, QString desip, MessageData msg)
{
    QString http = "http://" + desip + ":" + QString::number(HAPPY_GOASP_PORT);
    std::wstring result;
    NetworkData data;
    msg.toStream(data);
    std::wstring content;
    QByteArray ba(data.data(), data.length());
    content = QByteArrayCode(ba).toStdWString();
    int ret = mData->gsoap->ChatOneByOne(http.toLocal8Bit().constData(), "",
                                         mData->myUid.toStdWString(), content, &result);
    slotGsoapError(desuid, ret, result);
}


void GsoapCallOperation::slotChatGroupInerface(QStringList keys, QString gkey, MessageData msg)
{
    QString http = "http://%1";
    std::wstring result;
    NetworkData data;
    msg.toStream(data);
    std::wstring content;
    QByteArray ba(data.data(), data.length());
    content = QByteArrayCode(ba).toStdWString();
    int counts = 0;
    for (QString key : keys)
    {
        QString h = http.arg(key);
        int ret = mData->gsoap->ChatGroup(h.toLocal8Bit().constData(), "",
                                          gkey.toStdWString(), mData->myUid.toStdWString(), content, &result);
        if (ret != SOAP_OK)
        {
            counts++;
        }
    }
    if (counts != 0)
    {
        QString error;
        if (counts == keys.size())
        {
            error = cns("错误，群消息发送失败");
        }
        else if (counts < keys.size())
        {
            error = cns("警告:发送群消息不完全成功");
        }
        emit signalChatMsgSendFail("all", error);
    }
}


void GsoapCallOperation::slotChatShakeInterface(QString desuid, QString desip)
{
    QString http = "http://" + desip + ":" + QString::number(HAPPY_GOASP_PORT);;
    std::wstring result;
    int ret = mData->gsoap->ChatShake(http.toLocal8Bit().constData(), "",
                                         mData->myUid.toStdWString(), QString().toStdWString(), &result);
    slotGsoapError(desuid, ret, result);
}


void GsoapCallOperation::slotSendFileExec(QString key, QString content)
{
    QString http = "http://" + key;
    std::wstring result;
    int ret = mData->gsoap->SendFileExec(http.toLocal8Bit().constData(), "",
                                         mData->myUid.toStdWString(), content.toStdWString(),
                                         &result);
    slotGsoapError(key, ret, result);
}


void GsoapCallOperation::slotSendFileAccept(QString key, QString content)
{
    QString http = "http://" + key;
    std::wstring result;
    int ret = mData->gsoap->SendFileAccept(http.toLocal8Bit().constData(), "",
                                         mData->myUid.toStdWString(), content.toStdWString(),
                                         &result);
    slotGsoapError(key, ret, result);
}

void GsoapCallOperation::slotUpdateHappy()
{
    QString http = "http://192.168.2.51:24689";
    std::wstring result;
    std::wstring content;
    QString v = QString::number(HAPPY_VERSON);
    content = v.toStdWString();
    int ret = mData->gsoap->UpdateHappy(http.toLocal8Bit().constData(), "",
                                        mData->myUid.toStdWString(), content, &result);
    if (ret != SOAP_OK)
    {
        QString error;
        error.append(cns("错误:"));
        if (ret == -1)
        {
            error.append(cns("更新失败"));
            emit signalUpdateInfo(error);
        }
        if (ret == -2)
        {
            QString temp = QString::fromStdWString(result);
            error.append(temp);
        }
        if (mData->gsoap->error)
        {
            QString temp = QString::fromLocal8Bit(mData->gsoap->soap_fault_string());
            error.append(temp);
        }
        emit signalUpdateInfo(error);
        return;
    }
    QByteArray ba = WStringDecode(result);
    if (ba == "fail")
    {
        emit signalUpdateInfo(cns("更新失败"));
    }
    else if (ba == "equal")
    {
        emit signalUpdateInfo(cns("已经是最新版本了，无需更新"));
    }
    else
    {
        QString path = GetWorkPath();
//        ConfigureData::getInstance()->getIni("path", path);
        QDir dir(path);
        QFileInfo info(path);
        if (info.isDir() == false)
        {
            emit signalUpdateInfo(cns("更新失败,未找到安置文件夹"));
            return;
        }
        path += "/update/";
        if (dir.exists(path) == false)
        {
            dir.mkdir(path);
        }

        QFile file(path + cns("TogetherHappy.exe"));
        if (file.exists())
        {
            file.remove();
        }
        if (!file.open(QIODevice::WriteOnly))
        {
            emit signalUpdateInfo(cns("更新失败,文件无法写入"));
            return;
        }
        file.write(ba);
//        ConfigureData::getInstance()->writeIni("update", path + "TogetherHappy.exe");
        file.close();
        emit signalUpdateInfo(cns("更新成功"));
    }
}

void GsoapCallOperation::slotInviteGroup(QStringList lst, QString buider, QString title, QString gkey, QString members)
{
    QString http = "http://%1";
    std::wstring result;
    std::wstring b;
    std::wstring t;
    std::wstring m;
    b = QByteArrayCode(buider.toUtf8()).toStdWString();
    t = QByteArrayCode(title.toUtf8()).toStdWString();
    m = QByteArrayCode(members.toUtf8()).toStdWString();
    for (QString key : lst)
    {
        QString h = http.arg(key);
        int ret = mData->gsoap->InviteGroup(h.toLocal8Bit().data(), "",
                                            b, t,
                                            gkey.toStdWString(), m, &result);
        if (ret != SOAP_OK)
        {
            QString error = cns("错误，邀请:") + key + cns("信息发送失败");
            emit signalGroupSendResult(gkey, error);
        }
    }
}

void GsoapCallOperation::slotGroupRespond(QStringList keys, QString key, QString gkey, QString content)
{
    QString http = "http://%1";
    std::wstring result;
    for (QString key : keys)
    {
        QString h = http.arg(key);
        int ret = mData->gsoap->InviteGroupRespond(h.toLocal8Bit().constData(), "",
                                                   key.toStdWString(), gkey.toStdWString(),
                                                   content.toStdWString(), &result);
        if (ret != SOAP_OK)
        {
            QString error = cns("错误，邀请:") + key + cns("信息发送失败");
            emit signalGroupSendResult(gkey, error);
        }
    }
}


void GsoapCallOperation::slotGsoapError(QString key, int result, std::wstring str)
{
    if (result != SOAP_OK)
    {
        QString error;
        error.append(cns("错误:"));
        if (result == -1)
        {
            error.append(cns("发送信息失败\n"));
            emit signalGsoapFail(key, error);
        }
        if (result == -2)
        {
            QString temp = QString::fromStdWString(str);
            error.append(temp + "\n");
        }
        if (mData->gsoap->error)
        {
            QString temp = QString::fromLocal8Bit(mData->gsoap->soap_fault_string());
            error.append(temp + "\n");
        }
        emit signalChatMsgSendFail(key, error);
    }
}

void GsoapCallOperation::slotGetResource(QString srcuid, QString desip, QString type, QString res)
{
    if (srcuid.isEmpty())
        return;
    if (mData->errorIps.contains(desip))
    {
        int t = mData->errorIps.value(desip);
        if (t > 2)
        {
            uint ct = QDateTime::currentDateTime().toTime_t();
//            qDebug() << "over time:" << ct - t;
            if (ct - t > 60 * 5)
            {
                mData->errorIps.remove(desip);
            }
            else
            {
                return;
            }
        }
    }
    QTime time;
    time.start();
    QString srcip = ConfigureData::getInstance()->getIni("ip");
    QString http = "http://" + desip + ":" + QString::number(HAPPY_GOASP_PORT);
    int soaprlt = 0;
    int ret = mData->gsoap->GetResource(http.toLocal8Bit().constData(), "",
                                        srcuid.toStdWString(), srcip.toStdWString(),
                                        type.toStdWString(), res.toStdWString(), &soaprlt);
    int ms = time.elapsed();
    if (ms > 5000)
    {
        int i = 0;
        if (mData->errorIps.contains(desip))
        {
            i = mData->errorIps.value(desip);
        }
        i++;
        if (i > 2)
        {
            mData->errorIps.insert(desip, QDateTime::currentDateTime().toTime_t());
        }
        else
        {
            mData->errorIps.insert(desip, i);
        }
    }
//    qDebug() << "get res:" << desip << " time out:" << ms;
    Q_UNUSED(ret)
}

void GsoapCallOperation::slotSetResource(QString desuid, QString desip, QString type, QString res)
{
    if (desuid.isEmpty())
        return;
    if (mData->errorIps.contains(desip))
    {
        int t = mData->errorIps.value(desip);
        if (t > 2)
        {
            uint ct = QDateTime::currentDateTime().toTime_t();
            if (ct - t > 60 * 5)
            {
                mData->errorIps.remove(desip);
            }
            else
            {
                return;
            }
        }
    }
    QString http = "http://" + desip + ":" + QString::number(HAPPY_GOASP_PORT);
    int rlt = 0;
    std::wstring content;
    QFile file(res);
    if (!file.exists())
        return;
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTime time;
    time.start();
    QByteArray ba = file.readAll();
    file.close();
    content = QByteArrayCode(ba).toStdWString();
    int ret = mData->gsoap->SetResource(http.toLocal8Bit().constData(), "",
                                        desuid.toStdWString(), type.toStdWString(),
                                        content, res.toStdWString(), &rlt);
    int ms = time.elapsed();
    if (ms > 5000)
    {
        int i = 0;
        if (mData->errorIps.contains(desip))
        {
            i = mData->errorIps.value(desip);
        }
        i++;
        if (i > 2)
        {
            mData->errorIps.insert(desip, QDateTime::currentDateTime().toTime_t());
        }
        else
        {
            mData->errorIps.insert(desip, i);
        }
    }
//    qDebug() << "set res:" << desip << " time out:" << ms;
    Q_UNUSED(ret)
}
