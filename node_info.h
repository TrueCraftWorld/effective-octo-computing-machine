
/**
 *   \file     node_info.h
 *   \version  0.04
 *   \date     2024.05.09
 */

#ifndef NODE_INFO_H
#define NODE_INFO_H

#include <QHostAddress>


/// Значения по умолчанию для UDP Socket
enum class Udp_socket_setting { TTL = 64, MULTICAST_PORT = 49001 };

const QString default_ip = "225.1.1.130";  /// мультикастовый адрес по умполчанию

/// Описание передаваемых опций
typedef struct
{
    quint16 multicast_port;  /// порт в мультикастовом режиме
    QString multicast_ip;  /// IP-адрес в мультикастовом режиме
    bool verbose;  /// подробный вывод
} Options_command_line;


/// Состояния узла
enum class ModeNode {MN_WAIT, MN_WORK};


/// IP и порт узла
typedef struct
{
    QHostAddress ip;
    quint16 port;
} NodeID;


/// Информация соседних узлов
typedef struct
{
    double mips;  /// вычислительная мощность
    quint32 priority;  /// приоритет
    NodeID node_id;  /// IP и порт узла
    bool compression;
} NodeData;


/// Информация об узлах
typedef struct
{
    ModeNode mode_node;  /// режим узла
    double mips;  /// вычислительная мощность
    quint32 priority;  /// приоритет
    QHostAddress ip_host;  /// IP
    quint16 port_host;  /// порт
    std::list<NodeData> neighbour_nodes;  /// информация соседних узлов
} NodeInfo;

#endif // NODE_INFO_H
