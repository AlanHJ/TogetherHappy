#ifndef FILEDATACONTENT_H
#define FILEDATACONTENT_H

#include "messagecontent.h"

class FileDataContent : public MessageContent
{
public:
#pragma pack(push)
#pragma pack(1)
    struct FileHead
    {
        qint64 total;           // 本次发送文件总个数
        qint64 currentIndex;    // 当前第几个
        int totalProgress;    // 总进度
        int currentProgress;  // 当前文件进度
        qint64 fileToalSize;    // 当前文件总大小
        qint64 fileCurrentSize; // 当前文件发送大小
    };
#pragma pack(pop)
    struct FileNameHead
    {
        qint32 pathSize;
        QString path;
        qint32 nameSize;
        QString name;
    };

public:
    FileDataContent();
    virtual~FileDataContent();
    void setSendInfo(FileHead head);
    void setFileName(QString path, QString name);
    void setData(char *data);
    FileHead *sendInfo();
    FileNameHead *fileName();
    char *data();
    // MessageContent interface
public:
    virtual MessageType datatype();
    virtual MessageContent *copy();
    virtual void toStream(NetworkData &data);
    virtual void parserStream(NetworkData &data);
private:
    class PrivData;
    PrivData *mData;
};

#endif // FILEDATACONTENT_H
