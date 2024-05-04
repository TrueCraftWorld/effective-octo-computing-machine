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

    //заглушка
       const     char someByte = 0xA1;
    static QByteArray formArr;
//    if (packet == FORMULA) {
//        formArr.clear();
//        formArr = data;
//        formArr.push_front(packet);
//        return;
//    }
    //заглушка

//    if (packet == DATA_IN) {
//        m_awaitedSize = newArr.size();
//        do  {

//            newArr.push_front(packet);
//            //заглушка

////            if (m_awaitedSize == newArr.size()) {//мы тут первый и единственный раз
//                newArr.push_front(formArr);
//                quint64 size = m_awaitedSize/sizeof(double);
//                quint32 anotherSize =  m_awaitedSize;
//                //простите мне мой С-style но просили быструю и топорную заглушку
//                char * pSize = (char*)&size;
//                char * pAnotherSize = (char*)&anotherSize;
//                newArr.prepend(pAnotherSize, sizeof(quint64));
//                newArr.prepend(pSize, sizeof(quint32));
//                newArr.push_front(someByte);
////                qDebug << "tuta";
////            }
//            //заглушка
//            m_tcpSocket->write(newArr.mid(0,MAX_DATA_FRAME));
//            newArr = newArr.mid(MAX_DATA_FRAME);
//        } while (newArr.size() > MAX_DATA_FRAME-1);
//    }
}

void NetworkLayer::socketSetup()
{
    m_tcpSocket->bind();

    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (auto addr : QNetworkInterface::allAddresses()) {
        if (!addr.isNull() && !addr.isLoopback() && (addr != localhost)) {
            m_tcpSocket->bind(QHostAddress(QString("127.0.0.1")),
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
    if (m_recievedData.size() == m_awaitedSize) emit dataReady(m_recievedData);
}
