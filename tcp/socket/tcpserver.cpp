#include "tcpserver.h"
#include "socketopt.h"
#include "../../util/thmethod.h"

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
}

TcpServer::~TcpServer()
{

}

void TcpServer::incomingConnection(qintptr handle)
{
    SocketOpt *opt = new SocketOpt;
    emit opt->incomingConnect(handle);
}
