
/**
 *   \file     node.h
 *   \version  0.02
 *   \date     2024.05.07
 */

#ifndef NODE_H
#define NODE_H

#include <QObject>

#include "node_info.h"
#include "discovery_service.h"


class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent,
                  const Options_command_line &options_command_line);
    ~Node();
    quint32 get_priority() const;

signals:
    void node_info_updated();  /// список с данными узлов обновился

public slots:
    void node_data(NodeData &node_data);  /// данные соседнего узла

private:
    NodeInfo m_node_info;
    DiscoveryService *m_discovery_service = nullptr;
};

#endif // NODE_H
