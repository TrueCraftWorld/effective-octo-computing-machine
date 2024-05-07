
/**
 *   \file     discovery_service.h
 *   \version  0.03
 *   \date     2024.05.07
 */

#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H

#include <QObject>
#include <QTimer>

#include "tnode_ethernet.h"
#include "node_info.h"


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
    void timeout_timer_1hz(QObject *parent);  /// функция вызывается по таймеру раз в секунду
    void receive_data_node(QByteArray &data);  /// приёма данных по udp

private:
    TNode_Ethernet *eth = nullptr;
    QTimer *timer_1hz = nullptr;
};

#endif // DISCOVERYSERVICE_H
