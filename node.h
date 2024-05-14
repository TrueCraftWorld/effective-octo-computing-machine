
/**
 *   \file     node.h
 *   \version  0.06
 *   \date     2024.05.12
 */

#ifndef NODE_H
#define NODE_H

#include "node_info.h"
#include "discovery_service.h"
#include "data_storage_processing.h"
#include "tcpserver.h"
#include "nodeserializer.h"

#include <QObject>

constexpr int TIME_CHECK_CONNECTION_MSEC = 3000;

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent,
                  const Options_command_line &options_command_line);
    ~Node();
    quint32 get_priority() const;
    ModeNode get_mode_node();
    NodeInfo &get_node_info();

signals:
    void node_info_updated(NodeID id);  /// список с данными узлов обновился
    void transmit_data_node(QByteArray& data);  /// отправка данных по udp

public slots:
    void node_data(NodeData &node_data);  /// данные соседнего узла
    void connect_client(quint64 np_data);  /// подключился клиент
    // void connect_client();  /// TODO: заглушка заменить на строку выше, эту строку удалить
    void slotTcpSocketConnected(QTcpSocket*, QHostAddress ip4, quint16 port);
    void slotTcpSocketDisonnected(QTcpSocket*);
    void slotCheckConnections();
    void timeout_timer_1hz(QObject* parent);  /// функция вызывается по таймеру раз в секунду

private:
    NodeInfo m_node_info;
    DiscoveryService *m_discovery_service = nullptr;
    DataStorageProcessing *m_data_storage_processing = nullptr;
    QTimer* m_timerCheckerExistedTcpConnections;
    QTimer* timer_1hz = nullptr;
    TcpModule m_tcpServer;
    NodeSerializer m_serializer;
    // QTimer *timer = nullptr;  /// TODO: для отладки, удалить
    bool is_selected_node;
};

#endif // NODE_H
