#include "messagerichtext.h"
#include "../util/thmethod.h"
#include "../config/configuredata.h"
#include <QFile>
#include <QDebug>

class MessageRichText::PrivData
{
public:
    PrivData()
    {
        html.clear();
        memset(&head, 0, sizeof(MessageRichText::RichTextHead));
    }
    ~PrivData()
    {
        imgsrclst.clear();
    }
public:
    QString html;
    QStringList imgsrclst;
    MessageRichText::RichTextHead head;
};

MessageRichText::MessageRichText()
{
    mData = new PrivData();
}

MessageRichText::~MessageRichText()
{
    if (nullptr != mData)
    {
        delete mData;
        mData = nullptr;
    }
}

void MessageRichText::setMessageHtml(QString html)
{
    mData->html = html;
}

QString MessageRichText::messageHtml()
{
    return mData->html;
}

QStringList MessageRichText::messageImagePath()
{
    return mData->imgsrclst;
}

MessageContent *MessageRichText::copy()
{
    MessageRichText *msg = new MessageRichText();
    msg->setMessageHtml(mData->html);
    return msg;
}

void MessageRichText::toStream(NetworkData &data)
{
    mData->head.htmllength = mData->html.toLocal8Bit().size();

    // 解析Html下的所有图片资源
    mData->imgsrclst = ParserHtmlImgSrc(mData->html);

    // 先塞入Html资源
    mData->head.imagecounts = mData->imgsrclst.size();
    data.appendData((char *)&mData->head, sizeof(RichTextHead));
    data.appendData(mData->html.toLocal8Bit().data(), mData->head.htmllength);

    // 依次塞入图片资源
    int vaild = 0;
    QByteArray imagebuff;
    for (int i = 0; i < mData->imgsrclst.size(); i++)
    {
        QString path = mData->imgsrclst.at(i);
        QFile file(path);
        if (file.exists())
        {
            // 文件存在
            // 获取图片的格式
            QString imagetype = "jpg";
            QFileInfo info(path);
            imagetype = info.suffix();
            ImageSrc imagesrc;
            memset(&imagesrc, 0, sizeof(ImageSrc));
            QByteArray imageTypeBuff = imagetype.toLocal8Bit();
            imagesrc.imageTypeLength = imageTypeBuff.size();
            QByteArray tempBuff;
            if (file.open(QIODevice::ReadOnly))
            {
                // 文件打开成功
                tempBuff = file.readAll();
            }
            file.close();
            imagesrc.imagelength = tempBuff.length();
            imagesrc.positon = i;
            // 成功塞入一个图片资源
            imagebuff.append((char*)&imagesrc, sizeof(ImageSrc));
            imagebuff.append(imageTypeBuff);
            imagebuff.append(tempBuff);
            // 有效数+1
            vaild++;
        }
    }
    data.appendData(imagebuff.data(), imagebuff.length());
    // 塞入结束后，需要重新调整图片有效数
    // 让第三位的数，即个数重新赋值
    mData->head.totalleng = data.length();
    mData->head.imagecounts = vaild;
    char *buff = data.data();
    memcpy(buff, &mData->head, sizeof(mData->head));
}

void MessageRichText::parserStream(NetworkData &data)
{
    // 解析获取的数据
    char *buff = data.data();
    int length = data.length();
    int pos = 0;
    // 解析头部
    memcpy(&mData->head, buff, sizeof(RichTextHead));
    pos += sizeof(RichTextHead);
    if (mData->head.totalleng != length)
    {
        mData->html = cn("解析接收到的数据长度错误");
        return;
    }

    // 得到Html字符串
    char *htmlbuff = new char[mData->head.htmllength];
    memcpy(htmlbuff, buff + pos, mData->head.htmllength);
    pos += mData->head.htmllength;
    mData->html = QString::fromLocal8Bit(htmlbuff, mData->head.htmllength);
    delete[] htmlbuff;
    htmlbuff = nullptr;

    // 解析Html里的所有图片资源个数
    QStringList srclst = ParserHtmlImgSrc(mData->html);
    if (srclst.size() < mData->head.imagecounts)
    {
        qDebug() << "error, impossible!!!!!";
        mData->html = cn("解析接收到的数据资源数据错误");
        return;
    }

    QList<ParserImage> parserLst;
    // 解析图片资源
    int sum = 0;
    for (int i = 0; i < mData->head.imagecounts; i++)
    {
        ImageSrc src;
        ParserImage parser;
        memcpy(&src, buff + pos, sizeof(ImageSrc));
        pos += sizeof(ImageSrc);
        // 解析图片格式
        char *typebuff = new char[src.imageTypeLength];
        memcpy(typebuff, buff + pos, src.imageTypeLength);
        pos += src.imageTypeLength;
        QString imageType = QString::fromLocal8Bit(typebuff, src.imageTypeLength);
        delete[] typebuff;
        typebuff = nullptr;
        // 解析图片资源
        char *imagebuff = new char[src.imagelength];
        memcpy(imagebuff, buff + pos, src.imagelength);
        pos += src.imagelength;
        // 得到的图片需要保存下来存放到工作目录下，这里需要自定义其文件名，采用UUID命名
        // TODO: 修正此处
        QString path = GetWorkPath();
        path = path + "/temp";
        QDir dir(".");
        QFileInfo info(path);
        if (!info.exists())
        {
            dir.mkpath(info.absoluteFilePath());
        }

        QByteArray ba(imagebuff, src.imagelength);
        // 通过MD5效验
        QByteArray md5ba = QCryptographicHash::hash(ba, QCryptographicHash::Md5);
        QString md5 = md5ba.toHex();
        QString name = info.absoluteFilePath() + "/" + md5 + "." + imageType;
//        QString name = md5 + "recvimage%1." + imageType;
//        name = name.arg(QString::number(src.positon));

        // 缩小图片
//        bool flag = false;
//        QPixmap pix;
//        flag = pix.loadFromData(ba);
//        QSize smallsize(std::min(400, pix.width()), std::min(450, pix.height()));
//        pix = pix.scaled(smallsize, Qt::KeepAspectRatio);
//        flag = pix.save(info.absoluteFilePath() + "/" + name);

        // 如果失败，则采用文件流方式存储
        QFile file(name);
        if (!file.exists())
        {
            if (file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                file.write(imagebuff, src.imagelength);
            }
            file.close();
        }

        parser.path = name;
        parser.position = src.positon;
        parserLst.push_back(parser);
        delete[] imagebuff;
        imagebuff = nullptr;
        sum += sizeof(ImageSrc) + src.imageTypeLength + src.imagelength;
    }
    // 检查长度是否符合长度,这里的检查只能放到最后

    int size = mData->head.htmllength + sizeof(RichTextHead) + sum;
    if (size != length)
    {
        qDebug() << "error, parser RichText data length error";
        mData->html = cn("解析接收到的数据资源效验错误");
        return;
    }

    // 获取的最新路径替换原有路径
    ResetHtmlImgSrc(mData->html, parserLst);
}


MessageType MessageRichText::datatype()
{
    return RichText;
}
