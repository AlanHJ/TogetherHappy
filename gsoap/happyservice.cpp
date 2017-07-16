#include "stdsoap2.h"
#include "soapService.h"
#include "../data/messagedata.h"
#include "gsoapmessageinstance.h"
#include "../data/messagerichtext.h"
#include "../data/groupinvitedata.h"
#include "../data/messagechardata.h"
#include <QString>
#include <QDebug>
#include <QDir>
#include <QHostAddress>
#include <QCryptographicHash>
#include "call/gsoapcalloperation.h"

#define GSOAP_FAIL   -2
int Service::ChatOneByOne(std::wstring key, std::wstring content, std::wstring *result)
{
    QString k = QString::fromStdWString(key);
    QByteArray ba = WStringDecode(content);
    char *buff = ba.data();
    int length = ba.length();
    MessageData::MessageHead head;
    int headLength = sizeof(MessageData::MessageHead);
    memcpy(&head, buff, headLength);
    char *uidbuff = new char[head.uidlength];
    memcpy(uidbuff, buff + headLength, head.uidlength);
    QString uid = QString::fromLocal8Bit(uidbuff, head.uidlength);
    delete[] uidbuff;
    uidbuff = nullptr;

    QString str;
    GsoapMessageInstance *recever = GsoapMessageInstance::GetInstance();
    if (head.length != (length - sizeof(qint32)))
    {
        str = cns("长度错误");
        *result = str.toStdWString();
        return GSOAP_FAIL;
    }
    NetworkData ndata(buff + headLength + head.uidlength,
                      length - headLength - head.uidlength);
    if (head.type == RichText)
    {
        MessageRichText rich;
        rich.parserStream(ndata);
        // 发送消息到UI
        QString ip = QHostAddress(this->ip).toString();
        emit recever->signalRecvChat(k, ip, rich.messageHtml());
        return SOAP_OK;
    }
    else if (head.type == Invite)
    {
        GroupInviteData invite;
        invite.parserStream(ndata);
        emit recever->signalInvite(invite.groupData());
        return SOAP_OK;
    }
    else if (head.type == commonStr)
    {
        MessageCharData cd;
        cd.parserStream(ndata);
        QString ip = QHostAddress(this->ip).toString();
        emit recever->signalCommonStr(k, ip, cd.type(), QString::fromLocal8Bit(cd.content()));
        return SOAP_OK;
    }
    else
    {
        MessageData msg;
        msg.setDatatype((MessageType)head.type);
        msg.setUid(uid);
        emit recever->signalOtherCmd(head.type, msg);
        return SOAP_OK;
    }
}

int Service::ChatGroup(std::wstring gkey, std::wstring key, std::wstring content, std::wstring *result)
{
    QString gk = QString::fromStdWString(gkey);
    QString k = QString::fromStdWString(key);
    QByteArray ba = WStringDecode(content);
    char *buff = ba.data();
    int length = ba.length();
    MessageData::MessageHead head;
    int headLength = sizeof(MessageData::MessageHead);
    memcpy(&head, buff, headLength);
    QString str;
    if (head.type != RichText)
    {
        str = cns("协议错误");
        *result = str.toStdWString();
        return GSOAP_FAIL;
    }
    if (head.length != (length - sizeof(qint32)))
    {
        str = cns("长度错误");
        *result = str.toStdWString();
        return GSOAP_FAIL;
    }
    NetworkData ndata(buff + headLength, length - headLength);
    MessageRichText rich;
    rich.parserStream(ndata);
    // 发送消息到UI
    emit GsoapMessageInstance::GetInstance()->signalRecvGroup(gk, k, rich.messageHtml());
    return SOAP_OK;
}

int Service::ChatShake(std::wstring key, std::wstring content, std::wstring *result)
{
    QString srcuid = QString::fromStdWString(key);
    QString ip = QHostAddress(this->ip).toString();
    emit GsoapMessageInstance::GetInstance()->signalShakeChat(srcuid, ip);
    Q_UNUSED(content)
    Q_UNUSED(result)
    return SOAP_OK;
}

int Service::GetResource(std::wstring srcuid, std::wstring srcuip, std::wstring type, std::wstring resource, int *len)
{
    QString uid = QString::fromStdWString(srcuid);
    QString ip = QString::fromStdWString(srcuip);
    QString t = QString::fromStdWString(type);
    QString res = QString::fromStdWString(resource);
    GsoapCallOperation *sender = GsoapCallOperation::Sender();
    emit sender->signalSetResource(uid, ip, t, res);
    *len = 0;
    return SOAP_OK;
}

