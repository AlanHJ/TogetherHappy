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
        qint64 total;           // ���η����ļ��ܸ���
        qint64 currentIndex;    // ��ǰ�ڼ���
        int totalProgress;    // �ܽ���
        int currentProgress;  // ��ǰ�ļ�����
        qint64 fileToalSize;    // ��ǰ�ļ��ܴ�С
        qint64 fileCurrentSize; // ��ǰ�ļ����ʹ�С
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
