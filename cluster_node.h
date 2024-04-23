
/**
 *   \file     cluster_node.h
 *   \version  0.01
 *   \date     2024.04.23
 */

#ifndef CLUSTERNODE_H
#define CLUSTERNODE_H

#include <QObject>
#include <QTimer>
#include "node.h"
#include "list_node.h"


enum class StateNode { ID_WAIT, ID_WORK };
enum class StateWait { ID_CONNECT_CLIENT, ID_SIZE_DATA, ID_DATA };
enum class StateClient { ID_TASKER, ID_NODE };


class ClusterNode : public QObject
{
    Q_OBJECT

    public:
        explicit ClusterNode(QObject *parent = nullptr);

    signals:
        void connect_nodes(QString ip, quint16 port);  /**< соединение с узлом */
        void measure_speed_network(QString ip, quint16 port);  /**< измерить скорость сети */

    public slots:
        void handler_packet_udp(QString ip, quint16 port, quint32 priority,  double mips,  double throughput = 0.0);  /**< обработчик принятых udp пакетов */
        void handler_packet_tcp(QByteArray ba);  /**< обработчик принятых tcp пакетов */

    private:
        Node local_node;  /**< информация о узле */
        ListNode list_nodes;  /**< информация об узлах сети */
        StateNode state_node;  /**< автомат состояния узла */
        StateWait state_wait;  /**< автомат состояния в режиме ожитдания */
        StateClient state_client;  /**< автомат состояния подключения */

        quint32 priority();  /**< генерация приоритета для узла */
        double mips();  /**< замер вычислительной мощности узла */
        void add_node_to_list(QString ip, quint16 port, quint32 priority,  double mips,  double throughput, bool connected);  /**< добавление узла в список */
};

#endif // CLUSTERNODE_H
