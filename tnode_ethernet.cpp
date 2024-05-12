
/**
 *   \file     tnode_ethernet.cpp
 *   \version  0.05
 *   \date     2024.05.12
 */

#include "tnode_ethernet.h"
#include "node.h"

#include <QDataStream>
#include <QAbstractSocket>
#include <QNetworkInterface>
#include <QRegularExpression>


/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
TNode_Ethernet::TNode_Ethernet(QObject *parent, const Options_command_line &options_command_line)
    : QObject{parent}, multicast_ip(options_command_line.multicast_ip),
    multicast_port(options_command_line.multicast_port),
    verbose(options_command_line.verbose)
{
    find_host_ip();  /// Поиск ip адреса хоста

    multicast_address = QHostAddress(multicast_ip);

    // Приём датаграмм
    udp_socket_receive = new QUdpSocket(this);
    udp_socket_receive->setObjectName("udp_socket_receive");

    if (udp_socket_receive->bind(QHostAddress::AnyIPv4, multicast_port, QUdpSocket::ShareAddress))
    {
        if (udp_socket_receive->joinMulticastGroup(multicast_address))
        {
            connect(udp_socket_receive, &QUdpSocket::readyRead, this, &TNode_Ethernet::receive_datagram_multicast_mode);
        }
        else
        {
            qDebug() << "udp_socket_receive joinMulticastGroup error";
        }
    }
    else
    {
        qDebug() << "udp_socket_sender bind error";
    }

    // Передача датаграмм
    udp_socket_transmit = new QUdpSocket(this);
    udp_socket_transmit->setObjectName("udp_socket_sender");

    if (udp_socket_transmit->bind(QHostAddress(QHostAddress::AnyIPv4), 0))
    {
        udp_socket_transmit->setSocketOption(QAbstractSocket::MulticastTtlOption, qint32(Udp_socket_setting::TTL));
    }
}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
TNode_Ethernet::~TNode_Ethernet()
{
    udp_socket_receive->deleteLater();
    udp_socket_transmit->deleteLater();
}


/**
 *   \brief   Поиск ip адреса хоста
 *   \param   Нет
 *   \retval  Нет
 */
void TNode_Ethernet::find_host_ip(void)
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            host_ip_address = address;
            qDebug() << host_ip_address.toString();
        }
    }
}


/**
 *   \brief   Передача данных в сеть
 *   \param  *data - данные для отправки
 *   \retval  Нет
 */
void TNode_Ethernet::transmit_datagram_multicast_mode(QByteArray &data)
{
    QRegularExpression exp("node");
    QList<Node *> node = parent()->findChildren<Node *>(exp);


    if (node.at(0)->get_mode_node() == ModeNode::MN_WAIT)
    {
        udp_socket_transmit->writeDatagram(data, multicast_address, multicast_port);
    }
}


/**
 *   \brief   Прием данных из сети
 *   \param   Нет
 *   \retval  Нет
 */
void TNode_Ethernet::receive_datagram_multicast_mode(void)
{
    if (udp_socket_receive->hasPendingDatagrams())
    {
        QNetworkDatagram n_datagram = udp_socket_receive->receiveDatagram();


        if (n_datagram.senderPort() != multicast_port)
        {
            QByteArray ba;


            ba.clear();

            QDataStream stream(&ba, QIODevice::WriteOnly);


            stream.setVersion(QDataStream::Qt_5_15);
            stream << n_datagram.senderAddress();
            stream << (quint16) n_datagram.senderPort();
            stream << n_datagram.data().toHex().toUInt(nullptr, 16);

            emit  ethernet_data_ready(ba);
        }
    }
}
