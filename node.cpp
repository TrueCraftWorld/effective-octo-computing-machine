
/**
 *   \file     node.cpp
 *   \version  0.06
 *   \date     2024.05.12
 */

#include "node.h"
#include "power_meter.h"
#include "tcpserver.h"
#include "nodeserializer.h"
#include "compute_core.h"

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
    m_tcpServer = new TcpModule(this, m_node_info.port_host);
    m_serializer = new NodeSerializer();

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
    connect(this, &Node::node_info_updated, m_tcpServer, &TcpModule::slotConnectSocket);
    connect(m_tcpServer, &TcpModule::signalSocketDisconnected, this, &Node::slotTcpSocketDisonnected);
    connect(m_tcpServer, &TcpModule::signalSocketConnected, this, &Node::slotTcpSocketConnected);
    
    //TCP Additional data packages
    connect(m_tcpServer, &TcpModule::signalNodeDataTcp, this, &Node::slotSendTcpInfo);
    connect(m_serializer, &NodeSerializer::signalNodeDataTcp, this, &Node::slotUpdateTcpInfo);


    // Connect UDP transfer for node
    connect(this, &Node::transmit_data_node, m_discovery_service, &DiscoveryService::transmit_data_node);

    // Init timer for TCP-socket life checker
    m_timerCheckerExistedTcpConnections = new QTimer(this);
    m_timerCheckerExistedTcpConnections->setInterval(TIME_CHECK_CONNECTION_MSEC);
    m_timerCheckerExistedTcpConnections->start();
    connect(m_timerCheckerExistedTcpConnections, &QTimer::timeout, this, &Node::slotCheckConnections);

    // Connect tcp-module and serializer
    connect(m_tcpServer, &TcpModule::signalSendDataToSerializer, m_serializer, &NodeSerializer::slotDeserializeMessage);
    connect(m_serializer, &NodeSerializer::signalDataInfo, this, &Node::connect_client);
    connect(m_serializer, &NodeSerializer::signalDataPrep, this, &Node::connect_node);

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
*\brief   Функция возвращает результат скоростного бенчмарка
* \param   Нет
* \retval  Приоритет узла
*/
double Node::get_mips() const
{
    return m_node_info.mips;
}


/**
 *   \brief   Обновление данных узлов
 *   \param   node_data - данные соседнего узла
 *   \retval  Нет
 */
void Node::node_data(NodeData &node_data)
{
    bool present = false;


    if (get_priority() > node_data.priority)
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
void Node::connect_node(QTcpSocket* socket, quint64 amount_processed_data)
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

    m_data_storage_processing = new DataStorageProcessing(this, false, m_node_info, amount_processed_data);
    // m_data_storage_processing = new DataStorageProcessing(this, true, m_node_info, amount_processed_data);  /// TODO: заглушка, замениьт на строку выше, а эту удалить
    m_data_storage_processing->setObjectName("dst");
    connect(m_serializer, &NodeSerializer::signalDataArray, m_data_storage_processing, &DataStorageProcessing::fill_data);
    connect(m_serializer, &NodeSerializer::signalFormula, m_data_storage_processing, &DataStorageProcessing::set_formula);
    connect(m_serializer, &NodeSerializer::signalDataModified, m_data_storage_processing, &DataStorageProcessing::fill_modified_data);
    connect(m_data_storage_processing, &DataStorageProcessing::data_tasker_ready, this, &Node::slotSerializeDataArray);
    connect(m_data_storage_processing, &DataStorageProcessing::data_worker_ready, m_data_storage_processing, &DataStorageProcessing::calculateData);
    connect(this, &Node::node_calculated_own_part, m_data_storage_processing, &DataStorageProcessing::calculateData);

    
    connect(m_data_storage_processing, &DataStorageProcessing::modified_data_worker_ready, this, &Node::slotSerializeDataModified);
    connect(m_data_storage_processing, &DataStorageProcessing::modified_data_tasker_ready, this, &Node::slotSendResultToClient);
}

void Node::connect_client(QTcpSocket* socket, quint64 amount_processed_data)
// void Node::connect_client()
{
    // Удаляем клиента из "списка узлов"
    m_clientSocket = socket;
    m_node_info.neighbour_nodes.erase(
        std::remove_if(
            m_node_info.neighbour_nodes.begin(),
            m_node_info.neighbour_nodes.end(),
            [socket](const NodeData& data) { return data.socket == socket; }),
        m_node_info.neighbour_nodes.end());

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

    m_data_storage_processing = new DataStorageProcessing(this, true, m_node_info, amount_processed_data);
    // m_data_storage_processing = new DataStorageProcessing(this, true, m_node_info, amount_processed_data);  /// TODO: заглушка, замениьт на строку выше, а эту удалить
    m_data_storage_processing->setObjectName("dst");
    connect(m_serializer, &NodeSerializer::signalDataArray, m_data_storage_processing, &DataStorageProcessing::fill_data);
    connect(m_serializer, &NodeSerializer::signalFormula, m_data_storage_processing, &DataStorageProcessing::set_formula);
    connect(m_serializer, &NodeSerializer::signalDataModified, m_data_storage_processing, &DataStorageProcessing::fill_modified_data);
    connect(m_data_storage_processing, &DataStorageProcessing::data_tasker_ready, this, &Node::slotSerializeDataArray);
    connect(this, &Node::node_calculated_own_part, m_data_storage_processing, &DataStorageProcessing::calculateData);


    connect(m_data_storage_processing, &DataStorageProcessing::modified_data_tasker_ready, this, &Node::slotSendResultToClient);
}

