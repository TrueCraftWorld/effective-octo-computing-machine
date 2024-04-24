#include "networklayer.h"
#include <QNetworkInterface>

NetworkLayer::NetworkLayer(QObject *parent)
    : QObject{parent},
      m_tcpSocket(new QTcpSocket(this))
{
    QObject::connect(m_tcpSocket, &QTcpSocket::readyRead, this, &NetworkLayer::dataRecieve);
}

void NetworkLayer::connectTo(const QHostAddress &addr, quint16 port)
{
    m_tcpSocket->connectToHost(addr, port);
}

void NetworkLayer::sendData(const QByteArray & data, char packet)
{
    QByteArray newArr(data);

    if (packet == DATA_IN) m_awaitedSize = newArr.size();
    do  {
        newArr.push_front(packet);
        m_tcpSocket->write(newArr.mid(0,MAX_DATA_FRAME));
        newArr = newArr.mid(MAX_DATA_FRAME);
    } while (newArr.size() > MAX_DATA_FRAME-1);
}

void NetworkLayer::socketSetup()
{
    m_tcpSocket->bind();

    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (auto addr : QNetworkInterface::allAddresses()) {
        if (!addr.isNull() && !addr.isLoopback() && (addr != localhost)) {
            m_tcpSocket->bind(addr,
                             m_tcpSocket->peerPort(),
                             QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
        }
        break;
    }
}

QByteArray &NetworkLayer::getData()
{
    return m_recievedData;
}

void NetworkLayer::dataRecieve()
{
    m_recievedData.append(m_tcpSocket->readAll().mid(1));
    //for now BAD check just in accepted SIZE
    if (m_recievedData.size() == m_awaitedSize) emit dataReady();
}
