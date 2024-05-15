
/**
 *   \file     discovery_service.cpp
 *   \version  0.06
 *   \date     2024.05.12
 */

#include "discovery_service.h"
#include "node.h"

#include <QRegularExpression>
#include <QDataStream>


/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
DiscoveryService::DiscoveryService(QObject *parent, const Options_command_line &options_command_line, quint32 priority)
    : QObject{parent}
{
    eth = new TNode_Ethernet(parent, options_command_line);
    eth->setObjectName("eth");
    connect(eth, &TNode_Ethernet::ethernet_data_ready, this, &DiscoveryService::receive_data_node);
    connect(this, &DiscoveryService::transmit_data_node, eth, &TNode_Ethernet::transmit_datagram_multicast_mode);

}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
DiscoveryService::~DiscoveryService()
{
    eth->deleteLater();
}


/**
 *   \brief   Обработчик сигнала от eth, вызывается по приему данных
 *   \param   data - указатель на родительский класс
 *   \retval  Нет
 */
void DiscoveryService::receive_data_node(QByteArray& data)
{
    NodeData node_data;
    quint32 sizeOfDatagram;
    QDataStream stream(&data, QIODevice::ReadOnly);


    stream.setVersion(QDataStream::Qt_5_15);
    stream >> node_data.node_id.ip;
    stream >> sizeOfDatagram;
    stream >> node_data.node_id.port;
    stream >> node_data.priority;
    stream >> node_data.mips;

    // qDebug() << node_data.ip.toString();
    // qDebug() << node_data.port;
    // qDebug() << node_data.priority;

    emit data_ready(node_data);
}