void Node::slotCheckConnections()
{
    QByteArray emptyArray;
    emptyArray.reserve(0);
    for (const auto& info : m_node_info.neighbour_nodes)
    {
        m_tcpServer->SendMessageToNode(info.socket, emptyArray);
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

void Node::slotUpdateTcpInfo(double mips, quint32 priority, quint16 port, QTcpSocket* socket)
{
    for (auto& info : m_node_info.neighbour_nodes)
    {
        if (socket == info.socket)
        {
            info.mips = mips;
            info.priority = priority;
            info.node_id.port = socket->peerPort();
            info.node_id.ip = socket->peerAddress();
            break;
        }
    }
}

void Node::slotSendTcpInfo(QTcpSocket* socket)
{
    QByteArray tempByte;
    tempByte.resize(15);
    QDataStream streamByte(&tempByte, QIODevice::WriteOnly);
    streamByte.setVersion(QDataStream::Qt_5_15);
    streamByte << PKG_NODEDATATCP << m_tcpServer->serverPort() << m_node_info.mips << m_node_info.priority;
    m_tcpServer->SendMessageToNode(socket, tempByte);
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
        m_node_info.neighbour_nodes.push_back({ 0,0, {ip4, 0}, socket, false});
    }
}

void Node::timeout_timer_1hz(QObject* parent)
{
    if (get_mode_node() != ModeNode::MN_WAIT)
        return;

    quint32 priority = get_priority();
    double mips = get_mips();
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);


    stream.setVersion(QDataStream::Qt_5_15);
    stream << m_tcpServer->serverPort();
    stream << priority;
    stream << mips;

    emit transmit_data_node(ba);
}

void Node::slotSerializeDataPrep(QString selectedNode, quint64 dataWaiting)
{
    QHostAddress ip;
    quint16 port;

    ParseQStringToIpPort(selectedNode, ip, port);

    QByteArray msg;
    QDataStream stream(msg);
    stream << dataWaiting;

    for (const auto& info : m_node_info.neighbour_nodes)
    {
        if (info.node_id.ip.toIPv4Address() == ip.toIPv4Address() && info.node_id.port == port)
        {
            m_tcpServer->SendMessageToNode(info.socket, msg);
        }
    }
}

void Node::slotSerializeFormula(QString selectedNode, QByteArray formula)
{
    QHostAddress ip;
    quint16 port;

    ParseQStringToIpPort(selectedNode, ip, port);

    for (const auto& info : m_node_info.neighbour_nodes)
    {
        if (info.node_id.ip.toIPv4Address() == ip.toIPv4Address() && info.node_id.port == port)
        {
            m_tcpServer->SendMessageToNode(info.socket, formula);
        }
    }
}

void Node::slotSerializeDataArray(QVector<QPair<QString, QVector<double>>>& data_tasker, QByteArray& formula)
{
    for (auto& [str, data] : data_tasker)
    {
        QHostAddress ip;
        quint16 port;

        ParseQStringToIpPort(str, ip, port);
        if (ip.toIPv4Address() == QHostAddress().toIPv4Address())
        {
            QVector<double>& ptrData(data);
            emit node_calculated_own_part(nullptr, ptrData);
        }
        else
        {
            QTcpSocket* socket_send = nullptr;
            for (const auto& info : m_node_info.neighbour_nodes)
            {
                if (info.node_id.ip == ip && info.node_id.port == port)
                {
                    socket_send = info.socket;
                    break;
                }
            }

            QByteArray msg1;
            msg1.resize(0);
            QDataStream stream1(&msg1, QIODevice::WriteOnly);
            stream1.device()->reset();
            stream1 << PKG_DATAPREP;
            stream1 << quint64(data.size());

            m_tcpServer->SendMessageToNode(socket_send, msg1);

            QByteArray msg2;
            msg2.resize(sizeof(char) + formula.size());
            QDataStream stream2(&msg2, QIODevice::WriteOnly);
            stream2.device()->reset();
            stream2 << PKG_FORMULA;
            stream2 << formula;

            m_tcpServer->SendMessageToNode(socket_send, msg2);

            QByteArray msg3;
            msg3.resize(0);
            QDataStream stream3(&msg3, QIODevice::WriteOnly);
            stream3.device()->reset();
            stream3 << PKG_DATAARRAY;
            for (const auto& el : data)
            {
                stream3 << el;
            }

            m_tcpServer->SendMessageToNode(socket_send, msg3);
        }
    }
}


void Node::slotSerializeDataModified(QTcpSocket* selectedNode, QVector<double>& data)
{

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    msg.resize(0);
    stream << PKG_DATAMODIFIED;
    for (const auto& el : data)
    {
        stream << el;
    }
    m_tcpServer->SendMessageToNode(selectedNode, msg);
}

void Node::slotSendResultToClient(QVector<QPair<QString, QVector<double>>>& res)
{
    QByteArray msg;
    msg.resize(0);
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << PKG_DATAMODIFIED;
    for (const auto& [str, data] : res)
    { 
        for (const auto& el : data)
        {
            stream << el;
        }
    }
    m_tcpServer->SendMessageToNode(m_clientSocket, msg);
    m_data_storage_processing->deleteLater();
}

void Node::ParseQStringToIpPort(const QString& str, QHostAddress& ip, quint16& port)
{
    QStringList list = str.split(':');
    if (list.size() == 2)
    {
        ip = QHostAddress(list.at(0));
        port = list.at(1).toInt();
    }
    else
    {
        ip = QHostAddress(list.at(3));
        port = list.at(4).toInt();
    }
}
