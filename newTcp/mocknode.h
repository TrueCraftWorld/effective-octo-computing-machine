#pragma once
#include "tcpserver.h"
#include "nodeserializer.h"

constexpr quint32 TIME_CHECK_CONNECTION_MSEC = 5000;

struct NodeData
{
	quint32 mips;
	quint32 priority;
	/* netWidth */
	QHostAddress ip;
	quint16 port;
	QTcpSocket* socket;
	bool compression;
};

struct NodeInfo
{
	quint32 mips;
	quint32 priority;
	// Have to be QLinkedList, not QVector
	QVector<NodeData> neighbour_nodes;
};

class MockNode : public QObject
{
	Q_OBJECT
public:
	MockNode(quint16 port);
	void SetNewConnection(const NodeData&);
signals:
	void signalNeighbourListUpdated(QHostAddress ip4, quint16 port);
public slots:
	void slotTcpSocketConnected(QTcpSocket*, QHostAddress ip4, quint16 port);
	void slotTcpSocketDisonnected(QTcpSocket*);
	void slotCheckConnections();

private:
	NodeInfo m_nodeInfo;
	Server m_tcpServer;
	NodeSerializer m_serializer;
	QTimer* m_timerCheckerExistedTcpConnections;
};