int Service::SetResource(std::wstring desuid, std::wstring type,
                         std::wstring resource, std::wstring resname, int *len)
{
    QString uid = QString::fromStdWString(desuid);
    QString t = QString::fromStdWString(type);
    QString res = QString::fromStdWString(resname);
    QByteArray ba = WStringDecode(resource);
    QImage image;
    if (!image.loadFromData(ba))
    {
        *len = 0;
        return SOAP_OK;
    }
    QString md5 = QCryptographicHash::hash(ba, QCryptographicHash::Md5)
            .toHex();
    QString path = GetWorkPath() + "/temp";
    if (QDir().mkpath(path))
    {
        QString name = path + "/" + QFileInfo(res).fileName();
        QFile file(name);
        if (file.exists())
        {
            QByteArray ba1;
            if (file.open(QIODevice::ReadOnly))
            {
                ba1 = file.readAll();
            }
            file.close();
            QString md51 = QCryptographicHash::hash(ba1, QCryptographicHash::Md5)
                    .toHex();
            if (md5 != md51)
            {
                if (file.remove() && file.open(QIODevice::WriteOnly))
                {
                    file.write(ba);
                }
                file.close();
            }
        }
        else
        {
           if (file.open(QIODevice::WriteOnly))
           {
               file.write(ba);
           }
           file.close();
        }
        emit GsoapMessageInstance::GetInstance()->signalGetResource(uid, t, name);
        QString ip = QHostAddress(this->ip).toString();
//        qDebug() << "recv res:" << uid << " name:" << name << " ip" << ip;
    }
    *len = 0;
    return SOAP_OK;
}

int Service::SendFileExec(std::wstring key, std::wstring content, std::wstring *result)
{
    QString k = QString::fromStdWString(key);
    QString c = QString::fromStdWString(content);
    emit GsoapMessageInstance::GetInstance()->signalFileExec(k, c);
    Q_UNUSED(result)
    return SOAP_OK;
}

int Service::SendFileAccept(std::wstring key, std::wstring content, std::wstring *result)
{
    QString k = QString::fromStdWString(key);
    QString c = QString::fromStdWString(content);
    emit GsoapMessageInstance::GetInstance()->signalFileAccept(k, c);
    Q_UNUSED(result)
    return SOAP_OK;
}

int Service::UpdateHappy(std::wstring key, std::wstring content, std::wstring *result)
{
    // 发送哈啤文件
    QString k = QString::fromStdWString(key);
    QString v = QString::fromStdWString(content);
    emit GsoapMessageInstance::GetInstance()->signalUpdateToMe(k, v);
    int ret = GsoapMessageInstance::GetInstance()->UpdateFile(v, result);
    QString str;
    if (ret != 0)
    {
        str = QString::fromLocal8Bit(WStringDecode(*result));
    }
    else
    {
        str = "success!";
    }
    emit GsoapMessageInstance::GetInstance()->signalWhoUpdate(k, str);
    return SOAP_OK;
}

int Service::InviteGroup(std::wstring builder, std::wstring title, std::wstring gkey, std::wstring members, std::wstring *result)
{
    QString b = QString::fromUtf8(WStringDecode(builder));
    QString t = QString::fromUtf8(WStringDecode(title));
    QString k = QString::fromStdWString(gkey);
    QString m = QString::fromUtf8(WStringDecode(members));
    emit GsoapMessageInstance::GetInstance()->signalInviteGroup(b, t, k, m);
    Q_UNUSED(result)
    return SOAP_OK;
}

int Service::InviteGroupRespond(std::wstring key, std::wstring gkey, std::wstring content, std::wstring *result)
{
    QString k = QString::fromStdWString(key);
    QString gk = QString::fromStdWString(gkey);
    QString c = QString::fromStdWString(content);
    emit GsoapMessageInstance::GetInstance()->signalGroupRespond(k, gk, c);
    Q_UNUSED(result)
    return SOAP_OK;
}

int Service::UpdateGroupUser(std::wstring gkey, std::wstring newusers, std::wstring *result)
{
    Q_UNUSED(result)
    return SOAP_OK;
}
