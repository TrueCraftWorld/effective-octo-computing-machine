
/**
 *   \file     cluster_node.cpp
 *   \version  0.01
 *   \date     2024.04.23
 */

#include <QHostAddress>
#include "cluster_node.h"


/**
 *   \brief   Конструктор
 *   \param  *parent -
 *   \return  Нет
 */
ClusterNode::ClusterNode(QObject *parent) : QObject{parent}
{
    QByteArray ba;

    local_node.set_ip(QHostAddress(QHostAddress::LocalHost).toString());
    local_node.set_port(48658);
    local_node.set_priority(priority());
    local_node.set_mips(mips());
    state_node = StateNode::ID_WAIT;
    state_wait = StateWait::ID_CONNECT_CLIENT;

    // TODO обработка сигнала по принятым данным udp
    qDebug() << list_nodes.size();
    handler_packet_udp("192.168.1.1", 2221, 50, 1000, 10);
    handler_packet_udp("192.168.1.2", 2222, 60, 1001, 11);
    handler_packet_udp("192.168.1.3", 2223, 70, 1002, 12);
    qDebug() << list_nodes.size();
    handler_packet_tcp(ba);

    qDebug() << "";
    qDebug() << "";
    list_nodes.list_all();

    qDebug() << "";
    list_nodes.remove_ip("192.168.1.2");

    qDebug() << list_nodes.size();
    list_nodes.list_all();

    qDebug() << "";
    list_nodes.remove_all();

    qDebug() << list_nodes.size();
    qDebug() << "";
}


/**
 *   \brief   Генерация приоритета
 *   \param   Нет
 *   \return  Приоритет узла
 */
quint32 ClusterNode::priority()
{
    return  18;
}


/**
 *   \brief   Замер вычислительной мощности узла
 *   \param   Нет
 *   \return  Вычислительная мощность узла
 */
double ClusterNode::mips()
{
    return  1000.0;
}


/**
 *   \brief   Конструктор с параметрами
 *   \param   ip - IP адрес узла в сети
 *   \param   port - порт узла в сети
 *   \param   priority - приоритет узла в сети
 *   \param   mips - вычислительная мощность узла в сети
 *   \param   throughput - пропускная способность до узла
 *   \return  Нет
 */
void ClusterNode::handler_packet_udp(QString ip, quint16 port, quint32 priority,  double mips,  double throughput)
{
    bool connected = false;


    if (ip == QHostAddress(QHostAddress::LocalHost).toString())
    {
        if (!list_nodes.find_port(port))
        {
            if (local_node.get_priority() < priority)
            {
                connected = true;
                emit  connect_nodes(ip, port);
            }

            add_node_to_list(ip, port, priority, mips, throughput, connected);
        }
    }
    else if (!list_nodes.find_ip(ip))
    {
        if (local_node.get_priority() < priority)
        {
            connected = true;
            emit  connect_nodes(ip, port);
        }
        else if (local_node.get_priority() == priority)
        {
            if ((local_node.get_ip().toUInt() & 0xFF) < (ip.toUInt() & 0xFF))
            {
                connected = true;
                emit  connect_nodes(ip, port);
            }
        }

        add_node_to_list(ip, port, priority, mips, throughput, connected);
    }
}


/**
 *   \brief   Замер вычислительной мощности узла
 *   \param   ba - принятые данные
 *   \return  Нет
 */
void ClusterNode::handler_packet_tcp(QByteArray ba)
{
    // TODO работаем с узлом (ID_NODE) или задачиком (ID_TASKER), переменно state_client присваеваем соответвующее состояние
    switch (state_node)
    {
        case  StateNode::ID_WAIT:
            switch (state_wait)
            {
                case StateWait::ID_CONNECT_CLIENT:
                    switch (state_client)
                    {
                        case StateClient::ID_TASKER:
                            state_wait = StateWait::ID_SIZE_DATA;
                            // TODO производим замер скорости

//                            QVector<Node>  nodes = list_nodes.list_connected();
//
//
//                            for ( quint32  i = 0; i < nodes.size(); i++  )
//                            {
//                                measure_speed_network( nodes[i].get_ip(), nodes[i].get_port() );
//                                qDebug() << nodes[i].get_ip();
//                                qDebug() << nodes[i].get_connected();
//                            }
//
                            break;

                        case StateClient::ID_NODE:
                            // TODO для iperf ожидаем результата замер скороости

                            break;

                    }

                    break;

                case StateWait::ID_SIZE_DATA:

                    break;

                case StateWait::ID_DATA:

                    break;
            }

            break;

        case  StateNode::ID_WORK:
            //

            break;
    }

    // TODO получить данные о задержки

    // TODO обработка пакета с временем задержки

    // TODO при получении размера (количество обрабатываемых элементов) входных данных произвести разделение по узлам

    // TODO отправить узлам функцию обработки

    // TODO получить функцию обработки

    // TODO отправить данные для обработки

    // TODO получить результат
}


/**
 *   \brief   Добавление узла в список
 *   \param   ip - IP адрес узла в сети
 *   \param   port - порт узла в сети
 *   \param   priority - приоритет узла в сети
 *   \param   mips - вычислительная мощность узла в сети
 *   \param   throughput - пропускная способность до узла
 *   \return  Нет
 */
void ClusterNode::add_node_to_list(QString ip, quint16 port, quint32 priority,  double mips,  double throughput, bool connected)
{
    Node node;


    node.set_ip(ip);
    node.set_port(port);
    node.set_priority(priority);
    node.set_mips(mips);
    node.set_throughput(throughput);
    node.set_connected(connected);

    list_nodes.append(node);
}
