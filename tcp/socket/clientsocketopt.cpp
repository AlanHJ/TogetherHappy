#include "clientsocketopt.h"
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QTcpSocket>
#include "../../data/filedatacontent.h"
#include "../../tcp/tcpdatadeal.h"
#include "../../tcp/messagedatadeal.h"
#include "../../data/messagechardata.h"

ClientSocketOpt::ClientSocketOpt(QObject *parent) : QObject(parent)
{
    timerid = -1;
    recvCounts = 0;
    socket = nullptr;
    socketThread = new QThread;
    socketThread->setObjectName("client socket thread");
    this->moveToThread(socketThread);

    connect(socketThread, &QThread::finished, this, [=] () {
        this->deleteLater();
        socketThread->deleteLater();
    });

    connect(this, &ClientSocketOpt::connectHost,
            this, [=] (QString ip, quint16 port) {
        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::connected,
                this, [=] () {
            // connected
            emit connected();
        });
        socket->connectToHost(ip, port);
        if (!socket->waitForConnected())
        {
            emit recvInformation(cn("接收终止"));
            emit disconnected();
            socketThread->quit();
            return;
        }


        QFile *file = new QFile(this);
        TcpDataDeal *deal = new TcpDataDeal(this);
        MessageDataDeal *msg = new MessageDataDeal(this);
        connect(deal, &TcpDataDeal::fullFrameData,
                msg, &MessageDataDeal::ParserFrameData);

        connect(msg, &MessageDataDeal::recvMessageData,
                this, [=] (MessageData data) {
            if (data.datatype() != FileSendType &&
                data.datatype() != commonStr)
                return;
            if (data.datatype() == commonStr)
            {
                MessageCharData *charconent = dynamic_cast<MessageCharData*>(data.content());
                if (charconent && charconent->type() == FileInfo)
                {
                    QString strinfo = QString::fromLocal8Bit(charconent->content());
                    emit recvInformation(strinfo);
                }
                return;
            }
            // write file
            FileDataContent *content = dynamic_cast<FileDataContent*>(data.content());
            if (!content)
                return;
            FileDataContent::FileHead *head = content->sendInfo();
            FileDataContent::FileNameHead *namehead = content->fileName();
            emit recvProgress(head->totalProgress >= 100 ? 99 : head->totalProgress, head->currentProgress / 100.0);
            QString filename = namehead->name.replace(namehead->path, savePath + "/");
            QFileInfo info(filename);
            emit recvInformation(cn("正在接收 %1").arg(info.fileName()));
            QDir dir(info.absolutePath());
            if (!dir.exists())
                dir.mkpath(info.absolutePath());
            if (file->fileName() != filename)
            {
                file->close();
                if (info.exists())
                {
                    QString dt = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                    filename = info.absolutePath() + "/" +
                            info.bundleName() + dt + "." + info.suffix();
                }
                file->setFileName(filename);
            }
            if (!file->isOpen())
            {
                file->open(QIODevice::WriteOnly | QIODevice::Append);
            }
            if (file->isWritable())
            {
                file->write(content->data(), head->fileCurrentSize);
                if (head->currentProgress == 100)
                    file->close();
            }
            if (socketThread->isInterruptionRequested())
            {
                emit recvInformation(cn("接收终止"));
                file->close();
                socket->disconnectFromHost();
                socket->abort();
                socketThread->quit();
            }
            if (head->currentProgress == 100 && head->totalProgress == 100)
            {
                file->close();
                emit recvProgress(100, 1);
                emit recvInformation(cn("接收完成"));
                emit disconnected();
                socket->disconnectFromHost();
                socket->abort();
                socketThread->quit();
            }
        });

        connect(socket, &QTcpSocket::disconnected,
                this, [=] () {
            // disconnected
        });
        connect(socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
                this, [=] () {
            emit recvInformation(cn("接收终止"));
            emit disconnected();
            socketThread->quit();
        });
        connect(this, &ClientSocketOpt::senddata,
                this, [=] (QByteArray ba) {
            if (socket->state() != QAbstractSocket::ConnectedState)
                return;
            socket->write(ba);
            socket->waitForBytesWritten();
            socket->flush();
        });
        connect(socket, &QTcpSocket::readyRead,
                this, [=] () {
            // read data
            QByteArray ba = socket->readAll();
            recvCounts += ba.length();
            NetworkData nd(ba.data(), ba.length());
            deal->slotPosData(nd);
        });

        timerid = startTimer(1000);
    });
    socketThread->start();
}

ClientSocketOpt::~ClientSocketOpt()
{

}
QString ClientSocketOpt::getSavePath() const
{
    return savePath;
}

void ClientSocketOpt::setSavePath(const QString &value)
{
    savePath = value;
}

void ClientSocketOpt::recvRequested()
{
//    emit socketRequest();
    socketThread->requestInterruption();
}


void ClientSocketOpt::timerEvent(QTimerEvent *e)
{
    if (e->timerId() != timerid)
        return;
    emit recvSpeed(recvCounts / 1024.0 / 1024.0);
    recvCounts = 0;
}

