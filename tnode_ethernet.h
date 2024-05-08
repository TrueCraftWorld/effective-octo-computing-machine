
/**
 *   \file     tnode_ethernet.h
 *   \version  0.04
 *   \date     2024.05.08
 */

#ifndef TNODE_ETHERNET_H
#define TNODE_ETHERNET_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QNetworkDatagram>

#include "node_info.h"


class TNode_Ethernet : public QObject
{
    Q_OBJECT
public:
    explicit TNode_Ethernet(QObject *parent, const Options_command_line &options_command_line);
    ~TNode_Ethernet();

signals:
    void ethernet_data_ready(QByteArray &data);

public slots:
    void receive_datagram_multicast_mode(void);  /// прием (receive) данных из сети мультикастовый режим работы
    void transmit_datagram_multicast_mode(QByteArray &data);  /// передача (sender) в сеть, мультикастовый режим работы

private:
    QHostAddress host_ip_address;  /// IP адрес хоста
    QHostAddress multicast_address;  /// мультикастовый адрес
    QString multicast_ip;  /// мультикастовый ip адрес
    quint16 multicast_port;  /// порт мультикастового адреса
    QUdpSocket *udp_socket_receive = nullptr;
    QUdpSocket *udp_socket_transmit = nullptr;
    bool verbose;  /// подробный вывод

    void find_host_ip();
};

#endif // TNODE_ETHERNET_H
