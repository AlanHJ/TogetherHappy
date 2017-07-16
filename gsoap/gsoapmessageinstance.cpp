#include "gsoapmessageinstance.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QFile>

static GsoapMessageInstance *GsoapMsgInstance = nullptr;
class GsoapMessageInstance::PrivData
{
public:
    PrivData()
    {

    }
    ~PrivData()
    {

    }
public:

};

GsoapMessageInstance::GsoapMessageInstance()
{
    mData = new PrivData();
    qRegisterMetaType<GroupData>("GroupData");
}

GsoapMessageInstance::~GsoapMessageInstance()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

GsoapMessageInstance *GsoapMessageInstance::GetInstance()
{
    if (nullptr == GsoapMsgInstance)
    {
        GsoapMsgInstance = new GsoapMessageInstance();
    }
    return GsoapMsgInstance;
}

void GsoapMessageInstance::DesInstance()
{
    if (nullptr != GsoapMsgInstance)
    {
        delete GsoapMsgInstance;
        GsoapMsgInstance = nullptr;
    }
}

int GsoapMessageInstance::UpdateFile(QString v, std::wstring *str)
{
    QByteArray ba;
    // 查询版本
    if (v == QString::number(HAPPY_VERSON))
    {
        ba.append("equal");
        *str = QByteArrayCode(ba).toStdWString();
        return -1;
    }
    // 获取本地哈啤文件
    QString path = GetWorkPath();
    QFile file(path);
    if (file.exists() == false)
    {
        ba.append("fail");
        *str = QByteArrayCode(ba).toStdWString();
        return -1;
    }
    if (file.open(QIODevice::ReadOnly) == false)
    {
        ba.append("fail");
        *str = QByteArrayCode(ba).toStdWString();
        return -1;
    }
    ba = file.readAll();
    *str = QByteArrayCode(ba).toStdWString();
    file.close();
    return 0;
}

