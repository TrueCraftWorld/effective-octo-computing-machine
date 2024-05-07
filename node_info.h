
/**
 *   \file     node_info.h
 *   \version  0.02
 *   \date     2024.05.07
 */

#ifndef NODE_INFO_H
#define NODE_INFO_H

#include <QHostAddress>


/// Значения по умолчанию для UDP Socket
enum class Udp_socket_setting { TTL = 64, MULTICAST_PORT = 49001, LOCAL_MODE_RECEIVE_PORT = 49002, LOCAL_MODE_TRANSMIT_PORT = 49002 };

const QString default_ip = "225.1.1.130";  /// мультикастовый адрес по умполчанию

/// Описание передаваемых опций
typedef struct
{
    quint16 multicast_port;  /// порт в мультикастовом режиме
    QString multicast_ip;  /// IP-адрес в мультикастовом режиме
    quint16 local_mode_transmit_port;  /// порт отправки в локальном режиме
    quint16 local_mode_receive_port;  /// порто приема в локальном режиме
    QString local_mode_ip;  /// IP-адрес в локальном режиме
    bool local_mode;  /// режим работы узла, работа узла в локальном режиме
    bool verbose;  /// подробный вывод
} Options_command_line;


/// Состояния узла
enum class ModeNode {SN_WAIT, SN_WORK};


/// Информация соседних узлов
typedef struct
{
    quint32 mips;  /// вычислительная мощность
    quint32 priority;  /// приоритет
    QHostAddress ip;
    quint16 port;
    bool compression;
} NodeData;


/// Информация текущего узла
typedef struct
{
    ModeNode mode_node;  /// режим узла
    quint32 mips;  /// вычислительная мощность
    quint32 priority;  /// приоритет
    quint16 port;  /// порт
    std::list<NodeData> neighbour_nodes;  /// информация соседних узлов
} NodeInfo;

#endif // NODE_INFO_H
