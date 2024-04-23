#include "networklayer.h"
#include <QNetworkInterface>

NetworkLayer::NetworkLayer(QObject *parent)
    : QObject{parent}
{

}

void NetworkLayer::connectTo(const QHostAddress &addr, quint16 port)
{
    m_tcpSocket.connectToHost(addr, port);
}

void NetworkLayer::sendData(const QByteArray & data, char packet)
{
    QByteArray newArr(data);


    while (newArr.size() > MAX_DATA_FRAME-1) {
        newArr.push_front(packet);
        m_tcpSocket.write(newArr.mid(0,MAX_DATA_FRAME));
        newArr = newArr.mid(MAX_DATA_FRAME);
    }

}

void NetworkLayer::socketSetup()
{
    m_tcpSocket.bind();

    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (auto addr : QNetworkInterface::allAddresses()) {
        if (!addr.isNull() && !addr.isLoopback() && (addr != localhost)) {
            m_tcpSocket.bind(addr,
                             m_tcpSocket.peerPort(),
                             QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
        }
        break;
    }

}
