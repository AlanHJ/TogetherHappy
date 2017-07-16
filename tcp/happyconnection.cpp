#include "happyconnection.h"
#include <QHostAddress>
#include <QTimerEvent>

HappyConnection::HappyConnection(QObject *parent): QTcpSocket(parent)
{

}

HappyConnection::~HappyConnection()
{

}

