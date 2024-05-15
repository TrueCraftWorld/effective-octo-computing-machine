
/**
 *   \file     discovery_service.h
 *   \version  0.04
 *   \date     2024.05.12
 */

#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H

#include "tnode_ethernet.h"
#include "node_info.h"

#include <QObject>
#include <QTimer>


class DiscoveryService : public QObject
{
    Q_OBJECT
public:
    explicit DiscoveryService(QObject *parent,
                              const Options_command_line &options_command_line,
                              quint32 priority = 0);
    ~DiscoveryService();

signals:
    void transmit_data_node(QByteArray &data);  /// отправка данных по udp
    void data_ready(NodeData &node_data);  /// данные узла

public slots:
    void receive_data_node(QByteArray &data);  /// приёма данных по udp

private:
    TNode_Ethernet *eth = nullptr;
};

#endif // DISCOVERYSERVICE_H
