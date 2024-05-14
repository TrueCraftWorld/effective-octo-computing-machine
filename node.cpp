
/**
 *   \file     node.cpp
 *   \version  0.06
 *   \date     2024.05.12
 */

#include "node.h"
#include "power_meter.h"
#include "tcpserver.h"
#include "nodeserializer.h"

#include <QRandomGenerator>
#include <QTimer>

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

    //Connection and disconnection of tcp-socket
    connect(this, &Node::node_info_updated, &m_tcpServer, &TcpModule::slotConnectSocket);
    connect(&m_tcpServer, &TcpModule::signalSocketDisconnected, this, &Node::slotTcpSocketDisonnected);
    connect(&m_tcpServer, &TcpModule::signalSocketConnected, this, &Node::slotTcpSocketConnected);

    // Connect UDP transfer for node
    connect(this, &Node::transmit_data_node, m_discovery_service, &DiscoveryService::transmit_data_node);

    // Init timer for TCP-socket life checker
    m_timerCheckerExistedTcpConnections = new QTimer(this);
    m_timerCheckerExistedTcpConnections->setInterval(TIME_CHECK_CONNECTION_MSEC);
    m_timerCheckerExistedTcpConnections->start();
    connect(m_timerCheckerExistedTcpConnections, &QTimer::timeout, this, &Node::slotCheckConnections);

    // Connect tcp-module and serializer
    connect(&m_tcpServer, &TcpModule::signalSendDataToSerializer, &m_serializer, &NodeSerializer::slotDeserializeMessage);
    connect(&m_serializer, &NodeSerializer::signalDataInfo, [](quint64 amount) { qDebug() << "Catch DataInfo: " << amount; });
    connect(&m_serializer, &NodeSerializer::signalDataPrep, [](quint64 amount) { qDebug() << "Catch DataPrep: " << amount; });
    connect(&m_serializer, &NodeSerializer::signalDataModified, []() { qDebug() << "Catch DataModified"; });

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

                emit node_info_updated(node_data.node_id);
            }
        }
        else
        {
            m_node_info.neighbour_nodes.push_back(node_data);
            qDebug() << node_data.node_id.ip << ":" << node_data.node_id.port;
            qDebug() << node_data.priority;

            emit node_info_updated(node_data.node_id);
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
    connect(&m_serializer, &NodeSerializer::signalFormula, m_data_storage_processing, &DataStorageProcessing::set_formula);
    connect(&m_serializer, &NodeSerializer::signalDataArray, m_data_storage_processing, &DataStorageProcessing::fill_data);
    connect(&m_serializer, &NodeSerializer::signalDataModified, m_data_storage_processing, &DataStorageProcessing::fill_data);
}

void Node::slotCheckConnections()
{
    QByteArray emptyArray;
    emptyArray.reserve(0);
    for (const auto& info : m_node_info.neighbour_nodes)
    {
        m_tcpServer.SendMessageToNode(info.socket, emptyArray);
    }
}

void Node::slotTcpSocketDisonnected(QTcpSocket* socket)
{
    m_node_info.neighbour_nodes.erase(
        std::remove_if(
            m_node_info.neighbour_nodes.begin(),
            m_node_info.neighbour_nodes.end(),
            [socket](const NodeData& data) { return data.socket == socket; }),
        m_node_info.neighbour_nodes.end());
    socket->deleteLater();
}

void Node::slotTcpSocketConnected(QTcpSocket* socket, QHostAddress ip4, quint16 port)
{
    bool findExistedNodeData = false;
    for (auto& info : m_node_info.neighbour_nodes)
    {
        if (info.node_id.ip == ip4 && info.node_id.port == port)
        {
            info.socket = socket;
            findExistedNodeData = true;
            break;
        }
    }
    if (!findExistedNodeData)
    {
        m_node_info.neighbour_nodes.push_back({ 0,0, {QHostAddress(), 0}, socket, false});
    }
}

void Node::timeout_timer_1hz(QObject* parent)
{
    if (get_mode_node() != ModeNode::MN_WAIT)
        return;

    quint32 priority = get_priority();
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);


    stream.setVersion(QDataStream::Qt_5_15);
    stream << m_tcpServer.serverPort();
    stream << priority;

    emit transmit_data_node(ba);
}
