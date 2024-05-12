
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

    timer_1hz = new  QTimer(parent);
    timer_1hz->setObjectName("timer_1hz");
    connect(timer_1hz, &QTimer::timeout, [=] { timeout_timer_1hz(parent); });

    timeout_timer_1hz(parent);

    if (!timer_1hz->isActive())
    {
        timer_1hz->start(1000);
    }
    else
    {
        timer_1hz->stop();
        timer_1hz->start(1000);
    }
}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
DiscoveryService::~DiscoveryService()
{
    if (timer_1hz->isActive())
    {
        timer_1hz->stop();
    }

    timer_1hz->deleteLater();
    eth->deleteLater();
}


/**
 *   \brief   Обработчик сигнала от таймера, вызывается раз в секунду
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
void DiscoveryService::timeout_timer_1hz(QObject *parent)
{
    QRegularExpression exp("node");
    QList<Node *> node = parent->findChildren<Node *>(exp);


    if (!node.isEmpty())
    {
        quint32 priority = node.at(0)->get_priority();
        QByteArray ba;
        QDataStream stream(&ba, QIODevice::WriteOnly);


        stream.setVersion(QDataStream::Qt_5_15);
        stream << priority;

        emit transmit_data_node(ba);
    }
}


/**
 *   \brief   Обработчик сигнала от eth, вызывается по приему данных
 *   \param   data - указатель на родительский класс
 *   \retval  Нет
 */
void DiscoveryService::receive_data_node(QByteArray &data)
{
    NodeData node_data;
    QDataStream stream(&data, QIODevice::ReadOnly);


    stream.setVersion(QDataStream::Qt_5_15);
    stream >> node_data.node_id.ip;
    stream >> node_data.node_id.port;
    stream >> node_data.priority;

    // qDebug() << node_data.ip.toString();
    // qDebug() << node_data.port;
    // qDebug() << node_data.priority;

    emit data_ready(node_data);
}
