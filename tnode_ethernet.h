
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
    void transmit_datagram_local_mode(QByteArray &data);  /// прием данных из сети (transmit), локальный режим работы
    void receive_datagram_local_mode(void);  /// прием (receive) данных из сети, локальный режим работы

private:
    const quint16 local_mode_port = 49002;
    QUdpSocket *udp_socket_local = nullptr;
    QString multicast_ip;  /// мультикастовый ip адрес
    quint16 multicast_port;  /// порт мультикастового адреса
    QString local_mode_ip;  /// IP-адрес в локальном режиме
    bool local_mode;  /// режим работы узла, работа узла в локальном режиме
    bool verbose;  /// подробный вывод

    void init_local_mode();
};

#endif // TNODE_ETHERNET_H
