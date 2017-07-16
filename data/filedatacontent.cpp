#include "filedatacontent.h"
#include <QDebug>

class FileDataContent::PrivData
{
public:
    PrivData()
    {
        data = nullptr;
        memset(&filehead, 0, sizeof(FileDataContent::FileHead));
    }
    ~PrivData()
    {
        if (nullptr != data)
        {
            delete[] data;
            data = nullptr;
        }
    }
public:
    char *data;
    FileDataContent::FileHead filehead;
    FileDataContent::FileNameHead namehead;
};

FileDataContent::FileDataContent()
{
    mData = new PrivData();
}

FileDataContent::~FileDataContent()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void FileDataContent::setSendInfo(FileDataContent::FileHead head)
{
    mData->filehead =  head;
}

void FileDataContent::setFileName(QString path, QString name)
{
    mData->namehead.pathSize = path.toLocal8Bit().size();
    mData->namehead.path = path;
    mData->namehead.nameSize = name.toLocal8Bit().size();
    mData->namehead.name = name;
}

void FileDataContent::setData(char *data)
{
    if (nullptr != mData->data)
    {
        delete mData;
        mData = nullptr;
    }
    mData->data = new char[mData->filehead.fileCurrentSize];
    memcpy(mData->data, data, mData->filehead.fileCurrentSize);
}

FileDataContent::FileHead *FileDataContent::sendInfo()
{
    return &mData->filehead;
}

FileDataContent::FileNameHead *FileDataContent::fileName()
{
    return &mData->namehead;
}

char *FileDataContent::data()
{
    return mData->data;
}

MessageType FileDataContent::datatype()
{
    return FileSendType;
}

MessageContent *FileDataContent::copy()
{
    FileDataContent *file = new FileDataContent();
    file->setSendInfo(mData->filehead);
    file->setFileName(mData->namehead.path, mData->namehead.name);
    file->setData(mData->data);
    return file;
}

void FileDataContent::toStream(NetworkData &data)
{
    data.appendData((char *)&mData->filehead, sizeof(FileHead));
    data.appendData((char *)&mData->namehead.pathSize, sizeof(qint32));
    data.appendData((char *)&mData->namehead.nameSize, sizeof(qint32));
    data.appendData(mData->namehead.path.toLocal8Bit().data(), mData->namehead.pathSize);
    data.appendData(mData->namehead.name.toLocal8Bit().data(), mData->namehead.nameSize);
    data.appendData(mData->data, mData->filehead.fileCurrentSize);
}

void FileDataContent::parserStream(NetworkData &data)
{
    // 反解析
    char *buff = data.data();
    int length = data.length();

    int index = 0;
    memcpy(&mData->filehead, buff + index, sizeof(FileHead));
    index += sizeof(FileHead);
    memcpy(&mData->namehead.pathSize, buff + index, sizeof(qint32));
    index += sizeof(qint32);
    memcpy(&mData->namehead.nameSize, buff + index, sizeof(qint32));
    index += sizeof(qint32);

    // 效验数据长度
    qint64 kLength = mData->filehead.fileCurrentSize +
            sizeof(FileHead) + sizeof(qint32) * 2 +
            mData->namehead.pathSize + mData->namehead.nameSize;
    if (kLength != length)
    {
        qDebug() << cns("文件类数据长度出现问题");
        return;
    }
    char *tempBuff = new char[mData->namehead.pathSize];
    memcpy(tempBuff, buff + index, mData->namehead.pathSize);
    index += mData->namehead.pathSize;
    mData->namehead.path = QString::fromLocal8Bit(tempBuff, mData->namehead.pathSize);
    delete tempBuff;
    tempBuff = nullptr;
    tempBuff = new char[mData->namehead.nameSize];
    memcpy(tempBuff, buff + index, mData->namehead.nameSize);
    index += mData->namehead.nameSize;
    mData->namehead.name = QString::fromLocal8Bit(tempBuff, mData->namehead.nameSize);
    delete tempBuff;
    tempBuff = nullptr;
    if (nullptr != mData->data)
    {
        delete mData->data;
        mData->data = nullptr;
    }
    mData->data = new char[mData->filehead.fileCurrentSize];
    memcpy(mData->data, buff + index, mData->filehead.fileCurrentSize);
    index += mData->filehead.fileCurrentSize;
}
