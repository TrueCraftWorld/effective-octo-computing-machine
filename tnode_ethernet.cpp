
/**
 *   \file     tnode_ethernet.cpp
 *   \version  0.04
 *   \date     2024.05.08
 */

#include <QDataStream>
#include <QAbstractSocket>

#include "tnode_ethernet.h"
#include "node.h"
#include "qregularexpression.h"


/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
TNode_Ethernet::TNode_Ethernet(QObject *parent, const Options_command_line &options_command_line)
    : QObject{parent}, multicast_ip(options_command_line.multicast_ip),
    multicast_port(options_command_line.multicast_port),
    local_mode_ip(options_command_line.local_mode_ip),
    local_mode(options_command_line.local_mode),
    verbose(options_command_line.verbose)
{
    if (options_command_line.local_mode)
    {
        init_local_mode();
        qDebug() << "Инициализация локального режима";
    }
}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
TNode_Ethernet::~TNode_Ethernet()
{
    udp_socket_local->deleteLater();
}


/**
 *   \brief   Прием (receive) данных из сети, локальный режим работы
 *   \param   Нет
 *   \retval  Нет
 */
void TNode_Ethernet::receive_datagram_local_mode(void)
{
    if (udp_socket_local->hasPendingDatagrams())
    {
        QNetworkDatagram n_datagram = udp_socket_local->receiveDatagram();


        if (n_datagram.destinationPort() == local_mode_port)
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


/**
 *   \brief   Передача (sender) данных в сеть, локальный режим работы
 *   \param  *data - данные для отправки
 *   \retval  Нет
 */
void TNode_Ethernet::transmit_datagram_local_mode(QByteArray &data)
{
    QRegularExpression exp("node");
    QList<Node *> node = parent()->findChildren<Node *>(exp);


    if (node.at(0)->get_mode_node() == ModeNode::MN_WAIT)
    {
        udp_socket_local->writeDatagram(data, QHostAddress::Broadcast, local_mode_port);
    }
}


/**
 *   \brief   Инициализация локального режима работы
 *   \param   Нет
 *   \retval  Нет
 */
void TNode_Ethernet::init_local_mode()
{
    udp_socket_local = new QUdpSocket(this);
    udp_socket_local->bind(QHostAddress(local_mode_ip), local_mode_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udp_socket_local, &QUdpSocket::readyRead, this, &TNode_Ethernet::receive_datagram_local_mode);
}
