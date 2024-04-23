
/**
 *   \file     list_node.h
 *   \version  0.02
 *   \date     2024.04.23
 */


#ifndef LIST_NODE_H
#define LIST_NODE_H


#include <QLinkedList>
#include <QtNetwork/QHostAddress>
#include <QVector>
#include "node.h"


/*
 *   Класс узлов
 */
class ListNode
{
private:
    QLinkedList<Node> list_nodes;

public:
    explicit ListNode();
    void append(Node node);
    void remove_ip(QString ip);
    void remove_port(quint16 port);
    void remove_all();
    Node value_ip(QString ip);
    Node value_port(quint16 port);
    bool find_ip(QString ip);
    bool find_port(quint16 port);
    QVector<Node> list_connected();
    void list_all();
    quint32 size();
};

#endif // LIST_NODE_H
