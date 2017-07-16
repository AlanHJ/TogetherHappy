#include "socketopt.h"
#include "../../data/messagedata.h"
#include "../../data/updatedata.h"
#include "../../config/configuredata.h"
#include "../../manager/filemanager.h"
#include "../../data/messagedata.h"
#include "../../data/filedatacontent.h"
#include "../../data/messagechardata.h"
#include <QFile>
#include <QThread>
#include <QTcpSocket>
#include <QApplication>

#define SEND_SIZE 1024 * 1024 * 2
SocketOpt::SocketOpt(QObject *parent) : QObject(parent)
{
    socket = nullptr;
    socketThread = new QThread;
    this->moveToThread(socketThread);
    connect(socketThread, &QThread::finished, this, [=] () {
        this->deleteLater();
        socketThread->deleteLater();
    });

    connect(this, &SocketOpt::incomingConnect,
            this, [=] (qintptr handle) {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected,
                this, [=] () {
        });
        connect(socket, &QTcpSocket::disconnected,
                this, [=] () {
            emit disconnected();
            socketThread->quit();
        });
        connect(socket, &QTcpSocket::readyRead,
                this, [=] () {
                QByteArray ba = socket->readAll();
                if (ba == "update")
                {
                    NetworkData data;
                    MessageData msg;
                    msg.setDatatype(update);
                    msg.setUid(ConfigureData::getInstance()->getUuid());
                    UpdateData *ud = new UpdateData;
                    msg.setContent(ud);
                    msg.toStream(data);
                    socket->write(data.data(), data.length());
                    socket->waitForBytesWritten();
                }
                else if (ba.contains("file"))
                {
                    QString str = QString::fromLocal8Bit(ba);
                    QStringList flags = str.split(":");
                    if (flags.size() != 2)
                    {
                        emit sendInformation(cn("验证失败,发送终止"));
                        socket->disconnectFromHost();
                        socket->abort();
                        socketThread->quit();
                        return;
                    }
                    QString uid = flags.last();
                    FileManager *fm = FileManager::GetInstance();
                    emit fm->sendRequested(uid, this);
                    socketThread->msleep(100);
                    QStringList list = fm->getQueue(uid);
                    fm->removeQueue(uid);
                    // send file
                    if (list.isEmpty())
                    {
                        emit sendInformation(cn("队列空,发送终止"));
                        socket->disconnectFromHost();
                        socket->abort();
                        socketThread->quit();
                        return;
                    }
                    // find all files
                    emit sendInformation(cn("检索文件..."));
                    {
                        MessageData msg;
                        MessageCharData *content = new MessageCharData;
                        content->setType(FileInfo);
                        content->setContent(cn("对方正在检索文件，请稍后...").toLocal8Bit());
                        msg.setContent(content);
                        msg.setDatatype(content->datatype());
                        msg.setUid(uid);
                        NetworkData nd1;
                        msg.toStream(nd1);
                        socket->write(nd1.data(), nd1.length());
                        socket->flush();
                        socket->waitForBytesWritten();
                    }

                    QList<FileStruct> structs;
                    for (QString str : list)
                    {
                        QDir dir(str);
                        QFileInfo finfo(str);
                        if (finfo.isFile())
                        {
                            FileStruct fs;
                            fs.root = finfo.absolutePath();
                            fs.path = finfo.filePath();
                            fs.name = finfo.fileName();
                            structs.append(fs);
                            continue;
                        }
                        if (!dir.exists())
                            continue;
                        dir.cdUp();
                        QDirIterator it(str, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
                        while (it.hasNext())
                        {
                            it.next();
                            QFileInfo info = it.fileInfo();
                            FileStruct fs;
                            fs.root = dir.absolutePath();
                            fs.path = info.filePath();
                            fs.name = info.fileName();
                            structs.append(fs);
                            QString infostr = cn("检索文件...%1").arg(structs.size());
                            emit sendInformation(infostr);

                            if (socketThread->isInterruptionRequested())
                            {
                                emit sendInformation(cn("发送终止"));
                                socket->abort();
                                socketThread->quit();
                                return;
                            }
                            if (socket->state() != QAbstractSocket::ConnectedState)
                            {
                                emit sendInformation(cn("断开,发送终止"));
                                socketThread->quit();
                                return;
                            }
                        }
                    }
                    // ready send
                    QTime sendTime;
                    sendTime.start();
                    qint64 sendCountPerMin = 0;
                    for (int i = 0; i < structs.size(); i++)
                    {
                        if (socketThread->isInterruptionRequested())
                        {
                            emit sendInformation(cn("发送终止"));
                            socket->abort();
                            socketThread->quit();
                            return;
                        }
                        if (socket->state() != QAbstractSocket::ConnectedState)
                        {
                            emit sendInformation(cn("断开,发送终止"));
                            socketThread->quit();
                            return;
                        }

                        FileStruct fs = structs.at(i);
                        emit sendInformation(cn("正在发送 %1").arg(fs.name));

                        QFile file(fs.path);
                        if (!file.exists())
                            continue;
                        if (!file.open(QIODevice::ReadOnly))
                            continue;
                        qint64 fileSize = file.size();
                        QDataStream stream(&file);
                        for (qint64 count = 0; count <= fileSize;)
                        {
                            if (socketThread->isInterruptionRequested())
                            {
                                emit sendInformation(cn("发送终止"));
                                file.close();
                                socket->abort();
                                socketThread->quit();
                                return;
                            }
                            char *temp = new char[SEND_SIZE];
                            int len = stream.readRawData(temp, SEND_SIZE);
                            if (len == -1)
                            {
                                break;
                            }
                            count += len;
                            MessageData msg;
                            FileDataContent *content = new FileDataContent;
                            FileDataContent::FileHead head;
                            head.total = structs.size();
                            head.currentIndex = i + 1;
                            float c1;
                            float c2;
                            c1 = head.currentIndex;
                            c2 = head.total;
                            head.totalProgress = c1 / c2 * 100;
                            c1 = (count == 0 ? 1 : count);
                            c2 = (fileSize == 0 ? 1 : fileSize);
                            head.currentProgress = c1 / c2 * 100;
                            head.fileCurrentSize = len;
                            head.fileToalSize = fileSize;
                            content->setSendInfo(head);
                            content->setFileName(fs.root, fs.path);
                            content->setData(temp);
                            msg.setDatatype(content->datatype());
                            msg.setContent(content);
                            msg.setUid(uid);

                            emit sendProgress(head.totalProgress >= 100 ? 99 : head.totalProgress,
                                              c1 / c2);

                            delete[]  temp;
                            temp = nullptr;

                            NetworkData nd;
                            msg.toStream(nd);
                            if (socket->state() != QAbstractSocket::ConnectedState)
                            {
                                emit sendInformation(cn("断开,发送终止"));
                                file.close();
                                socketThread->quit();
                                return;
                            }
                            sendCountPerMin += socket->write(nd.data(), nd.length());
                            QTime time;
                            time.start();
                            while (!socketThread->isInterruptionRequested())
                            {
                                qint64 bytes = socket->bytesToWrite();
                                socket->flush();
                                if (sendTime.elapsed() >= 1000)
                                {
                                    emit sendSpeed(sendCountPerMin / 1024.0 / 1024.0);
                                    sendTime.restart();
                                    sendCountPerMin = 0;
                                }
                                if (bytes < 512)
                                {
                                    break;
                                }
                                if (time.elapsed() > 30 * 1000)
                                {
                                    // time out
                                    emit sendInformation(cn("超时,发送终止"));
                                    file.close();
                                    socket->abort();
                                    socketThread->quit();
                                    return;
                                }
                            }
                            if (fileSize == 0 || fileSize == count)
                            {
                                break;
                            }
                        }
                        file.close();
                    }

                    // send success
                    emit sendProgress(100, 1);
                    emit sendInformation(cn("发送完成"));
                }
                else
                {
                    socket->disconnectFromHost();
                    socket->abort();
                    socketThread->quit();
                }
        });
        connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
                this, [=] () {
            emit disconnected();
            socketThread->quit();
        });
        socket->setSocketDescriptor(handle);
    });
    socketThread->start();
}

SocketOpt::~SocketOpt()
{

}

void SocketOpt::requested()
{
    socketThread->requestInterruption();
}

