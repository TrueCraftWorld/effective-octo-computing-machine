#include "networklayer.h"

NetworkLayer::NetworkLayer(QObject *parent)
    : QObject{parent}
{

}

void NetworkLayer::connectTo(const QHostAddress &addr, quint16 port)
{
    m_tcpSocket.connectToHost(addr, port);
}

void NetworkLayer::sendData(const QByteArray & data)
{
    m_tcpSocket.write(data); //assuming data is not big
}
