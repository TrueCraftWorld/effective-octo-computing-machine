
/**
 *   \file     node.h
 *   \version  0.01
 *   \date     2024.04.23
 */


#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QtNetwork/QHostAddress>


class Node
{
    public:
        explicit Node();
        explicit Node(QString ip, quint16 port, quint32 priority,  double mips,  double throughput = 0.0);
        void set_ip(QString ip);
        void set_port(quint16 port);
        void set_priority(quint32 priority);
        void set_mips(double mips);
        void set_throughput(double throughput);
        void set_connected(bool connected);
        QString get_ip();
        quint16 get_port();
        quint32 get_priority();
        double get_mips();
        double get_throughput();
        bool get_connected();

    private :
        QHostAddress ip;
        quint16 port;
        quint32 priority;
        double mips;
        double throughput;
        bool connected;
};

#endif // NODE_H
