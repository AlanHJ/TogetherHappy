#include "happyserver.h"
#include "clientthread.h"
#include "../util/thmethod.h"
#include "QByteArray"

HappyServer::HappyServer(QObject *parent) : QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
    quint16 port = HAPPY_LISTEN_PORT;
    listen(QHostAddress::Any, port);
}

HappyServer::~HappyServer()
{

}

void HappyServer::incomingConnection(qintptr handle)
{
    ClientThread *thread = new ClientThread();
    emit thread->setSocketDescriptor(handle);
    emit newConnectionChat(thread);
}
