
/**
 *   \file     node.cpp
 *   \version  0.06
 *   \date     2024.05.12
 */

#include "node.h"
#include "power_meter.h"

#include <QRandomGenerator>


/**
 *   \brief   Конструктор
 *   \param  *parent - указатель на родительский класс
 *   \retval  Нет
 */
Node::Node(QObject *parent, const Options_command_line &options_command_line)
    : QObject{parent}
{
    is_selected_node = false;
    m_node_info.mode_node = ModeNode::MN_WAIT;
    m_node_info.priority = QRandomGenerator::global()->generate();
    m_node_info.port_host = options_command_line.multicast_port;
    qDebug() << "Приоритет узла:" << m_node_info.priority;

    PowerMeter pow;
    m_node_info.mips = pow.power_check();
    // m_node_info.mips = QRandomGenerator::global()->bounded(1, 1000000);  /// TODO: заглушка бенчмарка, удалить
    qDebug() << "Вычислительная мощность узла:" << m_node_info.mips;
    m_discovery_service = new DiscoveryService(parent, options_command_line);
    m_discovery_service->setObjectName("dis");
    connect(m_discovery_service, &DiscoveryService::data_ready, this, &Node::node_data);

    /* TODO: удалить, начало */
    // timer = new  QTimer(parent);
    // timer->setObjectName("timer_1hz");
    // connect(timer, &QTimer::timeout, this, &Node::connect_client);


    // if (!timer->isActive())
    // {
    //     timer->start(20000);
    // }
    // else
    // {
    //     timer->stop();
    //     timer->start(20000);
    // }
    /* конец */
}


/**
 *   \brief   Деструктор
 *   \param   Нет
 *   \retval  Нет
 */
Node::~Node()
{
    m_discovery_service->deleteLater();
    m_data_storage_processing->deleteLater();

    // if (timer->isActive())
    // {
    //     timer->stop();
    // }

    // timer->deleteLater();
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
                if ((it->node_id.ip == node_data.node_id.ip) && (it->node_id.port == node_data.node_id.port) && (it->priority == node_data.priority))
                {
                    present = true;
                }
            }

            if (!present)
            {
                m_node_info.neighbour_nodes.push_back(node_data);
                qDebug() << node_data.node_id.ip << ":" << node_data.node_id.port;
                qDebug() << node_data.priority;

                emit node_info_updated();
            }
        }
        else
        {
            m_node_info.neighbour_nodes.push_back(node_data);
            qDebug() << node_data.node_id.ip << ":" << node_data.node_id.port;
            qDebug() << node_data.priority;

            emit node_info_updated();
        }
    }
}


/**
 *   \brief   Функция возвращает режим узла
 *   \param   Нет
 *   \retval  Режим узла
 */
ModeNode Node::get_mode_node()
{
    return m_node_info.mode_node;
}


/**
 *   \brief   Функция возвращает информацию об узлах
 *   \param   Нет
 *   \retval  Информацию об узлах
 */
NodeInfo &Node::get_node_info()
{
    return m_node_info;
}


/**
 *   \brief   Подключился клиент, строим план расперделения, считаем что вычислительный кластер готов к работе
 *   \param   amount_processed_data - количество обрабатываемых данных
 *   \retval  Нет
 */
void Node::connect_client(quint64 amount_processed_data)
// void Node::connect_client()
{
    // quint64 amount_processed_data = 1000000;  /// TODO: заглушка количества обрабатываемых данных, удалить
    qDebug() << "Количество узлов в вычислительном кластере" << m_node_info.neighbour_nodes.size() + 1;
    qDebug() << "Вычислительная мощьность текущего узла" << m_node_info.mips;
    if (!m_node_info.neighbour_nodes.empty())
    {
        qDebug() << "Вычислительные мощьности соседий";
        std::list<NodeData>::iterator it;

        for (it = m_node_info.neighbour_nodes.begin(); it != m_node_info.neighbour_nodes.end(); ++it)
        {
            // qDebug() << it->node_id.ip;
            // qDebug() << it->node_id.port;
            // qDebug() << it->priority;
            // it->mips = QRandomGenerator::global()->bounded(1, 1000000);  /// TODO: заглушка вычилсительной мощности, удалить
            // qDebug() << it->mips;
        }
    }

    m_data_storage_processing = new DataStorageProcessing(this, is_selected_node, m_node_info, amount_processed_data);
    // m_data_storage_processing = new DataStorageProcessing(this, true, m_node_info, amount_processed_data);  /// TODO: заглушка, замениьт на строку выше, а эту удалить
    m_data_storage_processing->setObjectName("dst");
}
