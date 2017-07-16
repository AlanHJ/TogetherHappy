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

    // ����Html�µ�����ͼƬ��Դ
    mData->imgsrclst = ParserHtmlImgSrc(mData->html);

    // ������Html��Դ
    mData->head.imagecounts = mData->imgsrclst.size();
    data.appendData((char *)&mData->head, sizeof(RichTextHead));
    data.appendData(mData->html.toLocal8Bit().data(), mData->head.htmllength);

    // ��������ͼƬ��Դ
    int vaild = 0;
    QByteArray imagebuff;
    for (int i = 0; i < mData->imgsrclst.size(); i++)
    {
        QString path = mData->imgsrclst.at(i);
        QFile file(path);
        if (file.exists())
        {
            // �ļ�����
            // ��ȡͼƬ�ĸ�ʽ
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
                // �ļ��򿪳ɹ�
                tempBuff = file.readAll();
            }
            file.close();
            imagesrc.imagelength = tempBuff.length();
            imagesrc.positon = i;
            // �ɹ�����һ��ͼƬ��Դ
            imagebuff.append((char*)&imagesrc, sizeof(ImageSrc));
            imagebuff.append(imageTypeBuff);
            imagebuff.append(tempBuff);
            // ��Ч��+1
            vaild++;
        }
    }
    data.appendData(imagebuff.data(), imagebuff.length());
    // �����������Ҫ���µ���ͼƬ��Ч��
    // �õ���λ���������������¸�ֵ
    mData->head.totalleng = data.length();
    mData->head.imagecounts = vaild;
    char *buff = data.data();
    memcpy(buff, &mData->head, sizeof(mData->head));
}

void MessageRichText::parserStream(NetworkData &data)
{
    // ������ȡ������
    char *buff = data.data();
    int length = data.length();
    int pos = 0;
    // ����ͷ��
    memcpy(&mData->head, buff, sizeof(RichTextHead));
    pos += sizeof(RichTextHead);
    if (mData->head.totalleng != length)
    {
        mData->html = cn("�������յ������ݳ��ȴ���");
        return;
    }

    // �õ�Html�ַ���
    char *htmlbuff = new char[mData->head.htmllength];
    memcpy(htmlbuff, buff + pos, mData->head.htmllength);
    pos += mData->head.htmllength;
    mData->html = QString::fromLocal8Bit(htmlbuff, mData->head.htmllength);
    delete[] htmlbuff;
    htmlbuff = nullptr;

    // ����Html�������ͼƬ��Դ����
    QStringList srclst = ParserHtmlImgSrc(mData->html);
    if (srclst.size() < mData->head.imagecounts)
    {
        qDebug() << "error, impossible!!!!!";
        mData->html = cn("�������յ���������Դ���ݴ���");
        return;
    }

    QList<ParserImage> parserLst;
    // ����ͼƬ��Դ
    int sum = 0;
    for (int i = 0; i < mData->head.imagecounts; i++)
    {
        ImageSrc src;
        ParserImage parser;
        memcpy(&src, buff + pos, sizeof(ImageSrc));
        pos += sizeof(ImageSrc);
        // ����ͼƬ��ʽ
        char *typebuff = new char[src.imageTypeLength];
        memcpy(typebuff, buff + pos, src.imageTypeLength);
        pos += src.imageTypeLength;
        QString imageType = QString::fromLocal8Bit(typebuff, src.imageTypeLength);
        delete[] typebuff;
        typebuff = nullptr;
        // ����ͼƬ��Դ
        char *imagebuff = new char[src.imagelength];
        memcpy(imagebuff, buff + pos, src.imagelength);
        pos += src.imagelength;
        // �õ���ͼƬ��Ҫ����������ŵ�����Ŀ¼�£�������Ҫ�Զ������ļ���������UUID����
        // TODO: �����˴�
        QString path = GetWorkPath();
        path = path + "/temp";
        QDir dir(".");
        QFileInfo info(path);
        if (!info.exists())
        {
            dir.mkpath(info.absoluteFilePath());
        }

        QByteArray ba(imagebuff, src.imagelength);
        // ͨ��MD5Ч��
        QByteArray md5ba = QCryptographicHash::hash(ba, QCryptographicHash::Md5);
        QString md5 = md5ba.toHex();
        QString name = info.absoluteFilePath() + "/" + md5 + "." + imageType;
//        QString name = md5 + "recvimage%1." + imageType;
//        name = name.arg(QString::number(src.positon));

        // ��СͼƬ
//        bool flag = false;
//        QPixmap pix;
//        flag = pix.loadFromData(ba);
//        QSize smallsize(std::min(400, pix.width()), std::min(450, pix.height()));
//        pix = pix.scaled(smallsize, Qt::KeepAspectRatio);
//        flag = pix.save(info.absoluteFilePath() + "/" + name);

        // ���ʧ�ܣ�������ļ�����ʽ�洢
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
    // ��鳤���Ƿ���ϳ���,����ļ��ֻ�ܷŵ����

    int size = mData->head.htmllength + sizeof(RichTextHead) + sum;
    if (size != length)
    {
        qDebug() << "error, parser RichText data length error";
        mData->html = cn("�������յ���������ԴЧ�����");
        return;
    }

    // ��ȡ������·���滻ԭ��·��
    ResetHtmlImgSrc(mData->html, parserLst);
}


MessageType MessageRichText::datatype()
{
    return RichText;
}
