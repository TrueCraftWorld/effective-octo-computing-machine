
/**
 *   \file     node.cpp
 *   \version  0.02
 *   \date     2024.05.07
 */

#include <QRandomGenerator>
#include "node.h"


/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
Node::Node(QObject *parent, const Options_command_line &options_command_line)
    : QObject{parent}
{
    m_node_info.mode_node = ModeNode::SN_WAIT;
    m_node_info.priority = QRandomGenerator::global()->generate();
    if (options_command_line.local_mode)
    {
        m_node_info.port = options_command_line.local_mode_receive_port;
    }
    else
    {
        m_node_info.port = options_command_line.multicast_port;
    }
    qDebug() << "Приоритет узла:" << m_node_info.priority;
    m_discovery_service = new DiscoveryService(parent, options_command_line);
    m_discovery_service->setObjectName("dis");

    connect(m_discovery_service, &DiscoveryService::data_ready, this, &Node::node_data);
}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
Node::~Node()
{
    m_discovery_service->deleteLater();
}


/**
 *   \brief   Функция возвращает приоритет узла
 *   \param   Нет
 *   \retval  Приоритет узла
 */
quint32 Node::get_priority() const
{
    return m_node_info.priority;
}


/**
 *   \brief   Обновление данных узлов
 *   \param   node_data - данные соседнего узла
 *   \retval  Нет
 */
void Node::node_data(NodeData &node_data)
{
    bool present = false;


    if (get_priority() != node_data.priority)
    {
        if (!m_node_info.neighbour_nodes.empty())
        {
            std::list<NodeData>::iterator it;


            for (it = m_node_info.neighbour_nodes.begin(); it != m_node_info.neighbour_nodes.end(); ++it)
            {
                if ((it->ip == node_data.ip) && (it->port == node_data.port) && (it->priority == node_data.priority))
                {
                    present = true;
                }
            }

            if (!present)
            {
                m_node_info.neighbour_nodes.push_back(node_data);

                emit node_info_updated();
            }
        }
        else
        {
            m_node_info.neighbour_nodes.push_back(node_data);

            emit node_info_updated();
        }
    }
}
